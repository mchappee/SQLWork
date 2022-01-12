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
#include <gtk/gtk.h>

#include "dbif.h"

gboolean
on_main_window_delete_event(GtkWidget       *widget,
			    GdkEvent        *event,
			    gpointer         user_data);

void
on_menuopen_activate(GtkMenuItem     *menuitem,
		     gpointer         user_data);

void
on_menusave_activate(GtkMenuItem     *menuitem,
		     gpointer         user_data);

void
on_menusaveas_activate(GtkMenuItem     *menuitem,
		       gpointer         user_data);

void
on_add_connection_activate(GtkMenuItem     *menuitem,
			   gpointer         user_data);

void
on_save_connecion_activate(GtkMenuItem     *menuitem,
			    gpointer         user_data);

void
on_close_connection_activate(GtkMenuItem     *menuitem,
			     gpointer         user_data);

void
on_menuexit_activate(GtkMenuItem     *menuitem,
		     gpointer         user_data);

void
on_menucut_activate(GtkMenuItem     *menuitem,
		    gpointer         user_data);

void
on_menucopy_activate(GtkMenuItem     *menuitem,
		     gpointer         user_data);

void
on_menupaste_activate(GtkMenuItem     *menuitem,
		      gpointer         user_data);

void
on_menuselectall_activate(GtkMenuItem     *menuitem,
			  gpointer         user_data);

void
on_menuclear_activate(GtkMenuItem     *menuitem,
		      gpointer         user_data);

void
on_menuprefs_activate(GtkMenuItem     *menuitem,
                                        gpointer         user_data);

gboolean
on_login_window_delete_event(GtkWidget       *widget,
			     GdkEvent        *event,
			     gpointer         user_data);

void
on_login_ok_clicked(GtkButton       *button,
		    gpointer         user_data);

void
on_login_cancel_clicked(GtkButton       *button,
			gpointer         user_data);

void
do_login_dialog(const char *, const char *, DbInterface **, char **);

void
on_new_query_tbb_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_open_query_tbb_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_save_query_tbb_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_print_query_tbb_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_quit_tbb_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_exec_tbb_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_stop_tbb_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_print_output_tbb_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_export_tbb_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_menuhelp_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_menuabout_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_results_notebook_switch_page        (GtkNotebook     *notebook,
                                        GtkNotebookPage *page,
                                        gint             page_num,
                                        gpointer         user_data);

void
on_current_login_ent_changed           (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_prefs_sel_font_clicked              (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_prefs_window_delete_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_prefs_ok_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_prefs_apply_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_dialog_cancel_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_filesel_entry_changed(GtkEditable *editable, gpointer user_data);

void
on_export_ok_button_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_saveq_ok_button_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_fontsel_cancel_button_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_fontsel_apply_button_clicked        (GtkButton       *button,
                                        gpointer         user_data);

void
on_fontsel_ok_button_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_prefs_sel_font_clicked              (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_prefs_window_delete_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_prefs_apply_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_dbtype_entry_changed                (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_login_ok_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_login_cancel_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_sql_query_activate                  (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_prefs_sel_iconpath_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_prefs_sel_bmpath_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_prefs_edit_conn_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_prefs_delete_conn_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_prefs_add_conn_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_save_connecion_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_close_connection_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_dirsel_ok_button_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_main_window_map                     (GtkWidget       *widget,
                                        gpointer         user_data);

/* This is in mysqldb.c */
gboolean
on_mysql_login_entry_focus_out_event (GtkWidget       *widget,
				      GdkEventFocus   *event,
				      gpointer         user_data);

void
on_print_to_file_cb_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_print_ok_clicked                    (GtkButton       *button,
                                        gpointer         user_data);
gboolean
on_print_dialog_delete_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);


void
on_print_file_browse_clicked           (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_dialog_delete_event                 (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);
void
on_export_as_sql_toggled               (GtkToggleButton *togglebutton,
					gpointer         user_data);
