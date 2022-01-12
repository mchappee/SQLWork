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
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#endif

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "sqlworksig.h"
#include "sqlworkwin.h"
#include "bookmarksig.h"
#include "support.h"
#include "miscfunc.h"
#include "common.h"
#include "export.h"
#include "prefs.h"

#define list_set_opt_col_width(clist, col)     \
    gtk_clist_set_column_width (GTK_CLIST(clist), (col), \
				gtk_clist_optimal_column_width (GTK_CLIST(clist), (col)))

typedef struct {
    GtkText	    *text_results;
    GtkCList	    *grid_results;
    GdkFont	    *text_font;
    GdkColor	    color; 
    int		    text_width;
    int		    *col_widths;
    struct timeval  qstart;
    struct timeval  qdataready;
} ExecQueryRowCBData;

typedef struct {
    char            *stmt;
    gboolean        errored;
    GtkWidget       *grid;
    int             nb_page;
} ExecQueryStmtRecord;

static int abort_query;

static GtkWidget *
find_current_work_area(
    GtkWidget	 *widget)
{
    GtkWidget	 *main_notebook	 = lookup_widget(widget, "notebook1");
    GtkWidget	 *work_area =
	gtk_notebook_get_nth_page (GTK_NOTEBOOK(main_notebook), 
				   gtk_notebook_get_current_page(GTK_NOTEBOOK(main_notebook)));
    assert(work_area != 0);
    return work_area;
}

static GtkText *
find_query_text_widget(
    GtkWidget *widget)
{
    GtkWidget  *work_area = find_current_work_area(widget);
    GtkText    *sql_query = GTK_TEXT(gtk_object_get_data(GTK_OBJECT(work_area), 
							 "sql_query"));
    assert(sql_query != 0);
    return sql_query;
}

/* Used as a weakref callback when we want to destroy a popup
   associated with some widget should that widget be destroyed */
static void
weakref_destroy_popup(
    gpointer user_data)
{
    gtk_widget_destroy(GTK_WIDGET(user_data));
}

gboolean
on_main_window_delete_event(
    GtkWidget       *widget,
    GdkEvent        *event,
    gpointer         user_data)
{
    gtk_main_quit();
    return FALSE;
}


static void
save_or_load_query(
    GtkWidget  *widget,
    char       *action)
{
    GtkWidget  *fs	  = create_save_query_fileselector();
    GtkText    *query	  = find_query_text_widget(widget);
    char       *last_save = gtk_object_get_data(GTK_OBJECT(query), "last-saved-as");

    gtk_object_set_data(GTK_OBJECT(fs), "action", action);
    gtk_object_set_data(GTK_OBJECT(fs), "query", query);
    if (last_save)
	gtk_file_selection_set_filename(GTK_FILE_SELECTION(fs), last_save);
    gtk_widget_show(fs);
}

void
on_menuopen_activate(
    GtkMenuItem     *menuitem,
    gpointer         user_data)
{
    save_or_load_query(GTK_WIDGET(menuitem), "open");
}


void
on_menusave_activate(
    GtkMenuItem     *menuitem,
    gpointer         user_data)
{
    GtkText *query = find_query_text_widget(GTK_WIDGET(user_data));
    char    *pathname = gtk_object_get_data(GTK_OBJECT(query), "last-saved-as");

    if (pathname) {
	FILE *f = fopen(pathname, "w");
	if (f) {
	    int l = gtk_text_get_length(query);
	    char *text = gtk_editable_get_chars(GTK_EDITABLE(query), 0, l);
	    fwrite(text, 1, l, f);
	    fclose(f);
	    g_free(text);
	} else {
	    file_open_error(pathname, errno, "write");
	}
    } else {
	save_or_load_query(GTK_WIDGET(user_data), "save");
    }
}

void
on_menusaveas_activate(
    GtkMenuItem     *menuitem,
    gpointer         user_data)
{
    save_or_load_query(GTK_WIDGET(menuitem), "saveas");
}

/* Add a new database connection with its own work area */
void
on_add_connection_activate(
    GtkMenuItem	 *menuitem,
    gpointer	 user_data)
{
    GtkWidget	 *main_window  = GTK_WIDGET(user_data);
    char	 *cstring = 0;
    DbInterface	 *dbif;
    GtkWidget	 *sql_query;
    GSList        *mainw_accelgps;
    
    do_login_dialog("Login", 0, &dbif, &cstring);
    if	       (cstring) {
	char	   *dbid	  = dbif->getdbid(cstring);
	GtkWidget  *login_combo	  = lookup_widget(main_window, "current_login_combo");
	GtkWidget  *main_notebook = lookup_widget(main_window, "notebook1");
	GList	   *dbid_list	  = gtk_object_get_data (GTK_OBJECT (main_window), "dbid_list");
	GList	   *l		  = dbid_list;
	int	   dbidlen	  = strlen(dbid);
	int        connect_num    = -1;
	GtkWidget  *work_area;
	GtkWidget  *page;
	int        pageno;
	char       *list_id;

	/* Look through current list of connections and see whether
	   we already have any for this dbid. */
	while(l) {
	    if (strncmp((char *)l->data, dbid, dbidlen) == 0) {
		int n = 0;
		if (((char *)l->data)[dbidlen] != '\0') {
		    sscanf(((char *)l->data)+dbidlen+1, "%d", &n);
		}
		if ( connect_num < n)
		    connect_num = n;
	    }
	    l = g_list_next(l);
	}
	connect_num++;
	if (connect_num == 0) {
	    list_id = g_strdup(dbid);
	    if (dbif->login(cstring, list_id) == DB_ERROR) {
		show_warning_msg("Login failed");
		g_free(cstring);
		g_free(list_id);
		return;
	    }
	} else {
	    list_id = g_malloc(dbidlen+32);
	    sprintf(list_id, "%s[%d]", dbid, connect_num);
	}
	work_area = create_work_pane();

	sql_query = gtk_object_get_data (GTK_OBJECT (work_area), "sql_query");
	mainw_accelgps = gtk_accel_groups_from_object(GTK_OBJECT(main_window));
	
	gtk_widget_add_accelerator (sql_query, "activate", mainw_accelgps->data,
				    GDK_F6, 0,
				    GTK_ACCEL_VISIBLE);

	gtk_object_set_data_full(GTK_OBJECT (work_area), "dbid", g_strdup(dbid), g_free);
	gtk_object_set_data_full(GTK_OBJECT (work_area), "cstring", g_strdup(cstring), g_free);
	gtk_object_set_data (GTK_OBJECT (work_area), "dbif", dbif);
	gtk_object_set_data (GTK_OBJECT (work_area), "connection_num", 
			     (gpointer)connect_num);

	gtk_notebook_append_page(GTK_NOTEBOOK(main_notebook), work_area,
				 gtk_label_new(list_id));

	gtk_container_add(GTK_CONTAINER(main_notebook), work_area);
	/* find the page we just added */
	for (pageno = 0;
	     (page = gtk_notebook_get_nth_page (GTK_NOTEBOOK(main_notebook), pageno))
		 && page != work_area;
	     pageno++) ;
	assert(page);
	gtk_notebook_set_page(GTK_NOTEBOOK(main_notebook), pageno);

	dbid_list = g_list_append(dbid_list, list_id);
	gtk_combo_set_popdown_strings(GTK_COMBO(login_combo), dbid_list);
	gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(login_combo)->entry), list_id);
    }
    g_free(cstring);
}

void
on_save_connecion_activate(
    GtkMenuItem     *menuitem,
    gpointer         user_data)
{
    GtkWidget  	 *work_area  = find_current_work_area(GTK_WIDGET(menuitem));
    char	 *dbid	     = gtk_object_get_data (GTK_OBJECT (work_area), "dbid");
    char	 *cstring    = gtk_object_get_data (GTK_OBJECT (work_area), "cstring");
    DbInterface	 *dbif       = gtk_object_get_data (GTK_OBJECT (work_area), "dbif");
    char         *con        = prefs_get_char_var(PREF_CONNECT);
    GString      *newcon     = g_string_new("");
    char         *s          = con;

    /* copy all current connections to new prefs setting, except
       anything which has the same dbid as the current connection */
    while (s && *s) {
	char *condb = conn_getdbtype(s);
	char *concstring = conn_getcstring(s);
	char *condbid = cstring_getdbid(concstring);

	if (strcmp(dbid, condbid) != 0) {
	    g_string_sprintf(newcon, "%s%s(%s)", 
			     newcon->len > 0 ? ":" : "",
			     condb, concstring);
	}

	g_free(condb);
	g_free(concstring);
	g_free(condbid);
	
	/* Skip over this connection specifier */
	s = strstr(s, "(");
	if (s) s = strstr(s, ")");
	if (s) s = strstr(s, ":");
	if (s) ++s;
    }
    g_string_sprintf(newcon, "%s%s(%s)", 
		     newcon->len > 0 ? ":" : "",
		     dbif->db_name, cstring);

    prefs_set_char_var(PREF_CONNECT, newcon->str);
    g_string_free(newcon, 1);
	
}

void
on_close_connection_activate(
    GtkMenuItem	 *menuitem,
    gpointer	 user_data)
{
    GtkWidget	 *main_window	= GTK_WIDGET(user_data);
    GtkNotebook	 *main_notebook	= GTK_NOTEBOOK(lookup_widget(main_window, "notebook1"));
    GtkWidget	 *work_area	= find_current_work_area(GTK_WIDGET(menuitem));
    GtkWidget  	 *login_combo   = lookup_widget(main_window, "current_login_combo");
    char	 *dbid		= gtk_object_get_data (GTK_OBJECT (work_area), "dbid");
    DbInterface	 *dbif		= gtk_object_get_data (GTK_OBJECT (work_area), "dbif");
    GList	 *dbid_list	= gtk_object_get_data (GTK_OBJECT (main_window), "dbid_list");
    int		 connect_num	= (int) gtk_object_get_data(GTK_OBJECT(work_area), "connection_num");
    GList	 *l		= dbid_list;
    int		 dbidlen	= strlen(dbid);
    int		 nconn		= 0;

    if (!gtk_container_children(GTK_CONTAINER(main_notebook))->next) {
	if (confirm("Removing last connection will exit SQLWork")) {
	    gtk_main_quit();
	} 
	return;
    }

    while(l) {
	if (strncmp((char *)l->data, dbid, dbidlen) == 0) {
	    nconn++;
	}
	l = g_list_next(l);
    }
    // Holla if we didn't find *any* work areas using the current dbid
    assert(nconn > 0);
    if (nconn == 1) {
	// If only one work work area connected then close
	dbif->close(dbid);
    }

    if (connect_num == 0) {
	l = g_list_find_custom(dbid_list, dbid, (GCompareFunc)strcmp);
    } else {
	char *list_id = alloca(dbidlen+32);
	sprintf(list_id, "%s[%d]", dbid, connect_num);
	l = g_list_find_custom(dbid_list, list_id, (GCompareFunc)strcmp);
    }
    assert(l);
    g_free(l->data);
    dbid_list = g_list_remove_link(dbid_list, l);
    g_list_free(l);
    gtk_object_set_data (GTK_OBJECT (main_window), "dbid_list", dbid_list);

    gtk_combo_set_popdown_strings(GTK_COMBO(login_combo), dbid_list);
    gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(login_combo)->entry), dbid_list->data);

    gtk_notebook_remove_page(main_notebook,
			     gtk_notebook_page_num(main_notebook,
						   work_area));
}

void
on_menuexit_activate(
    GtkMenuItem     *menuitem,
    gpointer         user_data)
{
    gtk_main_quit();
}

void
on_menucut_activate(
    GtkMenuItem     *menuitem,
    gpointer         user_data)
{
    gtk_editable_cut_clipboard(GTK_EDITABLE(find_query_text_widget(GTK_WIDGET(menuitem))));
}

void
on_menucopy_activate(
    GtkMenuItem     *menuitem,
    gpointer         user_data)
{
    gtk_editable_copy_clipboard(GTK_EDITABLE(find_query_text_widget(GTK_WIDGET(menuitem))));
}

void
on_menupaste_activate(
    GtkMenuItem     *menuitem,
    gpointer         user_data)
{
    gtk_editable_paste_clipboard(GTK_EDITABLE(find_query_text_widget(GTK_WIDGET(menuitem))));
}

void
on_menuselectall_activate(
    GtkMenuItem     *menuitem,
    gpointer         user_data)
{
    GtkEditable *query_text = GTK_EDITABLE(find_query_text_widget(GTK_WIDGET(menuitem)));

    gtk_editable_select_region(query_text, 0, gtk_text_get_length(GTK_TEXT(query_text)));
}

void
on_menuclear_activate(
    GtkMenuItem	 *menuitem,
    gpointer	 user_data)
{
    GtkText	 *sql_query  = find_query_text_widget(GTK_WIDGET(menuitem));

    gtk_text_set_point(sql_query, 0);
    gtk_text_forward_delete(sql_query, gtk_text_get_length(sql_query));
}

void
on_menuprefs_activate(
    GtkMenuItem	 *menuitem,
    gpointer	 user_data)
{
    GtkWidget	 *prefs_win  = create_prefs_window ();
    GtkWidget	 *toolbar    = lookup_widget(GTK_WIDGET(menuitem), "toolbar1");
    GtkWidget	 *w;
    char         *connections;
    char         *val;

    gtk_object_set_data(GTK_OBJECT(prefs_win), "toolbar", toolbar);

    w = lookup_widget(prefs_win, "prefs_textv_font");
    gtk_entry_set_text(GTK_ENTRY(w), prefs_get_char_var(PREF_TEXTFNT));

    w = lookup_widget(prefs_win, "prefs_save");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w),
				 prefs_get_int_var(PREF_SAVE));

    w = lookup_widget(prefs_win, "prefs_textv_upd");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w),
				 prefs_get_int_var(PREF_TEXTUPD));

    w = lookup_widget(prefs_win, "prefs_gridv_upd");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w),
				 prefs_get_int_var(PREF_GRIDUPD));

    w = lookup_widget(prefs_win, "prefs_gridv_autos");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w),
				 prefs_get_int_var(PREF_GRIDAUT));

    w = lookup_widget(prefs_win, "prefs_max_rows");
    gtk_entry_set_text(GTK_ENTRY(w), prefs_get_char_var(PREF_MAXROWS));

    w = lookup_widget(prefs_win, "prefs_export_sep");
    gtk_entry_set_text(GTK_ENTRY(w), prefs_get_char_var(PREF_CHARSEP));

    w = lookup_widget(prefs_win, "prefs_text_colwid");
    gtk_entry_set_text(GTK_ENTRY(w), prefs_get_char_var(PREF_TEXTCOLW));

    w = lookup_widget(prefs_win, "prefs_q_commit");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w),
				 prefs_get_int_var(PREF_AUTOCMT));

    w = lookup_widget(prefs_win, "prefs_icon_path");
    val = prefs_get_char_var(PREF_ICONPATH);
    if (val)
	gtk_entry_set_text(GTK_ENTRY(w), val);

    w = lookup_widget(prefs_win, "prefs_bm_path");
    val = prefs_get_char_var(PREF_BMKPATH);
    if (val)
	gtk_entry_set_text(GTK_ENTRY(w), val);

    w = lookup_widget(prefs_win, "prefs_psize_ent");
    gtk_entry_set_text(GTK_ENTRY(w), prefs_get_char_var(PREF_PSIZE));

    w = lookup_widget(prefs_win, "prefs_fsize_ent");
    gtk_entry_set_text(GTK_ENTRY(w), prefs_get_char_var(PREF_FSIZE));

    w = lookup_widget(prefs_win, "prefs_font_ent");
    gtk_entry_set_text(GTK_ENTRY(w), prefs_get_char_var(PREF_FONT));

    switch (prefs_get_int_var(PREF_TBSTYLE)) {
      case -1:
	  w = lookup_widget(prefs_win, "prefs_tb_hide");
    	  break;
      case GTK_TOOLBAR_ICONS:
	  w = lookup_widget(prefs_win, "prefs_tb_icons");
    	  break;
      case GTK_TOOLBAR_TEXT:
	  w = lookup_widget(prefs_win, "prefs_tb_text");
    	  break;
      case GTK_TOOLBAR_BOTH:
      default:
	  w = lookup_widget(prefs_win, "prefs_tb_both");
    	  break;
    }
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(w), TRUE);

    w = lookup_widget(prefs_win, "prefs_saved_conn_list");
    connections = prefs_get_char_var(PREF_CONNECT);
    if (connections) {
	char *s = connections;

	while (s && *s) {
	    char *row[2];
	    char *db = conn_getdbtype(s);
	    char *cstring = conn_getcstring(s);

	    assert(db && cstring);

	    row[0] = db;
	    row[1] = cstring;
	    gtk_clist_append(GTK_CLIST(w), row);
	    g_free(db);
	    g_free(cstring);
	    
	    /* Skip over this connection specifier */
	    s = strstr(s, "(");
	    if (s) s = strstr(s, ")");
	    if (s) s = strstr(s, ":");
	    if (s) ++s;
	}
	
    }
    gtk_widget_show(prefs_win);
}

void
on_prefs_sel_font_clicked(
    GtkButton  *button,
    gpointer   user_data)
{
    GtkWidget  *fs	     = create_fontselectiondialog();
    GtkWidget  *w	     = lookup_widget(GTK_WIDGET(user_data), "prefs_textv_font");
    char       *current_font = gtk_entry_get_text(GTK_ENTRY(w));


    gtk_object_set_data(GTK_OBJECT(fs), "prefs_dialog", user_data);
    gtk_object_set_data_full(GTK_OBJECT(fs), "prev_font",
			     g_strdup(current_font), g_free);
    gtk_object_weakref(GTK_OBJECT(user_data), weakref_destroy_popup, fs);
    gtk_font_selection_dialog_set_font_name(GTK_FONT_SELECTION_DIALOG(fs),
					    current_font);
    gtk_widget_show(fs);
}

void
on_prefs_ok_clicked(
    GtkButton	 *button,
    gpointer	 user_data)
{
    on_prefs_apply_clicked(button, user_data);
    gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_prefs_apply_clicked(
    GtkButton	     *button,
    gpointer	     user_data)
{
    GtkWidget	     *w;
    GtkWidget	     *toolbar = gtk_object_get_data(GTK_OBJECT(user_data), "toolbar");
    GtkToolbarStyle  tbstyle = GTK_TOOLBAR_BOTH;
    char             *val;

    w = lookup_widget(GTK_WIDGET(button), "prefs_textv_font");
    prefs_set_char_var(PREF_TEXTFNT, gtk_entry_get_text(GTK_ENTRY(w)));

    w = lookup_widget(GTK_WIDGET(button), "prefs_save");
    prefs_set_int_var(PREF_SAVE,
		      gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w)));

    w = lookup_widget(GTK_WIDGET(button), "prefs_textv_upd");
    prefs_set_int_var(PREF_TEXTUPD, 
		      gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w)));

    w = lookup_widget(GTK_WIDGET(button), "prefs_gridv_upd");
    prefs_set_int_var(PREF_GRIDUPD, 
		      gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w)));

    w = lookup_widget(GTK_WIDGET(button), "prefs_gridv_autos");
    prefs_set_int_var(PREF_GRIDAUT, 
		      gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w)));

    w = lookup_widget(GTK_WIDGET(button), "prefs_max_rows");
    prefs_set_char_var(PREF_MAXROWS, gtk_entry_get_text(GTK_ENTRY(w)));

    w = lookup_widget(GTK_WIDGET(button), "prefs_export_sep");
    prefs_set_char_var(PREF_CHARSEP, gtk_entry_get_text(GTK_ENTRY(w)));

    w = lookup_widget(GTK_WIDGET(button), "prefs_text_colwid");
    prefs_set_char_var(PREF_TEXTCOLW, gtk_entry_get_text(GTK_ENTRY(w)));

    w = lookup_widget(GTK_WIDGET(button), "prefs_q_commit");
    prefs_set_int_var(PREF_AUTOCMT, 
		      gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w)));

    w = lookup_widget(GTK_WIDGET(button), "prefs_psize_ent");
    prefs_set_char_var(PREF_PSIZE, gtk_entry_get_text(GTK_ENTRY(w)));

    w = lookup_widget(GTK_WIDGET(button), "prefs_fsize_ent");
    prefs_set_char_var(PREF_FSIZE, gtk_entry_get_text(GTK_ENTRY(w)));

    w = lookup_widget(GTK_WIDGET(button), "prefs_font_ent");
    prefs_set_char_var(PREF_FONT, gtk_entry_get_text(GTK_ENTRY(w)));

    if ((w = lookup_widget(GTK_WIDGET(button), "prefs_tb_hide")) != 0 &&
	gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w))) {
	tbstyle = (GtkToolbarStyle)-1;
    } else if ((w = lookup_widget(GTK_WIDGET(button), "prefs_tb_icons")) != 0 &&
	       gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w))) {
	tbstyle = GTK_TOOLBAR_ICONS;
    } else if ((w = lookup_widget(GTK_WIDGET(button), "prefs_tb_text")) != 0 &&
	       gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w))) {
	tbstyle = GTK_TOOLBAR_TEXT;
    } else if ((w = lookup_widget(GTK_WIDGET(button), "prefs_tb_both")) != 0 &&
	       gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w))) {
	tbstyle = GTK_TOOLBAR_BOTH;
    }
    if (tbstyle == (GtkToolbarStyle)-1) {
	gtk_widget_hide(GTK_WIDGET(toolbar));
    } else {
	gtk_widget_show(GTK_WIDGET(toolbar));
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), tbstyle);
    }
    prefs_set_int_var(PREF_TBSTYLE, tbstyle);


    /* For these values the default is "not set" so we only set the value if it
       is non-empty */
    w = lookup_widget(GTK_WIDGET(button), "prefs_saved_conn_list");
    if (GTK_CLIST(w)->rows) {
	GString	 *connections  = g_string_new("");
	gchar	 *db;
	gchar	 *cstring;
	int	 row;

	for (row = 0 ; row < GTK_CLIST(w)->rows; row++) {
	    gtk_clist_get_text(GTK_CLIST(w), row, 0, &db);
	    gtk_clist_get_text(GTK_CLIST(w), row, 1, &cstring);
	    g_string_sprintfa(connections, "%s%s(%s)", row > 0 ? ":" : "", db, cstring);
	}
	prefs_set_char_var(PREF_CONNECT, connections->str);
	g_string_free(connections, 1);
    } else {
	if (prefs_get_char_var(PREF_CONNECT))
	    prefs_set_char_var(PREF_CONNECT, "");
    }


    w = lookup_widget(GTK_WIDGET(button), "prefs_icon_path");
    val = gtk_entry_get_text(GTK_ENTRY(w));
    if (val && *val)
	prefs_set_char_var(PREF_ICONPATH, val);
    else if (prefs_get_char_var(PREF_ICONPATH))
	prefs_set_char_var(PREF_ICONPATH, "");

    w = lookup_widget(GTK_WIDGET(button), "prefs_bm_path");
    val = gtk_entry_get_text(GTK_ENTRY(w));
    if (val && *val)
	prefs_set_char_var(PREF_BMKPATH, val);
    else if (prefs_get_char_var(PREF_BMKPATH))
	prefs_set_char_var(PREF_BMKPATH, "");
}


void
on_dialog_cancel_clicked(
    GtkButton *button,
    gpointer  user_data)
{
    gpointer  refholder = gtk_object_get_data(GTK_OBJECT(user_data), 
					      "refholder");

    if (refholder) {
	gtk_object_weakunref(GTK_OBJECT(refholder), weakref_destroy_popup, user_data);
    }

    gtk_widget_destroy(GTK_WIDGET(user_data));
}

gboolean
on_dialog_delete_event(
    GtkWidget *widget,
    GdkEvent  *event,
    gpointer  user_data)
{
    gpointer  refholder = gtk_object_get_data(GTK_OBJECT(user_data), 
					      "refholder");

    if (refholder) {
	gtk_object_weakunref(GTK_OBJECT(refholder), weakref_destroy_popup, user_data);
    }
    
    gtk_widget_destroy(GTK_WIDGET(user_data));
    return FALSE;
}

void
on_prefs_add_conn_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    char	 *cstring = 0;
    DbInterface	 *dbif;
    char         *row[2];

    do_login_dialog("OK", 0, &dbif, &cstring);
    if (cstring) {
	row[0] = dbif->db_name;
	row[1] = cstring;
	gtk_clist_append(GTK_CLIST(user_data), row);
	g_free(cstring);
    }
}

void
on_login_ok_clicked(
    GtkButton	       *button,
    gpointer	       user_data)
{
    GtkWidget	       *dbtype = lookup_widget(GTK_WIDGET(button), "dbtype_entry");
    char	       *db     = gtk_entry_get_text(GTK_ENTRY(dbtype));
    const DbInterface  *dbif   = db_find(db);
    const DbInterface  **dbifp = gtk_object_get_data (GTK_OBJECT(user_data), "dbif_ptr");
    
    dbif->get_cstring(GTK_WIDGET(user_data));
    *dbifp = dbif;

    gtk_widget_destroy(GTK_WIDGET (user_data));
    gtk_main_quit();
}

void
on_login_cancel_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    char       **response = (char **)gtk_object_get_data (GTK_OBJECT (user_data),
							  "cstring_ptr");

    if (response && *response) {
	g_free(*response);
	*response = 0;
    }
	
    gtk_widget_destroy(GTK_WIDGET (user_data));
    gtk_main_quit();
}

gboolean
on_login_window_delete_event(
    GtkWidget       *widget,
    GdkEvent        *event,
    gpointer         user_data)
{
    on_login_cancel_clicked(0, user_data);
    return FALSE;
}

void
on_dbtype_entry_changed(
    GtkEditable	       *editable,
    gpointer	       user_data)
{
    char	       *db	 = gtk_entry_get_text(GTK_ENTRY(editable));
    GtkWidget	       *page	 = lookup_widget(GTK_WIDGET(editable), db);
    GtkWidget	       *nb	 = lookup_widget(GTK_WIDGET(editable), "login_auth_nb");
    char	       *deflogin = gtk_object_get_data(GTK_OBJECT(nb), "deflogin");
    char	       *defdb	 = 0;
    const DbInterface  *dbif;

    gtk_notebook_set_page(GTK_NOTEBOOK(nb), 
			  gtk_notebook_page_num(GTK_NOTEBOOK(nb), page));

    if (deflogin && *deflogin && (defdb = conn_getdbtype(deflogin))) {
	char *cstring = conn_getcstring(deflogin);
	dbif = db_find(defdb);
	dbif->init_login(nb, cstring);
	g_free(cstring);
    } else {
	dbif = db_find(db);
	dbif->init_login(nb, 0);
    }
    if (defdb)
	g_free(defdb);
}

void
do_login_dialog(
    const char   *login_label_text,
    const char   *deflogin,
    DbInterface	 **dbif, 
    char	 **cstring)
{
    GtkWidget	 *login_window = create_login_window ();
    GList	 *dblist       = db_list();
    GtkWidget	 *dbtypes      = lookup_widget(login_window, "dbtype_combo");
    GtkWidget    *ok_button    = lookup_widget(login_window, "login_ok");
    GtkWidget    *nb           = lookup_widget(login_window, "login_auth_nb");

    if (deflogin)
	gtk_object_set_data_full (GTK_OBJECT(nb), "deflogin", g_strdup(deflogin), g_free);

    gtk_combo_set_popdown_strings(GTK_COMBO(dbtypes), dblist);
    gtk_button_set_pixmap(ok_button, "gnome-ok.xpm", login_label_text);

    gtk_object_set_data (GTK_OBJECT(login_window), "cstring_ptr", cstring);
    gtk_object_set_data (GTK_OBJECT(login_window), "dbif_ptr", dbif);

    gtk_widget_show(login_window);
    gtk_main();
}

void
on_new_query_tbb_clicked(
    GtkButton  	 *button,
    gpointer   	 user_data)
{
    GtkWidget  	 *main_window   = GTK_WIDGET(user_data);
    GtkWidget  	 *login_combo   = lookup_widget(main_window, "current_login_combo");
    GtkWidget  	 *main_notebook = lookup_widget(main_window, "notebook1");
    GtkWidget  	 *work_area     =
	gtk_notebook_get_nth_page (GTK_NOTEBOOK(main_notebook), 
				   gtk_notebook_get_current_page(GTK_NOTEBOOK(main_notebook)));

    char	 *dbid	     = gtk_object_get_data (GTK_OBJECT (work_area), "dbid");
    char	 *cstring    = gtk_object_get_data (GTK_OBJECT (work_area), "cstring");
    DbInterface	 *dbif       = gtk_object_get_data (GTK_OBJECT (work_area), "dbif");
    GList	 *dbid_list  = gtk_object_get_data (GTK_OBJECT (main_window), "dbid_list");
    GList	 *l	     = dbid_list;
    int		 dbidlen     = strlen(dbid);
    int		 connect_num = -1;
    GtkWidget	 *page;
    int		 pageno;
    char	 *list_id;
    GtkWidget	 *sql_query;
    GSList	 *mainw_accelgps;

    /* Look through current list of connections get index for the new
       connection */
    while(l) {
	if (strncmp((char *)l->data, dbid, dbidlen) == 0) {
	    int n = 0;
	    if (((char *)l->data)[dbidlen] != '\0') {
		sscanf(((char *)l->data)+dbidlen+1, "%d", &n);
	    }
	    if ( connect_num < n)
		connect_num = n;
	}
	l = g_list_next(l);
    }
    connect_num++;

    list_id = g_malloc(dbidlen+32);
    sprintf(list_id, "%s[%d]", dbid, connect_num);

    work_area = create_work_pane();

    gtk_object_set_data_full (GTK_OBJECT (work_area), "dbid", g_strdup(dbid), g_free);
    gtk_object_set_data_full (GTK_OBJECT (work_area), "cstring", g_strdup(cstring), g_free);
    gtk_object_set_data (GTK_OBJECT (work_area), "dbif", dbif);
    gtk_object_set_data (GTK_OBJECT (work_area), "connection_num", (gpointer)connect_num);

    gtk_notebook_append_page(GTK_NOTEBOOK(main_notebook), work_area,
			     gtk_label_new(list_id));

    sql_query = gtk_object_get_data (GTK_OBJECT (work_area), "sql_query");
    mainw_accelgps = gtk_accel_groups_from_object(GTK_OBJECT(main_window));
    
    gtk_widget_add_accelerator (sql_query, "activate", mainw_accelgps->data,
				GDK_F6, 0,
				GTK_ACCEL_VISIBLE);

    /* find the page we just added */
    for (pageno = 0;
	 (page = gtk_notebook_get_nth_page (GTK_NOTEBOOK(main_notebook), pageno))
	     && page != work_area;
	 pageno++) ;
    assert(page);
    gtk_notebook_set_page(GTK_NOTEBOOK(main_notebook), pageno);

    dbid_list = g_list_append(dbid_list, list_id);
    gtk_combo_set_popdown_strings(GTK_COMBO(login_combo), dbid_list);
    gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(login_combo)->entry), list_id);
}


void
on_open_query_tbb_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    save_or_load_query(GTK_WIDGET(button), "open");
}


void
on_save_query_tbb_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    on_menusave_activate(0, user_data);
}


void
on_quit_tbb_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    gtk_main_quit();
}


static void
on_grid_view_click_column(
    GtkCList        *clist,
    gint             column,
    gpointer         user_data)
{
    if (column != clist->sort_column) {
	char	     *title;
	GtkWidget    *hbox;
	GtkWidget    *label;
	GtkWidget    *arrow;

	if (clist->sort_column != -1) {
	    title = g_strdup(gtk_clist_get_column_title (clist, clist->sort_column));
	    gtk_clist_set_column_title (clist, clist->sort_column, title);
	    g_free(title);
	}

	title = g_strdup(gtk_clist_get_column_title (clist, column));

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox);
	label = gtk_label_new(title);
	gtk_widget_show(label);
	gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);
	arrow = gtk_arrow_new(GTK_ARROW_DOWN, GTK_SHADOW_IN);
	gtk_widget_show(arrow);
	gtk_object_set_data(GTK_OBJECT(hbox), "arrow", arrow);
	gtk_box_pack_start(GTK_BOX(hbox), arrow, FALSE, FALSE, 2);

	/* There's no gtk_clist_get_column_justification function */
	switch (clist->column[column].justification) {
	case GTK_JUSTIFY_LEFT:
	    gtk_misc_set_alignment (GTK_MISC(label), 0.0, 0.5);
	    break;

	case GTK_JUSTIFY_RIGHT:
	    gtk_misc_set_alignment (GTK_MISC(label), 1.0, 0.5);
	    break;

	case GTK_JUSTIFY_CENTER:
	case GTK_JUSTIFY_FILL:
	    gtk_misc_set_alignment (GTK_MISC(label), 0.5, 0.5);
	}


	gtk_clist_set_column_widget(clist, column, hbox);
	list_set_opt_col_width(clist, column);
	gtk_clist_set_sort_type(clist, GTK_SORT_ASCENDING);
	/* Setting the widget sets the column title to null, however
	   the export functions expect the titles to be there.
	   Using set_column_title would remove the widget we just
	   set so we have to do this by fiddling with the data
	   in the widget directly (yes, this _is_ bad) */
	clist->column[column].title = title;
	gtk_clist_set_sort_column(clist, column);
    } else {
	GtkWidget *columnw = gtk_clist_get_column_widget (clist, column);
	GtkWidget *arrow = gtk_object_get_data(GTK_OBJECT(columnw), "arrow");
	
	/* re-sorting same column - toggle the sort order */
	if (clist->sort_type == GTK_SORT_ASCENDING) {
	    gtk_arrow_set(GTK_ARROW(arrow), GTK_ARROW_UP, GTK_SHADOW_IN);
	    gtk_clist_set_sort_type(clist, GTK_SORT_DESCENDING);
	} else {
	    gtk_arrow_set(GTK_ARROW(arrow), GTK_ARROW_DOWN, GTK_SHADOW_IN);
	    gtk_clist_set_sort_type(clist, GTK_SORT_ASCENDING);
	}
    }
    gtk_clist_sort(clist);
}

/* This, and the following function - on_exec_tbb_clicked are the real
   work horses of the program. on_exec_tbb_clicked is called when the
   user wants to execute some SQL. It extracts the query from the
   current sql text widget, clears out the old results and hands the
   query to the database.
   
   exec_query_rowcallback is called for each row returned by the query
   - it adds the row to the grid and text view widgets and tries to
   make sure that the columns in the text view line up nicely. */

static int
exec_query_rowcallback(
    int		 row, 
    int		 ncols, 
    char	 **colnames, 
    const short	 *indicators, 
    char	 **values, 
    gulong       *values_l, 
    short	 *types, 
    gpointer	 user_data)
{
    int		 col;
    int		 pad;
    static char	 tabs[]		= "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
    GString	 *line;
    int		 line_width;
    int		 max_col_width	= prefs_get_int_var(PREF_TEXTCOLW);
    int		 col_width;

    ExecQueryRowCBData *row_cb_data = (ExecQueryRowCBData *)user_data;
    /* if this is the first row create the clist for the grid view
       and insert the column titles into the text view */
    if (row < 0) {
	line = g_string_new("");
	row_cb_data->grid_results = (GtkCList *)gtk_clist_new_with_titles (ncols, colnames);
	row_cb_data->col_widths = g_malloc(ncols * sizeof(int));
	/* negative sort column can't be set with set_sort_column */
	row_cb_data->grid_results->sort_column = -1;
	if (prefs_get_int_var(PREF_GRIDUPD) == 0)
	    gtk_clist_freeze(row_cb_data->grid_results);
	for (col = 0; col < ncols; col++) {
	    g_string_append(line, colnames[col]);
	    /* Calculate width for this column, padded up to multiple of
	       text widget tab width */
	    col_width = MIN(MAX(values_l[col], strlen(colnames[col])), max_col_width);
	    col_width = ((col_width-1)/row_cb_data->text_results->default_tab_width)+1;
	    col_width *= row_cb_data->text_results->default_tab_width;
	    row_cb_data->col_widths[col] = col_width;
	    /* For all but the last column pad out to next column */
	    if (col < ncols-1) {
		pad = MAX(col_width - strlen(colnames[col]), 1);
		pad = ((pad-1)/row_cb_data->text_results->default_tab_width)+2;
		g_string_append(line, &tabs[sizeof(tabs)-pad]);
	    }
	    /* record type and length in case it is needed for later formatting */
	    gtk_object_set_data(GTK_OBJECT(gtk_clist_get_column_widget (row_cb_data->grid_results, col)), 
				"coltype", (gpointer)((int)types[col]));
	    gtk_object_set_data(GTK_OBJECT(gtk_clist_get_column_widget (row_cb_data->grid_results, col)), 
				"colsize", (gpointer)((int)values_l[col]));

	}
	g_string_append(line, "\n");
	gtk_text_insert(row_cb_data->text_results, row_cb_data->text_font, NULL, NULL, 
			line->str, line->len);
	line_width = text_width(row_cb_data->text_font, line->str, line->len,
				row_cb_data->text_results->default_tab_width);
	if (row_cb_data->text_width < line_width)
	    row_cb_data->text_width = line_width;
	g_string_free(line, 1);
    } else {
	if (row == 0)
	    gettimeofday(&(row_cb_data->qdataready), (struct timezone *)0);

	for (col = 0; col < ncols; col++)
	    if ( indicators[col] == -1)
		strcpy(values[col], "<NULL>");
	gtk_clist_append(row_cb_data->grid_results, values);
	line = g_string_new("");
	for (col = 0; col < ncols; col++) {
	    g_string_append(line, values[col]);
	    
	    /* For all but the last column pad out to next column */
	    if (col < ncols-1) {
		pad = MAX(row_cb_data->col_widths[col] - strlen(values[col]), 1);
		pad = ((pad-1)/row_cb_data->text_results->default_tab_width)+2;
		g_string_append(line, &tabs[sizeof(tabs)-pad]);
	    }
	}
	g_string_append(line, "\n");
	gtk_text_insert(row_cb_data->text_results, row_cb_data->text_font, NULL, NULL, 
			line->str, line->len);
	line_width = text_width(row_cb_data->text_font, line->str, line->len,
				row_cb_data->text_results->default_tab_width);
	if (row_cb_data->text_width < line_width)
	    row_cb_data->text_width = line_width;
	g_string_free(line, 1);
	
	/* Finally process events */
	while (gtk_events_pending())
	    gtk_main_iteration();
    }
    return !abort_query;
}

static void 
free_results_list(
    gpointer data)
{
    GList      *results = (GList *)data;
    GList      *result;

    for (result=results; result; result=g_list_next(result)) {
        g_free(((ExecQueryStmtRecord *)result->data)->stmt);
    }
    g_list_free (results);
}

void
on_exec_tbb_clicked(
		    GtkButton		*button,
		    gpointer		user_data)
{
    GtkWidget		*main_notebook = GTK_WIDGET(user_data);
    GtkWidget		*export_tbb    = lookup_widget(main_notebook, "export_tbb");
    GtkWidget		*exec_tbb      = lookup_widget(main_notebook, "exec_tbb");
    GtkWidget		*stop_tbb      = lookup_widget(main_notebook, "stop_tbb");
    GtkWidget		*print_tbb     = lookup_widget(main_notebook, "print_output_tbb");
    GtkWidget		*statusb       = lookup_widget(main_notebook, "statusbar2");
    GtkWidget		*work_area     =
	gtk_notebook_get_nth_page (GTK_NOTEBOOK(main_notebook), 
				   gtk_notebook_get_current_page(GTK_NOTEBOOK(main_notebook)));
    GtkText		*sql_query     = GTK_TEXT(gtk_object_get_data(GTK_OBJECT(work_area), 
								      "sql_query"));
    GtkText		*text_results  = GTK_TEXT(gtk_object_get_data(GTK_OBJECT(work_area), 
								      "text_view"));
    GtkWidget		*results_nb    = GTK_WIDGET(gtk_object_get_data(GTK_OBJECT(work_area), 
									"results_notebook"));
    char		*dbid	       = gtk_object_get_data (GTK_OBJECT (work_area), "dbid");
    DbInterface		*dbif	       = gtk_object_get_data (GTK_OBJECT (work_area), "dbif");
    int			startpos       = 0;
    int			endpos;
    char		*sql_query_str;
    char		*stmt;
    int			stmt_count     = 0;
    ExecQueryRowCBData	row_cb_data;
    GList		*results_list  = 0;
    struct timeval      start;
    struct timeval      end;

    assert(sql_query);
    
    if(GTK_EDITABLE (sql_query)->has_selection){
	startpos=GTK_EDITABLE (sql_query)->selection_start_pos;
	endpos=GTK_EDITABLE (sql_query)->selection_end_pos;

	if (startpos > endpos) {
	    startpos=GTK_EDITABLE (sql_query)->selection_end_pos;
	    endpos=GTK_EDITABLE (sql_query)->selection_start_pos;
	}
	sql_query_str = gtk_editable_get_chars(GTK_EDITABLE (sql_query),(gint) 
					       startpos,(gint) endpos);
    }else{

	sql_query_str = gtk_editable_get_chars(GTK_EDITABLE (sql_query),(gint) 0,
					       (gint) gtk_text_get_length (GTK_TEXT (sql_query)));
      
    }    
    if (sql_query_str && strlen(sql_query_str) > 0) {
	GtkWidget    *vbox;
	GtkWidget    *scrolledwindow;
	int	     col;
	int          err;

	/* Reset the results views */
	/* Remove all the pages except the first (text results) */
	while (gtk_notebook_get_nth_page(GTK_NOTEBOOK(results_nb), 1))
	    gtk_notebook_remove_page(GTK_NOTEBOOK(results_nb), 1);

	/* Clear previous text from display area */
	gtk_text_set_point(GTK_TEXT(text_results), 0);
	gtk_text_forward_delete(GTK_TEXT(text_results), 
				gtk_text_get_length(GTK_TEXT(text_results)));

	/* For some reason the text widget initialisation sets the
	   default tab width to 4 but pre-allocates the first two tabs
	   with width 8, so re-set the default to be the same as the
	   first of the preallocated tabs */
	text_results->default_tab_width = (int)text_results->tab_stops->data;

	row_cb_data.text_results = text_results;
	row_cb_data.text_width   = 0;
	row_cb_data.col_widths   = 0;
	row_cb_data.grid_results = 0;
	row_cb_data.text_font	 = gdk_font_load (prefs_get_char_var(PREF_TEXTFNT));

	if (prefs_get_int_var(PREF_TEXTUPD) == 0)
	    gtk_text_freeze(row_cb_data.text_results);

	/* Don't allow printing or export whilst a query is running
	   (although it should work it's probably a bit confusing) 

	   Don't allow a new query to be executed as we might destroy the
	   grid that we're currently writing to!

	   All of the above really apply only to the current work area
	   - if we set a "query running" flag of the work area we could 
	   just restrict this wo the work area with a query running */

	gtk_widget_set_sensitive (export_tbb, FALSE);
	gtk_widget_set_sensitive (print_tbb, FALSE);
	gtk_widget_set_sensitive (exec_tbb, FALSE);

	if (dbif->query_interruptable())
	    gtk_widget_set_sensitive (stop_tbb, TRUE);
	abort_query = FALSE;

	gtk_statusbar_pop( GTK_STATUSBAR(statusb),
			   gtk_statusbar_get_context_id(GTK_STATUSBAR(statusb),""));

	/* FIXME - this is somewhat experimental. Most importantly error
	   handling needs to be better */
	gettimeofday(&start, (struct timezone *)0);
	dbif->start_script(sql_query_str, dbid);
	while((stmt = dbif->fetch_statement(dbid)) != 0) {
	    struct timeval qend;
	    if (stmt_count > 0) {
		gtk_text_insert(GTK_TEXT(text_results), 0, 0, 0, "\n", 1);
	    }
	    ++stmt_count;
	    gettimeofday(&(row_cb_data.qstart), (struct timezone *)0);
	    row_cb_data.qdataready = row_cb_data.qstart;

	    err = dbif->do_query(stmt, dbid, prefs_get_int_var(PREF_MAXROWS),
				 exec_query_rowcallback, &row_cb_data);
	    gettimeofday(&qend, (struct timezone *)0);

	    if (err == DB_SUCCESS) {
		/* This is good up to 35 minutes */
		long t1 = (1000000*(qend.tv_sec - row_cb_data.qstart.tv_sec) 
			   + (qend.tv_usec - row_cb_data.qstart.tv_usec));
		long t2 = (1000000*(row_cb_data.qdataready.tv_sec - row_cb_data.qstart.tv_sec) 
			   + (row_cb_data.qdataready.tv_usec - row_cb_data.qstart.tv_usec));

		char statusmsg[256];
		char separator[256];
		int slen;
		ExecQueryStmtRecord *record;

		g_snprintf(statusmsg, 256, 
			   "Finished %d: %ld rows processed "
			   " %ld:%02ld.%03ld/%ld:%02ld.%03ld\n",
			   stmt_count,
			   dbif->get_rowcount(dbid),
			   t2/60000000, (t2/1000000)%60, (t2/1000)%1000,
			   t1/60000000, (t1/1000000)%60, (t1/1000)%1000);
		slen = strlen(statusmsg);
		memset(separator, '-', MIN(slen, sizeof(separator)));

		gtk_text_insert(GTK_TEXT(text_results), 0, 0, 0, separator, MIN(slen, sizeof(separator)));
		gtk_text_insert(GTK_TEXT(text_results), 0, 0, 0, "\n", 1);
		gtk_text_insert(GTK_TEXT(text_results), 0, 0, 0, statusmsg, -1);

		record = g_malloc(sizeof(*record));
		record->stmt = stmt;
		record->grid = (GtkWidget *)row_cb_data.grid_results;
		record->errored = FALSE;
		results_list = g_list_prepend(results_list, (gpointer)record);
	    } else {
		break;
	    }

	    if (row_cb_data.grid_results) {
		GtkWidget    *tab;
		char	     tab_label[64];

		vbox = gtk_vbox_new(FALSE, 0);
		gtk_widget_show(vbox);
		scrolledwindow = gtk_scrolled_window_new(0, 0);
		gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow),
						GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		
		gtk_widget_show(scrolledwindow);
		gtk_box_pack_start(GTK_BOX(vbox), scrolledwindow, TRUE, TRUE, 0);
		
		gtk_object_set_data(GTK_OBJECT(vbox), "grid", row_cb_data.grid_results);
		if (prefs_get_int_var(PREF_GRIDUPD) == 0)
		    gtk_clist_thaw(row_cb_data.grid_results);
		if (prefs_get_int_var(PREF_GRIDAUT) == 1)
		    for (col = 0; col < row_cb_data.grid_results->columns; col++) {
			list_set_opt_col_width(row_cb_data.grid_results, col);
		    }
		gtk_widget_show(GTK_WIDGET(row_cb_data.grid_results));
		gtk_container_add(GTK_CONTAINER(scrolledwindow), GTK_WIDGET(row_cb_data.grid_results));
		gtk_signal_connect (GTK_OBJECT (row_cb_data.grid_results), "click_column",
				    GTK_SIGNAL_FUNC (on_grid_view_click_column),
				    NULL);
		sprintf(tab_label, "Statement %d", stmt_count);
		tab = gtk_label_new(tab_label);
		gtk_widget_show(tab);

		gtk_notebook_append_page(GTK_NOTEBOOK(results_nb), vbox, tab);
	    }
	    row_cb_data.grid_results = 0;
	    g_free(row_cb_data.col_widths);
	    row_cb_data.col_widths   = 0;
	}

	gettimeofday(&end, (struct timezone *)0);

	if (err == DB_SUCCESS) {
	    char statusmsg[128];
	    long stime = ((end.tv_sec - start.tv_sec) * 100 +
			  (end.tv_usec - start.tv_usec)/10000);

	    GtkWidget *plan_frame = gtk_frame_new("");
	    
	    gtk_object_set_data(GTK_OBJECT(plan_frame), "query-page", (gpointer)TRUE);
	    gtk_widget_show(plan_frame);
	    gtk_notebook_append_page(GTK_NOTEBOOK(results_nb), plan_frame, 
				     gtk_label_new("Query Plan"));
	    
	    gtk_widget_set_sensitive (export_tbb, TRUE);
	    gtk_widget_set_sensitive (print_tbb, TRUE);
	    
	    if (stmt_count == 1) {
		sprintf(statusmsg, "Finished: %ld rows processed in %ld:%02ld.%02ld", 
			dbif->get_rowcount(dbid),
			stime/6000, (stime/100)%60, stime%100);
	    } else {
		sprintf(statusmsg, "Finished: %d statements processed in %ld:%02ld.%02ld", 
			stmt_count,
			stime/6000, (stime/100)%60, stime%100);
	    }
	    gtk_statusbar_push( GTK_STATUSBAR(statusb),
				gtk_statusbar_get_context_id(GTK_STATUSBAR(statusb),""),
				statusmsg);
	} else {
#if 0
	    char	 statusmsg[100];
	    long int	 errpos	  = dbif->get_error_offset(dbid);
	    
	    if (errpos > 0) {
		char	 *p	  = sql_query_str+errpos;
		int	 line     = 1;
		int	 errchar  = errpos;
		int	 i	  = 0;
		
		/* position of first error must be within string */
		assert(errpos < strlen(sql_query_str));
		
		/* count lines up to error */
		while (i < errpos) {
		    if (sql_query_str[i] == '\n') {
			++line;
			errchar = errpos-i;
		    }
		    ++i;
		}
		/* try to skip to the end of the current token */
		if (isalpha(*p) || *p == '_') {
		    while(isalnum(*p) || *p == '_')
			++p;
		} else if (isdigit(*p) || *p == '.') {
		    while(isdigit(*p) || *p == '.')
			++p;
		} else if (*p == '\'') {
		    while(*p && *p != '\'')
			++p;
		} else if (*p == '"') {
		    while(*p && *p != '"')
			++p;
		} else {
		    ++p;
		}
		gtk_editable_select_region(GTK_EDITABLE(sql_query), 
					   errpos+startpos, errpos+startpos+p-(sql_query_str+errpos));
		
		sprintf(statusmsg, "Error: line %d, char %d", line, errchar);
	    } else {
		sprintf(statusmsg, "Error");
	    }
	    
	    gtk_statusbar_push( GTK_STATUSBAR(statusb),
				gtk_statusbar_get_context_id(GTK_STATUSBAR(statusb),""),
				statusmsg);
#endif
	}
	if (results_list) {
	    results_list = g_list_reverse(results_list);
	    gtk_object_set_data_full(GTK_OBJECT(results_nb), "query-records", results_list, free_results_list);
	}

	gtk_widget_set_sensitive (stop_tbb, FALSE);
	
	if (prefs_get_int_var(PREF_TEXTUPD) == 0)
	    gtk_text_thaw(row_cb_data.text_results);
	
	gtk_widget_set_usize(GTK_WIDGET(text_results), row_cb_data.text_width, -2);
	gtk_widget_set_sensitive (exec_tbb, TRUE);
	g_free(sql_query_str);
    }
}

void
on_sql_query_activate(
    GtkEditable     *editable,
    gpointer         user_data)
{
    GtkWidget *main_notebook = GTK_WIDGET(editable)->parent->parent->parent;

    on_exec_tbb_clicked(0, main_notebook);
}

void
on_stop_tbb_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    abort_query = 1;
}


void
on_print_query_tbb_clicked(
    GtkButton	 *button,
    gpointer	 user_data)
{
    GtkText	 *query_text  = find_query_text_widget(GTK_WIDGET(button));
    GtkWidget	 *pd	      = create_print_dialog();

    gtk_object_weakref(GTK_OBJECT(query_text), weakref_destroy_popup, pd);
    gtk_object_set_data(GTK_OBJECT(pd), "refholder", query_text);
    gtk_object_set_data(GTK_OBJECT(pd), "text", query_text);

    /* Can only export as text */
    gtk_widget_set_sensitive(lookup_widget(GTK_WIDGET(pd), "printfmt_html"), FALSE);
    gtk_widget_set_sensitive(lookup_widget(GTK_WIDGET(pd), "printfmt_ps"), FALSE);

    gtk_widget_show(pd);
}

void
on_print_output_tbb_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    GtkWidget  *work_area  = find_current_work_area(GTK_WIDGET(button));
    GtkWidget  *results_nb = GTK_WIDGET(gtk_object_get_data(GTK_OBJECT(work_area), 
							    "results_notebook"));

    GtkWidget  *vbox	   = gtk_notebook_get_nth_page (GTK_NOTEBOOK(results_nb), 1);
    GtkWidget  *pd	   = create_print_dialog();

    gtk_object_weakref(GTK_OBJECT(vbox), weakref_destroy_popup, pd);
    gtk_object_set_data(GTK_OBJECT(pd), "refholder", vbox);
    gtk_object_set_data(GTK_OBJECT(pd), "grid", 
			gtk_object_get_data(GTK_OBJECT(vbox), "grid"));
    gtk_widget_show(pd);

}

void
on_print_to_file_cb_toggled(
    GtkToggleButton *togglebutton,
    gpointer         user_data)
{
    if (gtk_toggle_button_get_active (togglebutton)) {
	gtk_widget_set_sensitive ( lookup_widget ((GtkWidget *)togglebutton, 
						  "print_file_hist"), TRUE);
	gtk_widget_set_sensitive ( lookup_widget ((GtkWidget *)togglebutton, 
						  "print_filename_lab"), TRUE);
	gtk_widget_set_sensitive ( lookup_widget ((GtkWidget *)togglebutton, 
						  "print_file_browse"), TRUE);
	gtk_widget_set_sensitive ( lookup_widget ((GtkWidget *)togglebutton,
						  "print_cmd"), FALSE);
	gtk_widget_set_sensitive ( lookup_widget ((GtkWidget *)togglebutton,
						  "print_cmd_lab"), FALSE);
    } else {
	gtk_widget_set_sensitive ( lookup_widget ((GtkWidget *)togglebutton,
						  "print_cmd"), TRUE);
	gtk_widget_set_sensitive ( lookup_widget ((GtkWidget *)togglebutton,
						  "print_cmd_lab"), TRUE);
	gtk_widget_set_sensitive ( lookup_widget ((GtkWidget *)togglebutton,
						  "print_file_hist"), FALSE);
	gtk_widget_set_sensitive ( lookup_widget ((GtkWidget *)togglebutton,
						  "print_filename_lab"), FALSE);
	gtk_widget_set_sensitive ( lookup_widget ((GtkWidget *)togglebutton, 
						  "print_file_browse"), FALSE);
    }
}


void
on_print_ok_clicked(
    GtkButton	 *button,
    gpointer	 user_data)
{
    GtkObject	 *pd	     = GTK_OBJECT(user_data);
    GtkWidget	 *text	     = gtk_object_get_data(pd, "text");
    GtkWidget	 *grid	     = gtk_object_get_data(pd, "grid");
    gpointer	 refholder   = gtk_object_get_data(pd, "refholder");
    GtkWidget	 *cmd_entry  = lookup_widget(GTK_WIDGET(pd), "print_cmd");
    char	 *cmd_text   = gtk_entry_get_text(GTK_ENTRY(cmd_entry));
    
    /* Either but not both */
    assert((grid == 0) & (text != 0) || (text == 0) & (grid != 0));

    if (text) {
	char     *sql_query_str = 
	    gtk_editable_get_chars(GTK_EDITABLE (text),(gint) 0,
				   (gint) gtk_text_get_length (GTK_TEXT (text)));
	
	FILE     *stream     = popen(cmd_text, "w");

	if (stream) {
	    fwrite(sql_query_str, 1, strlen(sql_query_str), stream);
	    pclose(stream);
	} else {
	    file_open_error(cmd_text, errno, "write");
	}
    } else {
	GtkWidget  *printfmt_text = lookup_widget(GTK_WIDGET(pd), "printfmt_text");
	GtkWidget  *printfmt_html = lookup_widget(GTK_WIDGET(pd), "printfmt_html");
	GtkWidget  *printfmt_ps	  = lookup_widget(GTK_WIDGET(pd), "printfmt_ps");
	char	   *pipe	  = g_malloc(strlen(cmd_text)+2);

	strcpy(pipe, "|");
	strcat(pipe, cmd_text);

	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(printfmt_text))) {
	    export_as_text(pipe, GTK_CLIST(grid));
	} else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(printfmt_html))) {
	    export_as_html(pipe, GTK_CLIST(grid));
	} else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(printfmt_ps))) {
	    export_as_postscript(pipe, GTK_CLIST(grid));
	}
	g_free(pipe);
    }

    if (refholder) {
	gtk_object_weakunref(GTK_OBJECT(refholder), weakref_destroy_popup, pd);
    }

    gtk_widget_destroy(GTK_WIDGET(user_data));
}

void
on_export_tbb_clicked(
    GtkButton  *button,
    gpointer   user_data)
{
    GtkWidget  *work_area  = find_current_work_area(GTK_WIDGET(button));
    GtkWidget  *results_nb = GTK_WIDGET(gtk_object_get_data(GTK_OBJECT(work_area), 
							    "results_notebook"));

    GtkWidget  *vbox	   = gtk_notebook_get_nth_page (GTK_NOTEBOOK(results_nb), 1);
    GtkWidget  *fs	   = create_export_fileselector();

    gtk_object_weakref(GTK_OBJECT(vbox), weakref_destroy_popup, fs);
    gtk_object_set_data(GTK_OBJECT(fs), "grid", 
			gtk_object_get_data(GTK_OBJECT(vbox), "grid"));
    gtk_object_set_data(GTK_OBJECT(fs), "refholder", vbox);
    gtk_widget_set_sensitive(GTK_FILE_SELECTION (fs)->ok_button, FALSE);
    gtk_widget_show(fs);
}


void
on_menuhelp_activate(
    GtkMenuItem     *menuitem,
    gpointer         user_data)
{
    /* this function intentionally does nothing */
}


void
on_menuabout_activate(
    GtkMenuItem     *menuitem,
    gpointer         user_data)
{

}


void
on_results_notebook_switch_page(
    GtkNotebook	     *notebook,
    GtkNotebookPage  *page,
    gint	     page_num,
    gpointer	     user_data)
{
    if		     (!GTK_OBJECT_DESTROYED(notebook)) {
	/* Navigate widgets to get the query displayed by the current grid */
	GtkWidget    *work_area	 = GTK_WIDGET(notebook)->parent;
	char	     *dbid	 = gtk_object_get_data (GTK_OBJECT (work_area), "dbid");
	DbInterface  *dbif	 = gtk_object_get_data (GTK_OBJECT (work_area), "dbif");
	GtkWidget    *page       = gtk_notebook_get_nth_page (notebook, page_num);

	assert(dbid != 0);

	if (gtk_object_get_data(GTK_OBJECT(page), "query-page") &&
	    !GTK_BIN(page)->child ) {
	    /* GtkWidget  *grid_vbox = gtk_notebook_get_nth_page (notebook, 1); */
	    GList      *results = gtk_object_get_data(GTK_OBJECT(notebook), "query-records");
	    GList      *result;
	    GtkWidget  *vbox = gtk_vbox_new(FALSE, 0);
	    GtkWidget  *scrolledwindow = gtk_scrolled_window_new(0, 0);

	    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow),
					    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	    gtk_widget_show(scrolledwindow);
	    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledwindow), vbox);
	    gtk_widget_show(vbox);
	    gtk_container_add(GTK_CONTAINER(page), scrolledwindow);

	    for (result=results; result; result=g_list_next(result)) {
	      GtkWidget  *plan;
	      GtkStyle   *style = gtk_style_new();
	      char       *stmt = ((ExecQueryStmtRecord *)result->data)->stmt;
	      GtkWidget  *text = gtk_label_new(stmt);

	      gtk_label_set_justify(GTK_LABEL(text), GTK_JUSTIFY_LEFT);
	      style->fg[0].blue = 65535;
	      gtk_widget_set_style(text, style);
	      gtk_misc_set_alignment (GTK_MISC(text), 0.0, 0.5);
	      gtk_widget_show(GTK_WIDGET(text));
	      gtk_box_pack_start(GTK_BOX(vbox), text, TRUE, TRUE, 0);
	      
	      plan = dbif->explain_plan(stmt, dbid, GTK_WIDGET(notebook));
	      if (plan) {
		gtk_widget_show(GTK_WIDGET(plan));
		gtk_box_pack_start(GTK_BOX(vbox), plan, TRUE, TRUE, 0);
	      }
	    }
	}
    }
}

void
on_current_login_ent_changed(
    GtkEditable	 *editable,
    gpointer	 user_data)
{
    GtkNotebook	 *main_notebook	 = GTK_NOTEBOOK(user_data);
    char	 *list_id	 = gtk_entry_get_text(GTK_ENTRY(editable));
    int		 pageno;
    GtkWidget	 *page;

    /* find the page corresponding to the selected entry */
    for (pageno = 0;
	 (page = gtk_notebook_get_nth_page (main_notebook, pageno)) != 0;
	 pageno++) 
    {
	const char  *page_dbid	     = (const char *)gtk_object_get_data(GTK_OBJECT(page), "dbid");
	int	    page_connect_num = (int) gtk_object_get_data(GTK_OBJECT(page), "connection_num");
	char	    *page_list_id;

	if (page_connect_num == 0) {
	    page_list_id = g_strdup(page_dbid);
	} else {
	    page_list_id = g_malloc(strlen(page_dbid)+23);
	    sprintf(page_list_id, "%s[%d]", page_dbid, page_connect_num);
	}
	if (strcmp(list_id, page_list_id) == 0) { 
	    g_free(page_list_id);
	    gtk_notebook_set_page(GTK_NOTEBOOK(main_notebook), pageno);
	    return;
	}
	g_free(page_list_id);
    }
    show_warning_msg("Error - connection %s not found!\n"
		     "It is advisable to save any uncommitted work and\n"
		     "restart the application",
		     list_id);
}

void
on_filesel_entry_changed(
    GtkEditable	 *editable,
    gpointer	 user_data)
{
    char *s = gtk_entry_get_text(GTK_ENTRY(editable));

    gtk_widget_set_sensitive(GTK_FILE_SELECTION (user_data)->ok_button,
			     (s && strlen(s) > 0));
}

void
on_export_ok_button_clicked(
    GtkButton	      *button,
    gpointer	      user_data)
{
    GtkFileSelection  *fs	       = GTK_FILE_SELECTION(user_data);
    GtkWidget	      *export_sql      = lookup_widget(GTK_WIDGET(fs), "export_sql");
    GtkWidget	      *export_text     = lookup_widget(GTK_WIDGET(fs), "export_text");
    GtkWidget	      *export_html     = lookup_widget(GTK_WIDGET(fs), "export_html");
    GtkWidget	      *export_csv      = lookup_widget(GTK_WIDGET(fs), "export_csv");
    GtkWidget	      *export_delimit  = lookup_widget(GTK_WIDGET(fs), "export_delimit");
    GtkWidget         *results_grid    = gtk_object_get_data(GTK_OBJECT(fs), "grid");
    char              *filename        = gtk_file_selection_get_filename(fs);
    gpointer          refholder        = gtk_object_get_data(GTK_OBJECT(fs), "refholder");


    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(export_text))) {
	export_as_text(filename, GTK_CLIST(results_grid));
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(export_html))) {
	export_as_html(filename, GTK_CLIST(results_grid));
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(export_sql))) {
	GtkWidget  *table_name_ent = lookup_widget(GTK_WIDGET(fs), "export_tablename");
	GtkWidget  *create_tab	   = lookup_widget(GTK_WIDGET(fs), "export_table_create");
	char	   *table_name	   = gtk_entry_get_text(GTK_ENTRY(table_name_ent));

	gtk_object_set_data_full(GTK_OBJECT(results_grid), "tablename", 
				 g_strdup(table_name), g_free);
	gtk_object_set_data(GTK_OBJECT(results_grid), "create", 
			    (gpointer)gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(create_tab)));
	export_as_sql(filename, GTK_CLIST(results_grid));
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(export_csv))) {
	export_as_csv(filename, GTK_CLIST(results_grid));
    } else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(export_delimit))) {
    }

    if (refholder) {
	gtk_object_weakunref(GTK_OBJECT(refholder), weakref_destroy_popup, fs);
    }

    gtk_widget_destroy(GTK_WIDGET(user_data));
}

void
on_export_as_sql_toggled(
    GtkToggleButton *togglebutton,
    gpointer         user_data)
{
    if (gtk_toggle_button_get_active (togglebutton)) {
	gtk_widget_set_sensitive ( lookup_widget ((GtkWidget *)togglebutton, 
						  "label1"), TRUE);
	gtk_widget_set_sensitive ( lookup_widget ((GtkWidget *)togglebutton, 
						  "export_tablename"), TRUE);
	gtk_widget_set_sensitive ( lookup_widget ((GtkWidget *)togglebutton, 
						  "export_table_create"), TRUE);
    } else {
	gtk_widget_set_sensitive ( lookup_widget ((GtkWidget *)togglebutton,
						  "label1"), FALSE);
	gtk_widget_set_sensitive ( lookup_widget ((GtkWidget *)togglebutton,
						  "export_tablename"), FALSE);
	gtk_widget_set_sensitive ( lookup_widget ((GtkWidget *)togglebutton,
						  "export_table_create"), FALSE);
    }
}

void
on_saveq_ok_button_clicked(
    GtkButton	      *button,
    gpointer	      user_data)
{
    GtkFileSelection  *fs	 = GTK_FILE_SELECTION(user_data);
    char	      *filename	 = gtk_file_selection_get_filename(fs);
    char	      *action	 = gtk_object_get_data(GTK_OBJECT(fs), "action");
    GtkText	      *query	 = GTK_TEXT(gtk_object_get_data(GTK_OBJECT(fs), "query"));
    char	      *last_save = gtk_object_get_data(GTK_OBJECT(query), "last-saved-as");
    int		      qlen	 = gtk_text_get_length(query);
    char	      *qtext	 = gtk_editable_get_chars(GTK_EDITABLE(query), 0, qlen);

    if (strcmp(action, "open") == 0) {
	FILE	     *inf     = fopen(filename, "r");
	if (inf) {
	    struct stat	 st;
	    char	 *filebuf;
	    
	    if (fstat(fileno(inf), &st) < 0) {
		show_warning_msg("Can't stat open file %s, *highly* wierd!", 
				 filename);
	    } else if (st.st_size > 0) {
		/* If the current work area is blank, or was last saved to
		   the same file load the query into it. Otherwise open a new
		   work area on the same connection and load the file into that */
		if (qtext && *qtext && (!last_save || strcmp(filename, last_save) != 0)) {
		    GtkWidget *mw = lookup_widget(GTK_WIDGET(query), "main_window");
		    on_new_query_tbb_clicked(0, mw);
		    query = find_query_text_widget(mw);
		}
		filebuf = g_malloc(st.st_size);
		fread(filebuf, 1, st.st_size, inf);
		gtk_text_set_point(GTK_TEXT(query), 0);
		gtk_text_forward_delete(GTK_TEXT(query), 
					gtk_text_get_length(GTK_TEXT(query)));

		gtk_text_insert(GTK_TEXT(query), 0, 0, 0, filebuf, st.st_size);
		gtk_object_set_data_full(GTK_OBJECT(query), "last-saved-as", 
					 g_strdup(filename), g_free);
		g_free(filebuf);
	    }
	}
    } else {
	FILE *f = fopen(filename, "w");
	if (f) {
	    fwrite(qtext, 1, qlen, f);
	    fclose(f);
	    gtk_object_set_data_full(GTK_OBJECT(query), "last-saved-as", 
				     g_strdup(filename), g_free);
	} else {
	    file_open_error(filename, errno, "write");
	}
    }
    g_free(qtext);
    gtk_widget_destroy(GTK_WIDGET(user_data));
}

void
on_fontsel_cancel_button_clicked(
    GtkButton	 *button,
    gpointer	 user_data)
{
    GtkWidget	 *prefs_win;
    GtkWidget	 *w;
    char	 *font;

    prefs_win = lookup_widget(GTK_WIDGET(user_data), "prefs_dialog");
    w = lookup_widget(prefs_win, "prefs_textv_font");
    font = gtk_object_get_data(GTK_OBJECT(user_data), "prev_font");
    gtk_entry_set_text(GTK_ENTRY(w), font);

    gtk_object_weakunref (GTK_OBJECT(prefs_win),
			  weakref_destroy_popup, user_data);
    gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_fontsel_apply_button_clicked(
    GtkButton	 *button,
    gpointer	 user_data)
{
    GtkWidget	 *prefs_win;
    GtkWidget	 *w;
    
    prefs_win = lookup_widget(GTK_WIDGET(user_data), "prefs_dialog");
    w = lookup_widget(prefs_win, "prefs_textv_font");

    gtk_entry_set_text(GTK_ENTRY(w), 
		       gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(user_data)));
}


void
on_fontsel_ok_button_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    GtkWidget	 *prefs_win;

    prefs_win = lookup_widget(GTK_WIDGET(user_data), "prefs_dialog");

    on_fontsel_apply_button_clicked(button, user_data);
    gtk_object_weakunref (GTK_OBJECT(prefs_win),
			  weakref_destroy_popup, user_data);
    gtk_widget_destroy(GTK_WIDGET(user_data));
}




void
on_prefs_sel_iconpath_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_prefs_sel_bmpath_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_prefs_edit_conn_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    GtkCList *cl = GTK_CLIST(user_data);
    
    if (cl->selection) {
	GString	     *connection  = g_string_new("");
	DbInterface  *dbif;
	gchar	     *db;
	gchar	     *cstring = 0;
	gint	     row	  = (gint)(cl->selection->data);

	gtk_clist_get_text(cl, row, 0, &db);
	gtk_clist_get_text(cl, row, 1, &cstring);
	cstring = g_strdup(cstring);
	g_string_sprintfa(connection, "%s%s(%s)", row > 0 ? ":" : "", db, cstring);
	
	do_login_dialog("OK", connection->str, &dbif, &cstring);

	if (cstring) {
	    gtk_clist_set_text(cl, row, 0, dbif->db_name);
	    gtk_clist_set_text(cl, row, 1, cstring);
	
	    g_free(cstring);
	}
	g_string_free(connection, 1);
    }
}


void
on_prefs_delete_conn_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    GtkCList *cl = GTK_CLIST(user_data);

    if (cl->selection) {
	gtk_clist_remove(cl, (gint)(cl->selection->data));
    }
}



void
on_dirsel_ok_button_clicked(
    GtkButton       *button,
    gpointer         user_data)
{

}

void
on_main_window_map(
    GtkWidget       *widget,
    gpointer         user_data)
{
    build_bookmarks_menus(widget);
}
