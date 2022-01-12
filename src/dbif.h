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

/* definitions for the database back end */

#include <gtk/gtk.h>
typedef int (*RowcallbackFunc)(int row, int ncols, 
			       char **colnames, 
			       const short *indicators, 
			       char **values, 
			       unsigned long *values_l, 
			       short *types, 
			       gpointer user_data);

typedef struct _dbif {
    /* Name of this database back end  NB the login dialog window duplicates
     * this name for the child of the "auth parameters notebook" which collects
     * the login parameters for connections to databases of this type - if the
     * widget name and the backend name don't match things will "go wrong"
     */
    char *db_name;

    /* Initialise db subsystem */
    int (*initialise)(void);

    /* Cleanly shutdown */
    int (*finalise)(void);

    /* return database id string from connect string */
    char *(*getdbid)(const char *);

    /* Initialise login dialog with supplied cstring or driver chosen default */
    void (*init_login)(GtkWidget *dialog, const char *cstring);

    /* return connection string from login dialog */
    void (*get_cstring)(GtkWidget *dialog);

    /* Open a new connection using credentials in cstring and and associate
       dbid as symbolic identifier */
    int (*login)(const char *cstring, const char *dbid);

    /* Close the connection identified by dbid */
    int (*close)(const char *dbid);

    /* Start parsing a script */
    int (*start_script)(const char *sql_script, const char *dbid);

    /* return the next statement from the current script */
    char *(*fetch_statement)(const char *dbid);

    /* Do a db query returning a rowset */
    int (*do_query)( const char	*sql_query, const char *dbid, int max_rows,
		     RowcallbackFunc rowcallback, gpointer user_data);

    /* Execute arbitary sql statement */
    int (*execute_sql)(const char *dbid, const char *sql_stmt, ...);

    /* Get row count for last select/insert/delete */
    long int (*get_rowcount)(const char	 *dbid);

    /* Get character offset of first error in last statement */
    long int (*get_error_offset)(const char *dbid);

    /* End parsing a script */
    int (*end_script)(const char *dbid);

    /* get query plan for given query */
    GtkWidget * (*explain_plan)(const char *query, const char *dbid, GtkWidget  *widget);

    /* Returns true if queries can be aborted before all data has been read */
    int (*query_interruptable)(void);

} DbInterface ;

void db_initialise(void);
void db_finalise(void);
GList *db_list(void);
const DbInterface *db_find(const char *name);

/* Return codes for database routines */
#define DB_SUCCESS 0
#define DB_ERROR -1

/* Type codes for columns */
#define COLTYPE_INT	  1	/* integer type */
#define COLTYPE_UINT	  2	/* unsigned integer type */
#define COLTYPE_FLOAT	  3     /* floating point */
#define COLTYPE_DECIMAL	  4	/* fixed point */
#define COLTYPE_CHAR	  5	/* Fixed length char (usu space padded) */
#define COLTYPE_VARCHAR   6     /* Variable length char */
#define COLTYPE_NCHAR     7     /* wide (native) Fixed length char*/
#define COLTYPE_NVARCHAR  8     /* wide (native) variable length char*/
#define COLTYPE_BLOB      9     /* Binary/raw */
#define COLTYPE_TIMESTAMP 10    /* timestamp (usu opaque binary) */
#define COLTYPE_DATE      11    /* date */
#define COLTYPE_DATETIME  12    /* date & time */
#define COLTYPE_TIME      13    /* time */
#define COLTYPE_ROWID     14    /* unique rowid */
