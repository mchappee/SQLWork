/* SQLWork - the Oracle Query tool
 *
 * Copyright � Matthew Chappee <matthew@orasoft.org>
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
/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "sqlworkwin.h"
#include "sqlworksig.h"
#include "support.h"
#include "miscfunc.h"
#include "common.h"
#include "prefs.h"

static int
connectdb(
    GtkWidget	       *main_window,
    const DbInterface  *dbif,
    const char	       *cstring)
{
    GtkWidget	       *main_notebook;
    GtkWidget	       *login_combo;
    GtkWidget	       *work_area;
    GtkWidget	       *sql_query;
    GList	       *dbid_list;
    GSList	       *mainw_accelgps;

    if (cstring) {
	char *dbid = dbif->getdbid(cstring);
	
	if (dbif->login(cstring, dbid) == DB_ERROR) {
	    show_warning_msg("Login failed for %s", dbid);
	    return 0;
	}

	login_combo = lookup_widget(main_window, "current_login_combo");

	dbid_list = gtk_object_get_data (GTK_OBJECT (main_window), "dbid_list");
	dbid_list = g_list_prepend(dbid_list, dbid);
	gtk_object_set_data (GTK_OBJECT (main_window), "dbid_list", dbid_list);
	work_area = create_work_pane();

	sql_query = gtk_object_get_data (GTK_OBJECT (work_area), "sql_query");
	mainw_accelgps = gtk_accel_groups_from_object(GTK_OBJECT(main_window));
	
	gtk_widget_add_accelerator (sql_query, "activate", mainw_accelgps->data,
				    GDK_F6, 0,
				    GTK_ACCEL_VISIBLE);

	main_notebook = lookup_widget(main_window, "notebook1");
	gtk_notebook_append_page(GTK_NOTEBOOK(main_notebook), work_area,
				 gtk_label_new(dbid));

	gtk_object_set_data_full (GTK_OBJECT (work_area), "dbid", g_strdup(dbid), g_free);
	gtk_object_set_data_full (GTK_OBJECT (work_area), "cstring", g_strdup(cstring), g_free);
	gtk_object_set_data (GTK_OBJECT (work_area), "dbif", (gpointer)dbif);
	gtk_object_set_data (GTK_OBJECT (work_area), "connection_num", (gpointer)0);
	gtk_combo_set_popdown_strings(GTK_COMBO(login_combo), dbid_list);
    }
    return 1;
}

/* Insert query specified on command line */
static void
insert_query(
    char	 *query,
    char	 *dbid,
    GtkWidget	 *main_notebook,
    int          execute)
{
    int		 pageno;
    GtkWidget	 *page;
    char	 *page_dbid;
    GtkText	 *sql_query;

    for (pageno = 0;
	 (page = gtk_notebook_get_nth_page (GTK_NOTEBOOK(main_notebook), pageno)) &&
	     (page_dbid = gtk_object_get_data(GTK_OBJECT(page), "dbid")) &&
	     strcmp(page_dbid, dbid) != 0;
	 pageno++) ;
    
    assert(page);
    gtk_notebook_set_page(GTK_NOTEBOOK(main_notebook), pageno);
    sql_query = GTK_TEXT(gtk_object_get_data(GTK_OBJECT(page), "sql_query"));
    if (*query == '@') {
	FILE	 *inf	  = fopen(query+1, "r");
	if (inf) {
	    struct stat	     st;
	    char	     *filebuf;
	    
	    if (fstat(fileno(inf), &st) < 0) {
		show_warning_msg("Can't stat open file %s, *highly* wierd!", 
				 query+1);
	    }
	    if (st.st_size > 0) {
		filebuf = g_malloc(st.st_size);
		fread(filebuf, 1, st.st_size, inf);
		gtk_text_insert(sql_query, NULL, NULL, NULL, filebuf, st.st_size);
		if (filebuf[st.st_size-1] != '\n')
		    gtk_text_insert(sql_query, NULL, NULL, NULL, "\n", 1);
		    
		g_free(filebuf);
	    }
	} else {
	    file_open_error(query+1, errno, "read");
	}

    } else {
	size_t l = strlen(query);
	gtk_text_insert(sql_query, NULL, NULL, NULL, query, l);
	if (query[l] != '\n')
	    gtk_text_insert(sql_query, NULL, NULL, NULL, "\n", 1);
    }

    if (execute) {
	gtk_widget_show(lookup_widget(main_notebook, "main_window"));
	on_exec_tbb_clicked(0, main_notebook);
    }
}

static void
usage(
    char *progname)
{
    fprintf(stderr, "%s [<connection specifier>] [-q <query>] [-x <query>]\n",
	    progname);
    db_finalise();
    exit(1);
}

int
main (
    int		 argc, 
    char	 *argv[])
{
    GtkWidget	 *main_window;
    GtkWidget	 *main_notebook;
    DbInterface	 *dbif;
    GList	 *dbid_list;
    char	 *connections;
    char	 *cstring  = 0;
    char	 *last_dbid;
    char	 *icondir;
    int		 arg;

#ifdef ENABLE_NLS
    bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
    textdomain (PACKAGE);
#endif

    gtk_set_locale ();
    gtk_init (&argc, &argv);

    add_pixmap_directory (PACKAGE_DATA_DIR "/pixmaps");
    add_pixmap_directory (PACKAGE_SOURCE_DIR "/xpm");

    icondir = prefs_get_char_var(PREF_ICONPATH);
    if (icondir && *icondir)
	add_pixmap_directory (icondir);
	
    db_initialise();
    
    if (g_list_length(db_list()) == 0) {
      fatal_error("No database drivers initialised");
    }

    main_window = create_main_window();
    main_notebook = lookup_widget(main_window, "notebook1");

    /* Remove the dummy page which is inserted into the main notebook
       by glade */
    gtk_notebook_remove_page(GTK_NOTEBOOK(main_notebook), 0);

    /* Process saved connections */
    connections = prefs_get_char_var(PREF_CONNECT);
    if (connections && *connections) {
	char *s = connections;

	while (s && *s) {
	    char	       *db	 = conn_getdbtype(s);
	    const DbInterface  *dbif	 = db_find(db);
	    cstring                      = conn_getcstring(s);

	    assert(db && cstring);

	    if (dbif) {
		connectdb(main_window, dbif, cstring);
	    } else {
		show_warning_msg("Cannot log in to %s as %s databases are not supported",
				 cstring, db);
	    }
	    g_free(db);
	    g_free(cstring);
	    
	    /* Skip over this connection specifier */
	    s = strstr(s, "(");
	    if (s) s = strstr(s, ")");
	    if (s) s = strstr(s, ":");
	    if (s) ++s;
	}
    }

    dbid_list = gtk_object_get_data (GTK_OBJECT (main_window), "dbid_list");
    last_dbid = dbid_list ? dbid_list->data : 0;

    /* Process the command line
     *
     * the command line syntax is
     * sqlwork [<connection specifier>] [-q <query>] [-x <query>]
     */
    for(arg = 1; arg < argc; arg++) {
	if (argv[arg][0] == '-') {

	    if (strcmp(argv[arg], "-q") == 0 ||
		strcmp(argv[arg], "-x") == 0) {
		if (++arg >= argc) {
		    usage(argv[0]);
		}
		insert_query(argv[arg], last_dbid, main_notebook, argv[arg-1][1] == 'x');
	    } else {
		usage(argv[0]);
	    }
	} else {
	    char	      *s	= argv[arg];
	    char	      *db	= conn_getdbtype(s);
	    const DbInterface *dbif	= db ? db_find(db) : 0;
	    cstring			= conn_getcstring(s);
	    
	    if (!db || !cstring) {
		show_warning_msg("%s is not a valid connection string",
				 s);
	    } else if (!dbif) {
		show_warning_msg("Cannot log in to %s as %s is not a supported database",
				 cstring, db);
	    } else {
		char	     *pass    = cstring_getpassword(cstring);

		if (!pass) {
		    char     *dbid = dbif->getdbid(cstring);
		    GList    *dbl = dbid_list;
		    
		    while (dbl && strcmp(dbl->data, dbid) != 0) {
			dbl = g_list_next(dbl);
		    }
		    if (dbl) {
			last_dbid = dbl->data;
			continue;
		    }
		}
		if (connectdb(main_window, dbif, cstring)) {
		    dbid_list = gtk_object_get_data (GTK_OBJECT (main_window), "dbid_list");
		    last_dbid = dbid_list->data;
		}
	    }
	    g_free(db);
	    g_free(cstring);
	}
    }

    if (!dbid_list) {
	cstring = 0;
	do_login_dialog ("Login", 0, &dbif, &cstring);
	connectdb(main_window, dbif, cstring);
    }

    gtk_object_set_data (GTK_OBJECT (main_window), "dbid_list", dbid_list);

    gtk_widget_show(main_window);

    gtk_main ();
    prefs_save_values();
    db_finalise();
    return 0;
}

