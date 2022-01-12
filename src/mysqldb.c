/* SQLWork - the Oracle Query tool
 *
 * Copyright © Matthew Chappee <matthew@orasoft.org>
 *             Paul Flinders <paul@orasoft.org>
 *             Brent Gilmore <brent@orasoft.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2 
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <dlfcn.h>
#include <ctype.h>

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <mysql/mysql.h>

#include "common.h"
#include "support.h"
#include "miscfunc.h"
#include "dbif.h"

#define list_set_opt_col_width(clist, col)     \
    gtk_clist_set_column_width (GTK_CLIST(clist), (col), \
				gtk_clist_optimal_column_width (GTK_CLIST(clist), (col)))

static int	  mysql_initialised  = 0;
static GHashTable *login_hash_table  = 0;
static void	  *mysqlclient_handle;

/* local replacement of mysql functions which were macros in 3.22 & earlier */
static unsigned int mysql_mysql_errno(MYSQL *mysql);
static char * mysql_mysql_error(MYSQL *mysql);
static MYSQL_FIELD *mysql_mysql_fetch_fields(MYSQL_RES *res);
static unsigned int mysql_mysql_num_fields(MYSQL_RES *res);

MYSQL         *(*pMysql_init)(MYSQL *);
void           (*pMysql_close)(MYSQL *);
int            (*pMysql_query)(MYSQL *, const char *);
int            (*pMysql_real_query)(MYSQL *, const char *, unsigned int);
MYSQL_RES     *(*pMysql_use_result)(MYSQL *mysql);
void          *(*pMysql_free_result)(MYSQL_RES *result);
MYSQL_ROW      (*pMysql_fetch_row)(MYSQL_RES *result);
unsigned long *(*pMysql_fetch_lengths)(MYSQL_RES *result);

MYSQL         *(*pMysql_new_real_connect)(MYSQL *, const char *, const char *, const char *,
					  const char *, unsigned int, const char *, unsigned int);
MYSQL         *(*pMysql_old_real_connect)(MYSQL *, const char *, const char *, const char *,
					  unsigned int, const char *, unsigned int);

char          *(*pMysql_get_client_info)(void);
int            (*pMysql_select_db)(MYSQL *mysql, const char *db);
char          *(*pMysql_error)(MYSQL *mysql);
unsigned int   (*pMysql_errno)(MYSQL *mysql);
MYSQL_FIELD   *(*pMysql_fetch_fields)(MYSQL_RES *res);
unsigned int   (*pMysql_num_fields)(MYSQL_RES *res);

typedef struct {
    MYSQL	 *connection;
    
    /* the following are for the last executed query */
    int		 rowcount;
    int		 qtype;
    int		 errcode;
    char	 *errtxt;
    const char	 *current_script;
    const char	 *next_stmt;
} LoginData;

static void mysql_sql_error(LoginData *cd, char *context);

static int
mysql_initialise(void)
{
    if ((mysqlclient_handle = dlopen ("libmysqlclient.so", RTLD_LAZY)) == 0) {
	return DB_ERROR;
    }
    pMysql_init             = dlsym(mysqlclient_handle, "mysql_init");
    pMysql_close            = dlsym(mysqlclient_handle, "mysql_close");
    pMysql_query            = dlsym(mysqlclient_handle, "mysql_query");
    pMysql_real_query       = dlsym(mysqlclient_handle, "mysql_real_query");
    pMysql_use_result       = dlsym(mysqlclient_handle, "mysql_use_result");
    pMysql_free_result      = dlsym(mysqlclient_handle, "mysql_free_result");
    pMysql_fetch_row        = dlsym(mysqlclient_handle, "mysql_fetch_row");
    pMysql_fetch_lengths    = dlsym(mysqlclient_handle, "mysql_fetch_lengths");
    pMysql_new_real_connect = dlsym(mysqlclient_handle, "mysql_real_connect");
    pMysql_old_real_connect = dlsym(mysqlclient_handle, "mysql_real_connect");
    pMysql_get_client_info  = dlsym(mysqlclient_handle, "mysql_get_client_info");
    pMysql_select_db        = dlsym(mysqlclient_handle, "mysql_select_db");
    pMysql_errno            = dlsym(mysqlclient_handle, "mysql_errno");
    if (!pMysql_errno)
	pMysql_errno = mysql_mysql_errno;
    pMysql_error            = dlsym(mysqlclient_handle, "mysql_error");
    if (!pMysql_error)
	pMysql_error = mysql_mysql_error;
    pMysql_fetch_fields     = dlsym(mysqlclient_handle, "mysql_fetch_fields");
    if (!pMysql_fetch_fields)
	pMysql_fetch_fields = mysql_mysql_fetch_fields;
    pMysql_num_fields       = dlsym(mysqlclient_handle, "mysql_num_fields");
    if (!pMysql_num_fields)
	pMysql_num_fields = mysql_mysql_num_fields;

    login_hash_table =  g_hash_table_new(g_str_hash, g_str_equal);
    mysql_initialised = 1;
    return DB_SUCCESS;
}

static int
mysql_do_connect(
    MYSQL *connection,
    char  *host,
    char  *user,
    char  *passwd,
    char  *db)
{
    int          vmajor;
    int          vminor;
    int          old_api = 1;

    if (pMysql_get_client_info) {
      sscanf(pMysql_get_client_info(), "%d.%d", &vmajor, &vminor);
      
	if (vmajor > 3 
	    || (vmajor == 3 && vminor >= 22)) {
	    old_api = 0;
	}
    }

    if (old_api) {
	if (!pMysql_old_real_connect(connection,host,user,passwd,0,NULL,0)
	    || !pMysql_select_db(connection, db)) {
	    return DB_ERROR;
	}
    } else {
	if (!pMysql_new_real_connect(connection,host,user,passwd,db,0,NULL,0)) {
	    return DB_ERROR;
	}
    }
    return DB_SUCCESS;
}

static void
mysql_login_list_db(
    GtkWidget    *combo,
    char         *user,
    char         *password,
    char         *host)
{
    MYSQL        *conn = pMysql_init(0);
    GList        *dblist = 0;
    GList        *l;

    if (conn) {
	if (mysql_do_connect(conn, host, user, password, "mysql") == DB_SUCCESS) {
	    if (!pMysql_query(conn, "show databases")) {
		MYSQL_RES *result;
		
		if ((result = pMysql_use_result(conn)) != 0) {
		    MYSQL_ROW row;

		    while((row = pMysql_fetch_row(result))) {
			dblist = g_list_prepend(dblist, g_strdup(*row));
		    }
		    gtk_combo_set_popdown_strings(GTK_COMBO(combo), dblist);
		    pMysql_free_result(result);
		    for (l = dblist; l; l = g_list_next(l)) {
			g_free(l->data);
		    }
		    g_list_free(dblist);
		}
	    }
	}
    }
    pMysql_close(conn);
}


static void
mysql_init_login(
    GtkWidget	 *dialog,
    const char	 *cstring)
{
    gchar	 *username  = 0;
    gchar	 *password  = 0;
    gchar	 *hostname  = 0;
    GtkWidget	 *w	    = lookup_widget(dialog, "mysql_login_username");
    
    if (cstring && *cstring) {
	username  = cstring_getusername(cstring);
	password  = cstring_getpassword(cstring);
	hostname  = cstring_gethost(cstring);
    }
    if (!hostname)
	hostname = g_getenv("MYSQL_HOST");
    if (!username)
	username = g_get_user_name();

    assert(w);
    gtk_entry_set_text(GTK_ENTRY(w), username);

    if (password) {
	w = lookup_widget(dialog, "mysql_login_passwd");
	assert(w);
	gtk_entry_set_text(GTK_ENTRY(w), password);
    }
    
    if (hostname) {
	w = lookup_widget(dialog, "mysql_login_hostname");
	assert(w);
	gtk_entry_set_text(GTK_ENTRY(w), hostname);
    }
    w = lookup_widget(dialog, "mysql_login_database");
    assert(w);

    mysql_login_list_db(w, username, password, hostname);
}

static void
mysql_get_cstring(
    GtkWidget *dialog)
{
    char       **cstringp = (char **)gtk_object_get_data (GTK_OBJECT (dialog),
							  "cstring_ptr");

    if (cstringp) {
	GtkWidget  *w = lookup_widget(dialog, "mysql_login_username");
	gchar	   *username;
	gchar	   *password;
	gchar	   *hostname;
	gchar	   *dbname;
	GString	   *cstring;

	assert(w);
	username = gtk_entry_get_text(GTK_ENTRY(w));

	w = lookup_widget(dialog, "mysql_login_passwd");
	assert(w);
	password = gtk_entry_get_text(GTK_ENTRY(w));

	w = lookup_widget(dialog, "mysql_login_hostname");
	assert(w);
	hostname = gtk_entry_get_text(GTK_ENTRY(w));

	w = lookup_widget(dialog, "mysql_login_database");
	assert(w);
	dbname = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(w)->entry));

	cstring = g_string_new(username);
	if (password && *password)
	    g_string_sprintfa(cstring, "/%s", password);
	if (!hostname || !*hostname)
	    hostname = getenv("MYSQL_HOST");
	if (!hostname || !*hostname)
	    hostname = "localhost";
	if (hostname && *hostname) {
	    g_string_sprintfa(cstring, "@%s", hostname);
	}
	if (dbname && *dbname)
	    g_string_sprintfa(cstring, ":%s", dbname);
	*cstringp = cstring->str;
	g_string_free(cstring, 0);
    }
}

static int
mysql_login(
    const char	 *cstring,
    const char	 *dbid)
{
    LoginData	 *cd;
    char	 *user	  = cstring_getusername(cstring);
    char	 *passwd  = cstring_getpassword(cstring);
    char	 *host	  = cstring_gethost(cstring);
    char	 *db	  = cstring_getdb(cstring);

    
    assert(mysql_initialised);
    
    if (g_hash_table_lookup (login_hash_table, dbid)) {
	return 1;
    }

    /*
     * Connect and create an user session
     */
    cd = g_malloc0(sizeof(LoginData));
    cd->connection = pMysql_init(0);

    if (mysql_do_connect(cd->connection,host,user,passwd,db) == DB_ERROR) {
	mysql_sql_error(cd, "Failed to connect to database:");
	pMysql_close(cd->connection);
	g_free(cd);
	return DB_ERROR;
    }

    g_hash_table_insert (login_hash_table, g_strdup(dbid), cd);

    return DB_SUCCESS;
}

static int
mysql_close_conn(
    const char	 *dbid)
{
    LoginData	 *cd;

    assert(mysql_initialised);
    if ((cd = (LoginData *)g_hash_table_lookup (login_hash_table, dbid)) == 0) {
	return DB_ERROR;
    }
    pMysql_close(cd->connection);
    g_hash_table_remove (login_hash_table, dbid);
    g_free(cd);
}

static void
mysql_logoff(
    gpointer	key,
    gpointer	value,
    gpointer	user_data)
{
    pMysql_close (((LoginData *)value)->connection);
}

static int
mysql_finalise(void)
{
    assert(mysql_initialised);
    g_hash_table_foreach(login_hash_table, mysql_logoff, 0);

    return 0;
}


/* Start parsing a script */
static int 
mysql_start_script(
    const char	 *sql_script, 
    const char	 *dbid)
{
    LoginData	 *cd;

    if ((cd = (LoginData *)g_hash_table_lookup (login_hash_table, dbid)) == 0) {
	return DB_ERROR;
    }
    cd->current_script = sql_script;
    cd->next_stmt = sql_script;
    return DB_SUCCESS;
}

static const char *
skip_to_stmt_end(
    const char *stmt,
    char **stmt_out)
{
    /* this function is used to delimit statements, it also removes "--"
       comments (which the database itself doesn't understand).  Basically it
       looks for a ";" which is not embedded in a comment, a string or a quoted
       identifier
       
       I wanted something quick and cheerful for this - a hand implemented
       table driven FSA seemed best. It takes under 8k for the table (which is
       pretty sparse so could be compressed but it hardly seems worth it).

       There's a limited action on each transition you can either copy
       the input character to the output, append the input character to
       a hold buffer, append the hold buffer to the output or discard the
       hold buffer contects */
       
    /* states: whte - whitespace at start of statement
               body - nothing special
               lco* - comment terminated by end of line
	       cco* - C style comment
	       sst* - string delimeted by single quotes
	       dst* - string delimeted by double quotes
	       bqt* - character following backquote (within string)
	       lit  - quoted name eg `foo`
	       term - all done */
    typedef enum {start = 1, whte = start, body, lcom, lco1, lco2, 
		  ccom, cco1, cco2, sstr, sst1, dstr, dst1, bqts, 
		  bqtd, lit, term} fsa_state;
    typedef enum {copy = 1, hold = 2, transfer = 4, discard = 8} action;
    static struct {
	fsa_state     istate;
	unsigned char *ichar;
	fsa_state     nstate;
	action        act;
    } fsagen[] = {
	{whte, "**", body, copy},
	{whte, " ",  whte, 0},
	{whte, "\t", whte, 0},
	{whte, "\n", whte, 0},
	{whte, "\r", whte, 0},
	{whte, "\f", whte, 0},
	{body, "#",  lcom, 0},
	{body, "'",  sstr, copy},
	{body, "-",  lco1, hold},
	{body, "/",  cco1, copy},
	{body, "\"", dstr, copy},
	{body, "`",  lit,  copy},
	{body, ";",  term, 0},
	{bqtd, "**", dstr, copy},
	{bqts, "**", sstr, copy},
	{cco1, "**", body, copy},
	{cco1, "*",  ccom, copy},
	{cco2, "**", ccom, copy},
	{cco2, "/",  body, copy},
	{ccom, "*",  cco2, copy},
	{dst1, "**", body, copy},
	{dst1, "\"", dstr, copy},
	{dst1, ";",  term, copy},
	{dstr, "\"", dst1, copy},
	{dstr, "\\", bqtd, copy},
	{lco1, "**", body, transfer|copy},
	{lco1, "-",  lco2, hold},
	{lco1, "'",  sstr, transfer|copy},
	{lco1, "\"", dstr, transfer|copy},
	{lco2, "**", body, transfer|copy},
	{lco2, " ",  lcom, discard},
	{lco2, "'",  sstr, transfer|copy},
	{lco2, "\"", dstr, transfer|copy},
	{lco2, "\t", lcom, discard},
	{lcom, "**", lcom, 0},
	{lcom, "\n", body, 0},
	{sst1, "**", body, copy},
	{sst1, "'",  sstr, copy},
	{sst1, ";",  term, copy},
	{sstr, "'",  sst1, copy},
	{sstr, "\\", bqts, copy},
	{lit , "`",  body, copy},
	{0, 0, 0, 0} };

    static unsigned short **fsa	   = 0;
    char		  holdb[16];
    int			  held	   = 0;
    fsa_state		  state	   = start;
    GString		  *ostmt   = g_string_sized_new(512);

    /* As we pack action,next_state into 16 bits, there must be 255 or
       fewer states */
    assert(term < 256);
    if (!fsa) {
	/* Expand the packed definition into a table which can be indexed by
	   [current state, input char] to give the new FSA state. "**" in an
	   entry means any character triggers that state transition - this is
	   generally used as a default with later entries in the packed
	   definition giving a different transition */
	int i;
	
	fsa = g_malloc((int)(term+1) * sizeof(unsigned short *));
	fsa[0] = 0;
	for (i = 1; i < term+1; i++) {
	    int c;
	    fsa[i] = g_malloc(256*sizeof(unsigned short));
	    for (c = 0; c < 256; fsa[i][c++] = 0x0100) ;
	}
	for (i = 0; fsagen[i].ichar; i++) {
	    unsigned short code = (fsagen[i].nstate & 0xff) | ((short)fsagen[i].act << 8);
	    if (fsagen[i].ichar[0] == '*' && fsagen[i].ichar[1] == '*') {
		int c;

		for (c = 0; c < 256; fsa[fsagen[i].istate][c++] = code) ;
	    } else {
		fsa[fsagen[i].istate][fsagen[i].ichar[0]] = code;
	    }
	}
    }

    /* The main FSA. */
    while (stmt && *stmt) {
	fsa_state nstate = fsa[state][*(unsigned char *)stmt] & 0xff;
	action act = (fsa[state][*(unsigned char *)stmt] & 0xff00) >> 8;

	if ((int)nstate != 0)
	    state = nstate;

	/* Actions, note copy is after transfer so that transfer|copy does the
	   least surprising thing */
	if (act & hold) {
	    holdb[held++] = *stmt;
	    assert(held < sizeof(holdb));
	}
	if (act & transfer) {
	    holdb[held] = '\0';
	    g_string_append(ostmt, holdb);
	    held = 0;
	}
	if (act & discard)
	    held = 0;
	if (act & copy)
	    g_string_append_c(ostmt, *stmt);
	
	if (state == term)
	    break;
	++stmt;
    }
    *stmt_out = ostmt->str;
    g_string_free(ostmt, 0);
    return stmt;
}

static char *
mysql_fetch_statement(
    const char	 *dbid)
{
    LoginData	 *cd;
    const char	 *start;
    const char	 *end;
    char	 *stmt;

    if ((cd = (LoginData *)g_hash_table_lookup (login_hash_table, dbid)) == 0) {
	return 0;
    }

    start = cd->next_stmt;
    if (start && *start) {
	end = skip_to_stmt_end(start, &stmt);
	if (*end)
	    cd->next_stmt = end + 1;
	else
	    cd->next_stmt = 0;
	if (*stmt == '\0') {
	    g_free((char *)stmt);
	    return 0;
	} else {
	    /* fprintf(stderr, "%s\n", stmt); */
	    return stmt;
	}
    } else {
	return 0;
    }
}

/* translate mysql type to internal sqlwork type */
static short
type_lookup(
    enum enum_field_types mysqltype)
{
    switch(mysqltype) {
    case FIELD_TYPE_DECIMAL: 
	return COLTYPE_DECIMAL;
    case FIELD_TYPE_TINY:
    case FIELD_TYPE_SHORT:
    case FIELD_TYPE_LONG:
    case FIELD_TYPE_LONGLONG:
    case FIELD_TYPE_INT24:
    case FIELD_TYPE_YEAR:
	return COLTYPE_INT;
    case FIELD_TYPE_FLOAT:
    case FIELD_TYPE_DOUBLE:
	return COLTYPE_FLOAT;
    case FIELD_TYPE_TIMESTAMP:
	return COLTYPE_TIMESTAMP;
    case FIELD_TYPE_DATE:
	return COLTYPE_DATE;
    case FIELD_TYPE_TIME:
	return COLTYPE_TIME;
    case FIELD_TYPE_DATETIME:
	return COLTYPE_DATETIME;
    case FIELD_TYPE_TINY_BLOB:
    case FIELD_TYPE_MEDIUM_BLOB:
    case FIELD_TYPE_LONG_BLOB:
    case FIELD_TYPE_BLOB:
	return COLTYPE_BLOB;
    case FIELD_TYPE_ENUM:
    case FIELD_TYPE_SET:
    case FIELD_TYPE_VAR_STRING:
    case FIELD_TYPE_STRING:
	return COLTYPE_VARCHAR; 
    default:
	return 0;
    }
}

/* Do a query - for each row returned pass the data to a callback */
/* FIXME - write the other routines in this file in terms of this one */
static int
mysql_do_query(
    const char	    *sql_query,
    const char	    *databaseid,
    int		    max_rows,
    RowcallbackFunc rowcallback,
    gpointer	    user_data)
{
    LoginData	    *cd;
    MYSQL_FIELD	    *fields;
    MYSQL_RES	    *result = 0;
    MYSQL_ROW	    row;
    unsigned int    i;
    unsigned int    num_fields;
    int retval      = DB_ERROR;

    int             rownum;
    int             col;
    char	    **colnames	   = 0;
    short	    *indicators	   = 0; 
    char	    **values	   = 0; 
    unsigned long   *values_l	   = 0; 
    unsigned short  *datatypes	   = 0;
    short	    *types	   = 0; 

    assert(mysql_initialised);
    if ((cd = (LoginData *)g_hash_table_lookup (login_hash_table, databaseid)) == 0) {
	return -1;
    }

    cd->rowcount = 0;
    cd->qtype = 0;
    cd->errcode = 0;
    if (cd->errtxt)
	g_free(cd->errtxt);
    cd->errtxt = 0;

    if (pMysql_query(cd->connection, sql_query)) {
	mysql_sql_error(cd, "Query failed");
	goto cleanup;
    }

    if ((result = pMysql_use_result(cd->connection)) == 0 
	&& pMysql_errno(cd->connection)) {
	mysql_sql_error(cd, "Use result failed");
	goto cleanup;
    }


    if (result) {
	num_fields      = pMysql_num_fields(result);

	colnames	= g_malloc(num_fields * sizeof(char *));
	values	        = g_malloc(num_fields * sizeof(char *));
	values_l	= g_malloc(num_fields * sizeof(unsigned long));
	indicators	= g_malloc(num_fields * sizeof(short));
	datatypes	= g_malloc(num_fields * sizeof(unsigned short));
	
	for (col = 0; col < num_fields; col++) {
	    colnames[col] = 0;
	    values[col] = 0;
	}

	fields = pMysql_fetch_fields(result);
	for(i = 0; i < num_fields; i++) {
	    colnames[i] = fields[i].name;
	    datatypes[i] = type_lookup(fields[i].type);
	    values_l[i] = fields[i].length;
	}
	
	if (rowcallback)
	    rowcallback(-1, num_fields, colnames, 0, 0, values_l, datatypes, user_data);

	rownum = 0;
	while((row = pMysql_fetch_row(result))) {
	    /* make a copy of the lengths array so that we may update entries */
	    unsigned long *lengths = memcpy(g_malloc(num_fields * sizeof(unsigned long)),
					    pMysql_fetch_lengths(result),
					    num_fields * sizeof(unsigned long));

	    /* set indicators and storage for null values */
	    for(i = 0; i < num_fields; i++) {
		if ((indicators[i] = row[i] == 0 ? -1 : 0)) {
		    row[i] = g_strdup("<null>");
		    lengths[i] = 7;
		}
	    }
	    if (rowcallback)
		rowcallback(rownum++, num_fields, colnames, indicators, row,  
			    lengths, datatypes, user_data);
	    for(i = 0; i < num_fields; i++) {
		if (indicators[i] == -1) {
		    g_free(row[i]);
		}
	    }
	    g_free(lengths);

	}
	cd->rowcount = rownum;
	if(pMysql_errno(cd->connection)) { /* mysql_fetch_row() failed due to an error */
	    mysql_sql_error(cd, "Fetch failed");
	    goto cleanup;
	}
    }
    retval = DB_SUCCESS;
  cleanup:
    if (result) {
        pMysql_free_result(result);
    }
    if (colnames) {
	g_free(colnames);
    }
    if (values) {
	g_free(values);
    }
    if (indicators)
	g_free(indicators);
    if (datatypes)
	g_free(datatypes);
    if (values_l)
	g_free(values_l);
    if (types)
	g_free(types);
    return retval;
}

/* Execute the specified SQL statement */
static int
mysql_execute_sql(
    const char	 *databaseid,
    const char	 *sql_stmt,
    ...)
{
    return DB_ERROR;
}

static long int
mysql_get_rowcount(
    const char	 *dbid)
{
    LoginData    *cd;

    if ((cd = (LoginData *)g_hash_table_lookup (login_hash_table, dbid)) == 0) {
	return 0;
    }
    return cd->rowcount;
}

static long int
mysql_get_error_offset(
    const char	 *dbid)
{
    /* There's no way to determine the position of the parse error in 
       MySQL */
    return -1;
}

int 
mysql_end_script(
    const char *dbid)
{
    LoginData    *cd;

    if ((cd = (LoginData *)g_hash_table_lookup (login_hash_table, dbid)) == 0) {
	return DB_ERROR;
    }
    return DB_SUCCESS;
}

static int
plan_rowcallback(
    int		     row, 
    int		     ncols, 
    char	     **colnames, 
    const short	     *indicators, 
    char	     **values, 
    unsigned long    *values_l, 
    short	     *types, 
    gpointer	     user_data)
{
    GtkCList         **list = (GtkCList **)(user_data);

    if (row < 0) {
	assert (*list == 0);
	*list = (GtkCList *)gtk_clist_new_with_titles (ncols, colnames);
    } else {
	gtk_clist_append(GTK_CLIST(*list), values);

	/* Finally process events */
	while (gtk_events_pending())
	    gtk_main_iteration();
    }
    return TRUE;
}

static GtkWidget *
mysql_explain_plan(
    const char	 *query,
    const char	 *dbid,
    GtkWidget	 *widget)
{
    GtkCList	 *list = 0;
    int		 err;
    char	 *explain_query	 = g_malloc(strlen(query) + 9);

    sprintf(explain_query, "explain %s", query);

    err =  mysql_do_query(explain_query, dbid, 0, plan_rowcallback, &list);
    if (err == DB_SUCCESS) {
	return GTK_WIDGET(list);
    } else {
	gtk_widget_destroy(GTK_WIDGET(list));
	return 0;
    }
}

static int
mysql_q_interruptable(void)
{
    return FALSE;
}

static void
mysql_sql_error(
    LoginData	 *cd,
    char	 *context)
{
    char	 *errm	  = pMysql_error(cd->connection);

    if (errm && *errm)
	show_warning_msg("%s\n%s", context, errm);
}

/* For each of these functions mysql 3.22 and earlier provided a macro
   to access the relevant data whereas 3.23 and later provide a function.

   In order that this code may be used with both <= 3.22 and >= 3.23 client
   libraries function pointers are used (as with other functions) - if the
   function is not found then the pointer is set using the address of one of
   the functions below.

   Unfortunately there is still no guarantee that the functions below will work
   with any client library other than that against which they were compiled. In
   fact it is quite likely that the offsets of the fields *will* now change, or
   even that the data structures will become opaque. */

static unsigned int
mysql_mysql_errno(
    MYSQL *mysql)
{
    return mysql->net.last_errno;
}

static MYSQL_FIELD   *
mysql_mysql_fetch_fields(
    MYSQL_RES *res)
{
    return res->fields;
}

static unsigned int
mysql_mysql_num_fields(
    MYSQL_RES *res)
{
    return res->field_count;
}

static char *
mysql_mysql_error(
    MYSQL *mysql)
{
  return mysql->net.last_error;
}

gboolean
on_mysql_login_entry_focus_out_event(
    GtkWidget       *widget,
    GdkEventFocus   *event,
    gpointer         user_data)
{
    GtkWidget      *w = lookup_widget(widget, "mysql_login_username");
    gchar	   *username;
    gchar	   *password;
    gchar	   *hostname;
    
    assert(w);
    username = gtk_entry_get_text(GTK_ENTRY(w));
    
    w = lookup_widget(widget, "mysql_login_passwd");
    assert(w);
    password = gtk_entry_get_text(GTK_ENTRY(w));

    w = lookup_widget(widget, "mysql_login_hostname");
    assert(w);
    hostname = gtk_entry_get_text(GTK_ENTRY(w));

    w = lookup_widget(widget, "mysql_login_database");
    assert(w);

    mysql_login_list_db(w, username, password, hostname);
    
    return FALSE;
}


DbInterface mysqlIf =  {
    "MySql",
    mysql_initialise,
    mysql_finalise,
    cstring_getdbid,
    mysql_init_login,
    mysql_get_cstring,
    mysql_login,
    mysql_close_conn,
    mysql_start_script,
    mysql_fetch_statement,
    mysql_do_query,
    mysql_execute_sql,
    mysql_get_rowcount,
    mysql_get_error_offset,
    mysql_end_script, 
    mysql_explain_plan,
    mysql_q_interruptable};
