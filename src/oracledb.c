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

#include <oci.h>

#include "oracle.h"
#include "dbif.h"
#include "common.h"
#include "miscfunc.h"
#include "support.h"

static GdkPixmap *folder_pixmap=NULL;
static GdkBitmap *folder_mask;

/* icon used for plan tree */
static char *folder[] = {
"16 16 14 1",
"# c #000000",
"c c #008000",
"d c #00c000",
"g c #008080",
"h c #00c0c0",
"f c #00ffff",
"k c #808000",
"l c #c0c000",
"j c #ffff00",
"a c #c0ffc0",
"e c #c0ffff",
"i c #ffffc0",
"b c #00ff00",
". c None",
".....##.........",
"....#aa##.......",
"...#aaabb#......",
"..#bbbbbc###....",
"..#ddbbcc#ee##..",
"..#dddcc#eeeff#.",
"..#dddc#fffffg#.",
"...###c#hhffgg#.",
"...#ii##hhhggg#.",
"..#iiijj#hhgg#..",
".#jjjjjk##hg#...",
".#lljjkk#.##....",
".#lllkkk#.......",
".#lllkk#........",
"..##lk#.........",
"....##.........."};

#define MAX_COLS 256
#define MAX_VNAME_LEN 32

#define list_set_opt_col_width(clist, col)     \
    gtk_clist_set_column_width (GTK_CLIST(clist), (col), \
				gtk_clist_optimal_column_width (GTK_CLIST(clist), (col)))


typedef struct yy_buffer_state *YY_BUFFER_STATE;

static int	  oci_initialised    = 0;
static OCIEnv	  *envhp;
static OCIError	  *errhp;
static GHashTable *login_hash_table  = 0;
static void	  *libclnt_handle;

static sword (*pOCIAttrGet) (void *, ub4, void *, ub4 *, ub4, OCIError *);
static sword (*pOCIBindByPos) (OCIStmt *, OCIBind **, OCIError *, ub4, void *, sb4, ub2, void *, ub2 *, ub2 *, ub4, ub4 *, ub4);
static sword (*pOCIDefineByPos) (OCIStmt *, OCIDefine **, OCIError *, ub4, void *, sb4, ub2, void *, ub2 *, ub2 *, ub4);
static sword (*pOCIDefineObject) (OCIDefine *, OCIError *, OCIType *, void **, ub4 *, void **, ub4 *);
static sword (*pOCIEnvInit) (OCIEnv **, ub4, size_t, void **);
static sword (*pOCIErrorGet) (void *, ub4, OraText *, sb4 *, OraText *, ub4, ub4);
static sword (*pOCIHandleAlloc) (void *, void **, ub4, size_t, void **);
static sword (*pOCIHandleFree) (void *, ub4);
static sword (*pOCIInitialize) (ub4, void *, void *(*) (void *, size_t), void *(*) (void *, void *, size_t), void (*) (void *, void *));
static sword (*pOCILogoff) (OCISvcCtx *, OCIError *);
static sword (*pOCILogon) (OCIEnv *, OCIError *, OCISvcCtx **, OraText *, ub4, OraText *, ub4, OraText *, ub4);
static sword (*pOCIObjectPin) (OCIEnv *, OCIError *, OCIRef *, OCIComplexObject *, OCIPinOpt, OCIDuration, OCILockOpt, void **);
static sword (*pOCIParamGet) (void *, ub4, OCIError *, void **, ub4);
static sword (*pOCIStmtExecute) (OCISvcCtx *, OCIStmt *, OCIError *, ub4, ub4, OCISnapshot *, OCISnapshot *, ub4);
static sword (*pOCIStmtFetch) (OCIStmt *, OCIError *, ub4, ub2, ub4);
static sword (*pOCIStmtGetBindInfo) (OCIStmt *, OCIError *, ub4, ub4, sb4 *, OraText **, ub1 *, OraText **, ub1 *, ub1 *, OCIBind **);
static sword (*pOCIStmtPrepare) (OCIStmt *, OCIError *, OraText *, ub4, ub4, ub4);

YY_BUFFER_STATE yy_scan_string (const char *yy_str);
int yylex(char **ostmt, int *stmt_len);

typedef struct {
    OCISvcCtx	 *svchp;
    /* the following are for the last executed query */
    ub4          rowcount;
    int          qtype;
    int          errcode;
    int          errline;
    int          errchar;
    char         *errtxt;
    const char	 *current_script;
    const char	 *next_stmt;
} LoginData;

static void oracle_sql_error(LoginData *cd, OCIError *errhp);


static int
oracle_initialise(void)
{
    assert(!oci_initialised);

#if ORACLE_VERSION >= 80105
    if ((libclnt_handle = dlopen ("libclntsh.so", RTLD_LAZY)) == 0) {
	return DB_ERROR;
    }
    pOCIAttrGet         = dlsym(libclnt_handle, "OCIAttrGet");
    pOCIBindByPos       = dlsym(libclnt_handle, "OCIBindByPos");
    pOCIDefineByPos     = dlsym(libclnt_handle, "OCIDefineByPos");
    pOCIDefineObject    = dlsym(libclnt_handle, "OCIDefineObject");
    pOCIEnvInit         = dlsym(libclnt_handle, "OCIEnvInit");
    pOCIErrorGet        = dlsym(libclnt_handle, "OCIErrorGet");
    pOCIHandleAlloc     = dlsym(libclnt_handle, "OCIHandleAlloc");
    pOCIHandleFree      = dlsym(libclnt_handle, "OCIHandleFree");
    pOCIInitialize      = dlsym(libclnt_handle, "OCIInitialize");
    pOCILogoff          = dlsym(libclnt_handle, "OCILogoff");
    pOCILogon           = dlsym(libclnt_handle, "OCILogon");
    pOCIObjectPin       = dlsym(libclnt_handle, "OCIObjectPin");
    pOCIParamGet        = dlsym(libclnt_handle, "OCIParamGet");
    pOCIStmtExecute     = dlsym(libclnt_handle, "OCIStmtExecute");
    pOCIStmtFetch       = dlsym(libclnt_handle, "OCIStmtFetch");
    pOCIStmtGetBindInfo = dlsym(libclnt_handle, "OCIStmtGetBindInfo");
    pOCIStmtPrepare     = dlsym(libclnt_handle, "OCIStmtPrepare");
    
#else
    pOCIAttrGet         = &OCIAttrGet;
    pOCIBindByPos       = &OCIBindByPos;
    pOCIDefineByPos     = &OCIDefineByPos;
    pOCIDefineObject    = &OCIDefineObject;
    pOCIEnvInit         = &OCIEnvInit;
    pOCIErrorGet        = &OCIErrorGet;
    pOCIHandleAlloc     = &OCIHandleAlloc;
    pOCIHandleFree      = &OCIHandleFree;
    pOCIInitialize      = &OCIInitialize;
    pOCILogoff          = &OCILogoff;
    pOCILogon           = &OCILogon;
    pOCIObjectPin       = &OCIObjectPin;
    pOCIParamGet        = &OCIParamGet;
    pOCIStmtExecute     = &OCIStmtExecute;
    pOCIStmtFetch       = &OCIStmtFetch;
    pOCIStmtGetBindInfo = &OCIStmtGetBindInfo;
    pOCIStmtPrepare     = &OCIStmtPrepare;
#endif
    if (pOCIInitialize(OCI_OBJECT, 0, 
		      (dvoid * (*)(dvoid *, size_t)) 0,
		      (dvoid * (*)(dvoid *, dvoid *, size_t))0, 
		      (void (*)(dvoid *, dvoid *)) 0 )) {     
	return DB_ERROR;
    }
    
    if (pOCIEnvInit(&envhp, OCI_DEFAULT, 0, (dvoid **) 0 )) {
	return DB_ERROR;
    }

    if (pOCIHandleAlloc(envhp, (dvoid **)&errhp, (ub4) OCI_HTYPE_ERROR, 0, 0)) {
	return DB_ERROR;
    }
    login_hash_table =  g_hash_table_new(g_str_hash, g_str_equal);
    oci_initialised = 1;
    return DB_SUCCESS;
}

void
oracle_init_login(
    GtkWidget	 *dialog,
    const char	 *cstring)
{
    gchar	 *username  = 0;
    gchar	 *password  = 0;
    gchar	 *tnsname   = 0;
    GtkWidget	 *w	    = lookup_widget(dialog, "ora_login_username");
    
    if (cstring && *cstring) {
	username  = cstring_getusername(cstring);
	password  = cstring_getpassword(cstring);
	tnsname	  = cstring_gethost(cstring);
    }
    if (!tnsname)
	tnsname = g_getenv("ORACLE_SID");
    if (!username)
	username = g_get_user_name();

    assert(w);
    gtk_entry_set_text(GTK_ENTRY(w), username);

    if (password) {
	w = lookup_widget(dialog, "ora_login_passwd");
	assert(w);
	gtk_entry_set_text(GTK_ENTRY(w), password);
    }
    
    if (tnsname) {
	w = lookup_widget(dialog, "ora_login_tnsname");
	assert(w);
	gtk_entry_set_text(GTK_ENTRY(w), tnsname);
    }
}

void
oracle_get_cstring(
    GtkWidget *dialog)
{
    char       **cstringp = (char **)gtk_object_get_data (GTK_OBJECT (dialog),
							  "cstring_ptr");

    if (cstringp) {
	GtkWidget  *w = lookup_widget(dialog, "ora_login_username");
	gchar	   *username;
	gchar	   *password;
	gchar	   *tnsname;
	GString	   *cstring;

	assert(w);
	username = gtk_entry_get_text(GTK_ENTRY(w));

	w = lookup_widget(dialog, "ora_login_passwd");
	assert(w);
	password = gtk_entry_get_text(GTK_ENTRY(w));

	w = lookup_widget(dialog, "ora_login_tnsname");
	assert(w);
	tnsname = gtk_entry_get_text(GTK_ENTRY(w));

	cstring = g_string_new(username);
	if (password)
	    g_string_sprintfa(cstring, "/%s", password);
	if (tnsname)
	    g_string_sprintfa(cstring, "@%s", tnsname);
	*cstringp = cstring->str;
	g_string_free(cstring, 0);
    }
}

static int
oracle_login(
    const char	 *cstring,
    const char	 *dbid)
{
    LoginData	 *cd;
    char	 *user	  = cstring_getusername(cstring);
    char	 *passwd  = cstring_getpassword(cstring);
    char	 *tns	  = cstring_gethost(cstring);

    assert(oci_initialised);
    
    if (g_hash_table_lookup (login_hash_table, dbid)) {
	return 1;
    }

    /*
     * Connect and create an user session
     */

    cd = g_malloc0(sizeof(LoginData));
    if (pOCILogon(envhp, errhp, &cd->svchp, user, strlen (user),
		 passwd, strlen (passwd), tns, strlen(tns)) == OCI_ERROR) {
	g_free(cd);
	return DB_ERROR;
    }
    g_hash_table_insert (login_hash_table, g_strdup(dbid), cd);

    return DB_SUCCESS;
}

static int
oracle_close_conn(
    const char	 *dbid)
{
    LoginData	 *cd;

    assert(oci_initialised);
    if ((cd = (LoginData *)g_hash_table_lookup (login_hash_table, dbid)) == 0) {
	return DB_ERROR;
    }
    pOCILogoff (cd->svchp, errhp);
    g_hash_table_remove (login_hash_table, dbid);
    g_free(cd);
}

static void
oci_logoff(
    gpointer	key,
    gpointer	value,
    gpointer	user_data)
{
    pOCILogoff (((LoginData *)value)->svchp, errhp);
}

static int
oracle_finalise(void)
{
    assert(oci_initialised);
    g_hash_table_foreach(login_hash_table, oci_logoff, 0);

    if (errhp)
	pOCIHandleFree((dvoid *) errhp, OCI_HTYPE_ERROR);
    
    if (envhp)
	pOCIHandleFree((dvoid *) envhp, OCI_HTYPE_ENV);
    return DB_SUCCESS;
}


/* Start parsing a script */
static int 
oracle_start_script(
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
    int stmt_len;
    YY_BUFFER_STATE yybuf = yy_scan_string(stmt);

    yylex(stmt_out, &stmt_len);
    yy_delete_buffer(yybuf);
    return stmt+stmt_len;
}

static char *
oracle_fetch_statement(
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
	    fprintf(stderr, "%s\n", stmt);
	    return stmt;
	}
    } else {
	return 0;
    }
}

/* translate oracle type to internal sqlwork type */
static short
type_lookup(
    ub2 oratype)
{
    switch(oratype) {
      case SQLT_CHR:
      case SQLT_STR:
	  return COLTYPE_VARCHAR;
      case SQLT_NUM:
      case SQLT_INT:
	  return COLTYPE_INT;
      case SQLT_FLT:
	  return COLTYPE_FLOAT;
      default:
	return 0;
    }
}

/* Do a query - for each row returned pass the data to a callback */
/* FIXME - write the other routines in this file in terms of this one */
static int
oracle_do_query(
    const char	     *sql_query,
    const char	     *databaseid,
    int		     max_rows,
    RowcallbackFunc  rowcallback,
    gpointer	     user_data)
{
    OCIParam	     *parmdp	    = 0; 
    OCIStmt	     *stmthp	    = 0;
    LoginData	     *cd	    = 0;
    OCIDefine	     **defn	    = 0;
    char	     *colname;
    int		     col;
    int		     retval	    = DB_ERROR;
    long int	     err; 
    ub2		     datasize;
    ub2		     stmttype;
    ub4		     colnamelen;
    ub4		     parmcnt	    = 0; 
    
    char	     **colnames	    = 0;
    short	     *indicators    = 0; 
    char	     **values	    = 0; 
    ub2		     *lengths	    = 0; 
    unsigned long    *values_l	    = 0; 
    ub2		     *datatypes	    = 0;
    ub2		     *callbacktypes = 0;
    short	     *types	    = 0; 
    int		     row	    = 0; 
    OCIRef	     **refs	    = 0;
    ub4		     *ref_len	    = 0;
    
    void	     *indpp;
    void	     *indszp;

    indpp = g_malloc(1024);
    memset(indpp, '\0', 1024);
    
    indszp = g_malloc(1024);
    memset(indszp, '\0', 1024);

    assert(oci_initialised);
    if ((cd = (LoginData *)g_hash_table_lookup (login_hash_table, databaseid)) == 0) {
	return DB_ERROR;
    }

    cd->rowcount = 0;
    cd->qtype = 0;
    cd->errcode = 0;
    cd->errline = 0;
    cd->errchar = 0;
    if (cd->errtxt)
	g_free(cd->errtxt);
    cd->errtxt = 0;

    err = pOCIHandleAlloc( envhp, (dvoid **) &stmthp, OCI_HTYPE_STMT, 0, 0);

    if (err != OCI_SUCCESS) {
	fprintf(stderr, "OCIHandleAlloc(OCI_HTYPE_STMT) failed %ld\n", err);
	goto cleanup;
    }

    err = pOCIStmtPrepare (stmthp, errhp, (char *) /* Grrr */ sql_query,
			  (ub4)strlen(sql_query), 
			  (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT); 
    if (err != OCI_SUCCESS) {
	fprintf(stderr, "OCIStmtPrepare failed %ld\n", err);
	goto cleanup;
    }

    err = pOCIAttrGet(stmthp, OCI_HTYPE_STMT, &stmttype,
		     0, OCI_ATTR_STMT_TYPE, errhp);

    if (err != OCI_SUCCESS) {
	fprintf(stderr, "OCIAttrGet(OCI_ATTR_STMT_TYPE) failed %ld\n", err);
	oracle_sql_error(cd, errhp);
	goto cleanup;
    }

    err = pOCIStmtExecute (cd->svchp, stmthp, errhp, 
			  stmttype == OCI_STMT_SELECT ? 0 : 1, 0, 
			  (OCISnapshot*) 0, (OCISnapshot*)0, OCI_DEFAULT); 
 
    if (err != OCI_SUCCESS) {
	(void)pOCIAttrGet(stmthp, OCI_HTYPE_STMT, &cd->errchar, 0,
			 OCI_ATTR_PARSE_ERROR_OFFSET, errhp);
	
	fprintf(stderr, "OCIStmtExecute failed %ld\n", err);
	oracle_sql_error(cd, errhp);
	goto cleanup;
    }

    if (stmttype == OCI_STMT_SELECT) {
	/* */
    
	/* get the number of columns in the select list */ 
	err = pOCIAttrGet (stmthp, OCI_HTYPE_STMT, (dvoid *) &parmcnt, 
			  (ub4 *) 0, (ub4)OCI_ATTR_PARAM_COUNT, errhp); 

	if (err != OCI_SUCCESS) {
	    fprintf(stderr, "OCIAttrGet(OCI_ATTR_PARAM_COUNT) failed %ld\n", err);
	    oracle_sql_error(cd, errhp);
	    goto cleanup;
	}
	
	/* Allocate storage for callback and define handles */
	colnames	= g_malloc(parmcnt * sizeof(char *));
	values	        = g_malloc(parmcnt * sizeof(char *));
	lengths	        = g_malloc(parmcnt * sizeof(ub2));
	values_l	= g_malloc(parmcnt * sizeof(ub4));
	indicators	= g_malloc(parmcnt * sizeof(sb2));
	defn	        = g_malloc(parmcnt * sizeof(OCIDefine *));
	datatypes	= g_malloc(parmcnt * sizeof(ub2));
	callbacktypes	= g_malloc(parmcnt * sizeof(ub2));
	refs	        = g_malloc(parmcnt * sizeof(OCIRef *));
	ref_len    	= g_malloc(parmcnt * sizeof(ub4));
	
	for (col = 0; col < parmcnt; col++) {
	    colnames[col] = 0;
	    values[col] = 0;
	    defn[col] = 0;
	    refs[col] = 0;
	    ref_len[col] = 0;
	}

#define OCICGetAttr(attributep, sizep, attrtype, errhp) \
    if ((err = pOCIAttrGet(parmdp, OCI_DTYPE_PARAM, attributep, sizep, attrtype, errhp)) != OCI_SUCCESS) {\
        fprintf(stderr, "OCIAttrGet(" #attrtype ") failed\n");\
        oracle_sql_error(cd, errhp);\
	goto cleanup; }

#define OCICDefineByPos(defnpp, errhp, valuep, value_sz, dty, indp, rlenp) \
    if ((err = pOCIDefineByPos(stmthp, defnpp, errhp, col+1, valuep, value_sz,dty, \
                              indp, rlenp, 0, OCI_DEFAULT))!= OCI_SUCCESS) {\
        fprintf(stderr, "OCIDefineByPos for col %d failed\n", col+1);\
        oracle_sql_error(cd, errhp);\
	goto cleanup; }

	/* get describe information for each column */
	/* FIXME - the datatype values should be set to a db-neutral type code,
	   not the db-specific codes as here */
	/* FIXME - values_l is set here to the display size, not the
	   native size. The MySQL back end does the opposite */
	for (col = 0; col < parmcnt; col++) { 
	    err = pOCIParamGet (stmthp, OCI_HTYPE_STMT, errhp, (dvoid **)&parmdp, col+1); 
	    if (err != OCI_SUCCESS) {
		fprintf(stderr, "OCIParamGet(%d/%d) failed %ld\n", col, parmcnt, err);
		oracle_sql_error(cd, errhp);
		goto cleanup;
	    }
	    
	    OCICGetAttr(&colname, &colnamelen, OCI_ATTR_NAME, errhp);
	    /* There's no guarentee that there's room to terminate the
	       supplied string so we must copy it out */
	    colnames[col] = g_malloc(colnamelen+1);
	    memcpy(colnames[col], colname, colnamelen);
	    colnames[col][colnamelen] = '\0'; 

	    OCICGetAttr(&datatypes[col], (ub4 *) 0, OCI_ATTR_DATA_TYPE, errhp);
	    
	    callbacktypes[col] = type_lookup(datatypes[col]);
	    switch (datatypes[col]) {
	    case SQLT_RID:
	    case SQLT_DAT:
	    case SQLT_CHR:
	    case SQLT_AFC:
	    case SQLT_BIN:
	    case SQLT_NUM:
		/* DISP_SIZE isn't documented (thanks, Oracle) but appears
		   to be the size of string buffer you need */
		OCICGetAttr(&datasize, (ub4 *) 0, OCI_ATTR_DISP_SIZE, errhp);

		values_l[col] = (MAX(datasize+1, 7));
		values[col] = g_malloc(values_l[col]);
		OCICDefineByPos(&defn[col], errhp, values[col], values_l[col], 
				SQLT_STR, &indicators[col], &lengths[col]);
		break;
	    case SQLT_LNG:
	    case SQLT_LBI:
		datasize = 32767;
		values_l[col] = datasize;
		values[col] = g_malloc(values_l[col]);
		OCICDefineByPos(&defn[col], errhp, values[col], values_l[col], 
				SQLT_STR, &indicators[col], &lengths[col]);
		fprintf(stderr, "Nasty hack enabled to do *something* with LONGs\n");
		break;
	    case SQLT_NTY: {
		char	     *type_name	 = 0;
		ub2	     type_size	 = 0;
		ub1	     ptype;
		OCIRef	     *ref_tdo    = 0;
		OCIType	     *tdo	 = 0;


		OCICGetAttr(&ptype,     (ub4 *) 0, OCI_ATTR_PTYPE,     errhp);
		OCICGetAttr(&type_size, (ub4 *) 0, OCI_ATTR_DATA_SIZE, errhp);
		OCICGetAttr(&type_name, (ub4 *) 0, OCI_ATTR_TYPE_NAME, errhp);
		
		OCICGetAttr(&ref_tdo,   (ub4 *) 0, OCI_ATTR_REF_TDO,   errhp);
		err = pOCIObjectPin(envhp, errhp, ref_tdo, (OCIComplexObject *) 0,
				   OCI_PIN_ANY, OCI_DURATION_SESSION, OCI_LOCK_NONE,
				   (dvoid **)&tdo);

		if (err != OCI_SUCCESS) {
		    fprintf(stderr, "OCIObjectPin failed %ld\n", err);
		    oracle_sql_error(cd, errhp);
		    goto cleanup;
		}

		fprintf(stderr, "Col %d ptype %d type %s size %d\n", col, ptype, type_name, type_size);

		values_l[col] = 0;
		values[col] = 0;
		OCICDefineByPos(&defn[col], errhp, 0, 4, 
				SQLT_NTY, 0, 0);

		err = pOCIDefineObject(defn[col], errhp, tdo, &refs[col], &ref_len[col], 
				       indpp, indszp);
		if (err != OCI_SUCCESS) {
		    fprintf(stderr, "OCIDefineObject failed %ld\n", err);
		    oracle_sql_error(cd, errhp);
		    goto cleanup;
		}
		values[col] = g_strdup("<object>");

		break;
	    }

	    case SQLT_REF:
		fprintf(stderr, "REFs not supported yet %d\n", datatypes[col]);
		break;
	    case SQLT_CLOB:
	    case SQLT_BLOB:
		fprintf(stderr, "LOBs not supported yet %d\n", datatypes[col]);
		break;
	    default:
		fprintf(stderr, "No handler for datatype %d\n", datatypes[col]);
	    }
	}

	/* By this point we have the column definitions so call the
	   user supplied function (if there was one) */
	if (rowcallback)
	    rowcallback(-1, parmcnt, colnames, 0, 0, values_l, callbacktypes, user_data);


	while ((err = pOCIStmtFetch(stmthp, errhp, 1, 
				   OCI_FETCH_NEXT, OCI_DEFAULT)) == OCI_SUCCESS ||
	       err == OCI_SUCCESS_WITH_INFO) {
	    if (rowcallback) {
		/* widen the 2byte lengths that Oracle gives us to 4 byte lengths
		   for the callback */
		for (col = 0; col < parmcnt; col++)
		    values_l[col] = lengths[col];
		if (rowcallback(row++, parmcnt, colnames, indicators, 
				values, values_l, datatypes, user_data) == 0) {
		    break;
		}
	    }
	}
	if ( err != OCI_SUCCESS_WITH_INFO && err != OCI_SUCCESS && err != OCI_NO_DATA ) {
	    fprintf(stderr, "OCIStmtFetch failed on row %d\n", row);
	    oracle_sql_error(cd, errhp);
	    goto cleanup;
	}

	err = pOCIAttrGet(stmthp, OCI_HTYPE_STMT, &cd->rowcount,
			 0, OCI_ATTR_ROW_COUNT, errhp);
	if ( err != OCI_SUCCESS ) {
	    fprintf(stderr, "OCIAttrGet failed for OCI_ATTR_ROW_COUNT\n");
	    oracle_sql_error(cd, errhp);
	    goto cleanup;
	}
    }
 
    retval = DB_SUCCESS;
  cleanup:
    if (stmthp)
	pOCIHandleFree(stmthp, OCI_HTYPE_STMT);

    if (colnames) {
	for (col = 0; col < parmcnt; col++)
	    if (colnames[col])
		g_free(colnames[col]);
	g_free(colnames);
    }
    if (values) {
	for (col = 0; col < parmcnt; col++)
	    if (values[col])
		g_free(values[col]);
	g_free(values);
    }
    if (indicators)
	g_free(indicators);
    if (datatypes)
	g_free(datatypes);
    if (callbacktypes)
	g_free(callbacktypes);
    if (lengths)
	g_free(lengths);
    if (values_l)
	g_free(values_l);
    if (types)
	g_free(types);

    return retval;
}

/* Execute the specified SQL statement */
static int
oracle_execute_sql(
    const char	 *databaseid,
    const char	 *sql_stmt,
    ...)
{
    LoginData    *cd      = 0;
    OCIStmt	 *stmthp  = 0;
    int		 retval	  = DB_ERROR;
    long int	 err; 
    ub2		 stmttype;
    va_list	 ap;
    
    OraText	 **bvnp	  = 0;
    ub1		 *bvnl	  = 0;
    OraText	 **invp	  = 0;
    ub1		 *inpl	  = 0;
    ub1		 *dupl	  = 0;
    OCIBind	 **hndl	  = 0;
    sb4		 nbindvars;
    sb4          bv;

    assert(oci_initialised);

    if ((cd = (LoginData *)g_hash_table_lookup (login_hash_table, databaseid)) == 0) {
	return DB_ERROR;
    }

    err = pOCIHandleAlloc( envhp, (dvoid **) &stmthp, OCI_HTYPE_STMT, 0, 0);

    if (err != OCI_SUCCESS) {
	fprintf(stderr, "OCIHandleAlloc(OCI_HTYPE_STMT) failed %ld\n", err);
	goto cleanup;
    }

    err = pOCIStmtPrepare (stmthp, errhp, (char *) /* Grrr */ sql_stmt,
			  (ub4)strlen(sql_stmt), 
			  (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT); 
    if (err != OCI_SUCCESS) {
	fprintf(stderr, "OCIStmtPrepare failed %ld\n", err);
	goto cleanup;
    }

    nbindvars = 32;
    bvnp = g_malloc(nbindvars * sizeof(*bvnp));
    bvnl = g_malloc(nbindvars * sizeof(*bvnl));
    invp = g_malloc(nbindvars * sizeof(*invp));
    inpl = g_malloc(nbindvars * sizeof(*inpl));
    dupl = g_malloc(nbindvars * sizeof(*dupl));
    hndl = g_malloc(nbindvars * sizeof(*hndl));

    for (bv = 0; bv < nbindvars; bv++) {
	hndl[bv] = 0;
    }

    /* Find No of bind variables */

    err = pOCIStmtGetBindInfo ( stmthp, errhp, nbindvars, 1,
			       &nbindvars,
			       bvnp, bvnl, invp, inpl, dupl, hndl);
    
    if (err != OCI_SUCCESS &&  err != OCI_NO_DATA) {
	fprintf(stderr, "OCIStmtGetBindInfo failed %ld\n", err);
	oracle_sql_error(cd, errhp);
	goto cleanup;
    }

    if (nbindvars < 0) {
	nbindvars *= -1;
	bvnp = g_realloc(bvnp, nbindvars * sizeof(*bvnp));
	bvnl = g_realloc(bvnl, nbindvars * sizeof(*bvnl));
	invp = g_realloc(invp, nbindvars * sizeof(*invp));
	inpl = g_realloc(inpl, nbindvars * sizeof(*inpl));
	dupl = g_realloc(dupl, nbindvars * sizeof(*dupl));
	hndl = g_realloc(hndl, nbindvars * sizeof(*hndl));
	for (bv = 0; bv < nbindvars; bv++) {
	    hndl[bv] = 0;
	}

	err = pOCIStmtGetBindInfo ( stmthp, errhp, nbindvars, 1,
				   &nbindvars,
				   bvnp, bvnl, invp, inpl, dupl, hndl);

	if (err != OCI_SUCCESS) {
	    fprintf(stderr, "OCIStmtGetBindInfo failed %ld\n", err);
	    oracle_sql_error(cd, errhp);
	    goto cleanup;
	}

    }

    va_start(ap, sql_stmt);
    for (bv = 1; bv <= nbindvars; bv++) {
	char *var = va_arg(ap, char *);
	err = pOCIBindByPos(stmthp, &hndl[bv], errhp, bv,
			   var, strlen(var)+1, SQLT_STR, 
			   (dvoid *)0, (ub2 *)0, (ub2 *)0, (ub4)0, (ub4 *)0,
			   OCI_DEFAULT);
	if (err != OCI_SUCCESS) {
	    fprintf(stderr, "OCIBindByPos for %s failed\n", bvnp[bv-1]);
	    oracle_sql_error(cd, errhp);
	    goto cleanup;
	}
    }
    va_end(ap);

    err = pOCIAttrGet(stmthp, OCI_HTYPE_STMT, &stmttype,
		     0, OCI_ATTR_STMT_TYPE, errhp);

    if (err != OCI_SUCCESS) {
	fprintf(stderr, "OCIAttrGet(OCI_ATTR_STMT_TYPE) failed %ld\n", err);
	oracle_sql_error(cd, errhp);
	goto cleanup;
    }

    err = pOCIStmtExecute (cd->svchp, stmthp, errhp, stmttype == OCI_STMT_SELECT ? 0 : 1, 0, 
			  (OCISnapshot*) 0, (OCISnapshot*)0, OCI_DEFAULT); 
 
    if (err != OCI_SUCCESS) {
	fprintf(stderr, "OCIStmtExecute failed %ld\n", err);
	oracle_sql_error(cd, errhp);
	goto cleanup;
    }

    if (stmttype == OCI_STMT_SELECT) {
	/* this is really an error - if the query is a select than it should
	 * be passed to one of the routines which know how to handle a result set
	 */
    }

    retval = DB_SUCCESS;
  cleanup:
    if (bvnp) g_free(bvnp);
    if (bvnl) g_free(bvnl);
    if (invp) g_free(invp);
    if (inpl) g_free(inpl);
    if (dupl) g_free(dupl);
    if (hndl) g_free(hndl);
    if (stmthp) pOCIHandleFree(stmthp, OCI_HTYPE_STMT);

    return retval;
}

static long int
oracle_get_rowcount(
    const char	 *dbid)
{
    LoginData    *cd;

    if ((cd = (LoginData *)g_hash_table_lookup (login_hash_table, dbid)) == 0) {
	return DB_ERROR;
    }
    return cd->rowcount;
}

static long int
oracle_get_error_offset(
    const char	 *dbid)
{
    LoginData    *cd;

    if ((cd = (LoginData *)g_hash_table_lookup (login_hash_table, dbid)) == 0) {
	return DB_ERROR;
    }
    return cd->errchar;
}

int 
oracle_end_script(
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
    GtkCTreeNode *node;
    GtkCTreeNode *parent_node;
    GtkCTree     *tree = GTK_CTREE(user_data);
    int          i;

    if (row > 0) {
	int          id = atoi(values[0]);
	int          parent_id = atoi(values[1]);

	for (i = 0 ; i < ncols; i++)
	    if (indicators[i] == -1)
		*values[i] = '\0';

	if (parent_id == -1) {
	    node = gtk_ctree_insert_node(tree, NULL, NULL, &values[3], 4, 
					 folder_pixmap, folder_mask, 
					 folder_pixmap, folder_mask, 
					 FALSE, FALSE);
	} else {
	    parent_node = gtk_ctree_find_by_row_data (tree, NULL, (gpointer)parent_id);
	    node = gtk_ctree_insert_node(tree, parent_node, NULL, &values[3], 4, 
					 folder_pixmap, folder_mask, 
					 folder_pixmap, folder_mask, 
					 FALSE, FALSE);
	}
	gtk_ctree_node_set_row_data (tree, node, (gpointer)id);
    }
    return TRUE;
}

static GtkWidget *
oracle_explain_plan(
    const char *query,
    const char *dbid,
    GtkWidget  *widget)
{
    static char qcreatept_template[] =
	"create table PLAN_TABLE%s ("
	"	statement_id 	varchar2(30),"
	"	timestamp    	date,"
	"	remarks      	varchar2(80),"
	"	operation    	varchar2(30),"
	"	options       	varchar2(30),"
	"	object_node  	varchar2(128),"
	"	object_owner 	varchar2(30),"
	"	object_name  	varchar2(30),"
	"	object_instance numeric,"
	"	object_type     varchar2(30),"
	"	optimizer       varchar2(255),"
	"	search_columns  number,"
	"	id		numeric,"
	"	parent_id	numeric,"
	"	position	numeric,"
	"	cost		numeric,"
	"	cardinality	numeric,"
	"	bytes		numeric,"
	"	other_tag       varchar2(255),"
	"	partition_start varchar2(255),"
	"        partition_stop  varchar2(255),"
	"        partition_id    numeric,"
	"	other		long,"
	"	distribution    varchar2(30))";

    static char qexplain_template[] =
	"select id, NVL(parent_id, -1) parent_id, position, "
	"       operation|| decode(options, null,'',' '||options) \"Operation\", "
	"       decode(object_name, null, '', object_owner || '.' || object_name) \"Object\", "
	"       NVL(optimizer, '') \"Optimizer\", "
	"       decode(cardinality,null,'',"
	"                decode(sign(cardinality-1000), -1, cardinality||' ', "
	"                decode(sign(cardinality-1000000), -1, trunc(cardinality/1000)||'K', "
	"                decode(sign(cardinality-1000000000), -1, trunc(cardinality/1000000)||'M', "
	"                       trunc(cardinality/1000000000)||'G')))) \"Cardinality\", "
	"       decode(bytes,null,'',"
	"                decode(sign(bytes-1024), -1, bytes||' ', "
	"                decode(sign(bytes-1048576), -1, trunc(bytes/1024)||'K', "
	"                decode(sign(bytes-1073741824), -1, trunc(bytes/1048576)||'M', "
	"                       trunc(bytes/1073741824)||'G')))) \"Bytes\", "
	"       decode(cost,null,' ',"
	"                decode(sign(cost-10000000), -1, cost||' ', "
	"                decode(sign(cost-1000000000), -1, trunc(cost/1000000)||'M', "
	"                       trunc(cost/1000000000)||'G'))) \"Cost\", "
	"       rpad(decode(distribution, null,' ',"
	"          decode(distribution, 'PARTITION (ROWID)', 'PART (RID)', "
	"          decode(distribution, 'PARTITION (KEY)', 'PART (KEY)', "
	"          decode(distribution, 'ROUND-ROBIN', 'RND-ROBIN', "
	"          decode(distribution, 'BROADCAST', 'BROADCAST', distribution))))), 12, ' ') \"Distribution\", "
	"       lpad(decode(partition_start, 'ROW LOCATION', 'ROWID', "
	"            decode(partition_start, 'KEY', 'KEY', decode(partition_start, "
	"            'KEY(INLIST)', 'KEY(I)', decode(substr(partition_start, 1, 6), "
	"            'NUMBER', substr(substr(partition_start, 8, 10), 1, "
	"            length(substr(partition_start, 8, 10))-1), "
	"            decode(partition_start,null,' ',partition_start)))))||' ', 7, ' ') \"Partition Start\", "
	"       lpad(decode(partition_stop, 'ROW LOCATION', 'ROW L', "
	"          decode(partition_stop, 'KEY', 'KEY', decode(partition_stop, "
	"          'KEY(INLIST)', 'KEY(I)', decode(substr(partition_stop, 1, 6), "
	"          'NUMBER', substr(substr(partition_stop, 8, 10), 1, "
	"          length(substr(partition_stop, 8, 10))-1), "
	"          decode(partition_stop,null,' ',partition_stop)))))||' ', 7, ' ') \"Partition Stop\" "
	"from  PLAN_TABLE%s "
	"  ORDER BY NVL(parent_id, -1), position";

    static char qgenplan_template[] =
	"EXPLAIN PLAN INTO plan_table%s FOR %s";

    static char qclearup_template[] =
	"DROP TABLE plan_table%s";

    char	 *qcreatept  = 0;
    char	 *qgenplan   = 0;
    char	 *qexplain   = 0;
    char	 *qclearup   = 0;

    char         plan_id[128];
    GtkCTree     *tree = 0;
    static char  *titles[] = { 
	"Operation", "Object", "Optimizer", "Cardinality",
	"Bytes", "Cost", "Distribution", "Partition Start", "Partition Stop"};

    assert(oci_initialised);

    if (!folder_pixmap)
	folder_pixmap = gdk_pixmap_create_from_xpm_d(widget->window, &folder_mask, NULL, folder);

    /* We need a better unique tag but this will do for now */
    sprintf(plan_id, "%ld", (long)time(0));

    qcreatept = g_malloc(sizeof(qcreatept_template) + strlen(plan_id) + 1);
    sprintf(qcreatept, qcreatept_template, plan_id);
    oracle_execute_sql(dbid, qcreatept);
    g_free(qcreatept);
    qcreatept = 0;

    qgenplan = g_malloc(sizeof(qgenplan_template) + strlen(plan_id) + strlen(query) + 1);
    sprintf(qgenplan, qgenplan_template, plan_id, query);
    oracle_execute_sql(dbid, qgenplan);
    g_free(qgenplan);
    qgenplan = 0;

    tree = GTK_CTREE(gtk_ctree_new_with_titles (9, 0, titles));

    qexplain = g_malloc(sizeof(qexplain_template) + 2*strlen(plan_id) + 1);
    sprintf(qexplain, qexplain_template, plan_id, plan_id);
    oracle_do_query(qexplain, dbid, 0, plan_rowcallback, tree);
    g_free(qexplain);
    qexplain = 0;

    gtk_ctree_expand_recursive(tree, 0);

    if (tree) {
	list_set_opt_col_width(tree, 0);
	list_set_opt_col_width(tree, 1);
	list_set_opt_col_width(tree, 2);
    }

    qclearup = g_malloc(sizeof(qclearup_template) + strlen(plan_id) + 1);
    sprintf(qclearup, qclearup_template, plan_id);
    oracle_execute_sql(dbid, qclearup);
    g_free(qclearup);
    qclearup = 0;

    return GTK_WIDGET(tree);
  clearup:

    /* oracle_sql_error(); */
    if (qcreatept) g_free(qcreatept);
    if (qgenplan) g_free(qgenplan);
    if (qexplain) g_free(qexplain);
    if (qclearup) g_free(qclearup);
    if (tree)
	gtk_widget_destroy(GTK_WIDGET(tree));
    return 0;
}

static int
oracle_q_interruptable(void)
{
  return TRUE;
}

static void
oracle_sql_error(
    LoginData	 *cd,
    OCIError	 *errhp)
{
    text	 errbuf[512];
    
    if (errhp) {
	sb4    *errcodep;
	sb4    errcode;
	
	errcodep = &errcode;
	
	(void) pOCIErrorGet(errhp, (ub4) 1, (text *) NULL, errcodep,
			   errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);

	show_warning_msg("%s", errbuf);

	/* (void) fprintf(stderr, "Error - %.*s\n", 512, errbuf); */
    }
}

DbInterface oracleIf =  {
    "Oracle",
    oracle_initialise,
    oracle_finalise,
    cstring_getdbid,
    oracle_init_login,
    oracle_get_cstring,
    oracle_login,
    oracle_close_conn,
    oracle_start_script,
    oracle_fetch_statement,
    oracle_do_query,
    oracle_execute_sql,
    oracle_get_rowcount,
    oracle_get_error_offset,
    oracle_end_script,
    oracle_explain_plan,
    oracle_q_interruptable};
