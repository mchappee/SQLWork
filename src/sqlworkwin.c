/* SQLWork - the Oracle Query tool
 *
 * Copyright ? Matthew Chappee <matthew@orasoft.org>
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
 * This file is partly auto-generated. It may be edited but do not
 * change the formatting or indentation
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "sqlworksig.h"
#include "sqlworkwin.h"
#include "bookmarksig.h"

#include "support.h"
#include "miscfunc.h"
#include "prefs.h"

GtkWidget*
create_main_window (void)
{
  GtkWidget *main_window;
  GtkWidget *vbox1;
  GtkWidget *menubar1;
  guint tmp_key;
  GtkWidget *file1;
  GtkWidget *file1_menu;
  GtkAccelGroup *file1_menu_accels;
  GtkWidget *menuopen;
  GtkWidget *menusave;
  GtkWidget *menusaveas;
  GtkWidget *separator1;
  GtkWidget *add_connection;
  GtkWidget *save_connecion;
  GtkWidget *close_connection;
  GtkWidget *separator2;
  GtkWidget *menuexit;
  GtkWidget *edit1;
  GtkWidget *edit1_menu;
  GtkAccelGroup *edit1_menu_accels;
  GtkWidget *menucut;
  GtkWidget *menucopy;
  GtkWidget *menupaste;
  GtkWidget *menuselectall;
  GtkWidget *menuclear;
  GtkWidget *settings1;
  GtkWidget *settings1_menu;
  GtkAccelGroup *settings1_menu_accels;
  GtkWidget *menuprefs;
  GtkWidget *bookmarks;
  GtkWidget *bookmarks_menu;
  GtkAccelGroup *bookmarks_menu_accels;
  GtkWidget *add_bookmark_default;
  GtkWidget *add_bookmark;
  GtkWidget *insert_bookmark;
  GtkWidget *run_bookmark;
  GtkWidget *organise_bookmark;
  GtkWidget *help;
  GtkWidget *help_menu;
  GtkAccelGroup *help_menu_accels;
  GtkWidget *about;
  GtkWidget *toolbar1;
  GtkWidget *tmp_toolbar_icon;
  GtkWidget *new_query_tbb;
  GtkWidget *open_query_tbb;
  GtkWidget *save_query_tbb;
  GtkWidget *print_query_tbb;
  GtkWidget *tbb_vseparator1;
  GtkWidget *exec_tbb;
  GtkWidget *stop_tbb;
  GtkWidget *tbb_vseparator2;
  GtkWidget *print_output_tbb;
  GtkWidget *export_tbb;
  GtkWidget *notebook1;
  GtkWidget *nb_dummy;
  GtkWidget *hbox1;
  GtkWidget *current_login_combo;
  GtkWidget *current_login_ent;
  GtkWidget *statusbar2;
  GtkWidget *pixmap1;
  GtkAccelGroup *accel_group;
  GtkToolbarStyle tbstyle;

  accel_group = gtk_accel_group_new ();

  main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_object_set_data (GTK_OBJECT (main_window), "main_window", main_window);
  gtk_window_set_title (GTK_WINDOW (main_window), _("SQLWork 5.0"));

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "vbox1", vbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (main_window), vbox1);

  menubar1 = gtk_menu_bar_new ();
  gtk_widget_ref (menubar1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "menubar1", menubar1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (menubar1);
  gtk_box_pack_start (GTK_BOX (vbox1), menubar1, FALSE, FALSE, 0);

  file1 = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (file1)->child),
                                   _("_File"));
  gtk_widget_add_accelerator (file1, "activate_item", accel_group,
                              tmp_key, GDK_MOD1_MASK, 0);
  gtk_widget_ref (file1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "file1", file1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (file1);
  gtk_container_add (GTK_CONTAINER (menubar1), file1);

  file1_menu = gtk_menu_new ();
  gtk_widget_ref (file1_menu);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "file1_menu", file1_menu,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (file1), file1_menu);
  file1_menu_accels = gtk_menu_ensure_uline_accel_group (GTK_MENU (file1_menu));

  menuopen = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (menuopen)->child),
                                   _("_Open"));
  gtk_widget_add_accelerator (menuopen, "activate_item", file1_menu_accels,
                              tmp_key, 0, 0);
  gtk_widget_ref (menuopen);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "menuopen", menuopen,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (menuopen);
  gtk_container_add (GTK_CONTAINER (file1_menu), menuopen);

  menusave = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (menusave)->child),
                                   _("_Save"));
  gtk_widget_add_accelerator (menusave, "activate_item", file1_menu_accels,
                              tmp_key, 0, 0);
  gtk_widget_ref (menusave);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "menusave", menusave,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (menusave);
  gtk_container_add (GTK_CONTAINER (file1_menu), menusave);

  menusaveas = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (menusaveas)->child),
                                   _("Save _As"));
  gtk_widget_add_accelerator (menusaveas, "activate_item", file1_menu_accels,
                              tmp_key, 0, 0);
  gtk_widget_ref (menusaveas);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "menusaveas", menusaveas,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (menusaveas);
  gtk_container_add (GTK_CONTAINER (file1_menu), menusaveas);

  separator1 = gtk_menu_item_new ();
  gtk_widget_ref (separator1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "separator1", separator1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (separator1);
  gtk_container_add (GTK_CONTAINER (file1_menu), separator1);
  gtk_widget_set_sensitive (separator1, FALSE);

  add_connection = gtk_menu_item_new_with_label (_("Add Connection"));
  gtk_widget_ref (add_connection);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "add_connection", add_connection,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (add_connection);
  gtk_container_add (GTK_CONTAINER (file1_menu), add_connection);

  save_connecion = gtk_menu_item_new_with_label (_("Save Connecion"));
  gtk_widget_ref (save_connecion);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "save_connecion", save_connecion,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (save_connecion);
  gtk_container_add (GTK_CONTAINER (file1_menu), save_connecion);

  close_connection = gtk_menu_item_new_with_label (_("Close Connection"));
  gtk_widget_ref (close_connection);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "close_connection", close_connection,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (close_connection);
  gtk_container_add (GTK_CONTAINER (file1_menu), close_connection);

  separator2 = gtk_menu_item_new ();
  gtk_widget_ref (separator2);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "separator2", separator2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (separator2);
  gtk_container_add (GTK_CONTAINER (file1_menu), separator2);
  gtk_widget_set_sensitive (separator2, FALSE);

  menuexit = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (menuexit)->child),
                                   _("_Exit"));
  gtk_widget_add_accelerator (menuexit, "activate_item", file1_menu_accels,
                              tmp_key, 0, 0);
  gtk_widget_ref (menuexit);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "menuexit", menuexit,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (menuexit);
  gtk_container_add (GTK_CONTAINER (file1_menu), menuexit);

  edit1 = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (edit1)->child),
                                   _("_Edit"));
  gtk_widget_add_accelerator (edit1, "activate_item", accel_group,
                              tmp_key, GDK_MOD1_MASK, 0);
  gtk_widget_ref (edit1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "edit1", edit1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (edit1);
  gtk_container_add (GTK_CONTAINER (menubar1), edit1);

  edit1_menu = gtk_menu_new ();
  gtk_widget_ref (edit1_menu);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "edit1_menu", edit1_menu,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (edit1), edit1_menu);
  edit1_menu_accels = gtk_menu_ensure_uline_accel_group (GTK_MENU (edit1_menu));

  menucut = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (menucut)->child),
                                   _("C_ut"));
  gtk_widget_add_accelerator (menucut, "activate_item", edit1_menu_accels,
                              tmp_key, 0, 0);
  gtk_widget_ref (menucut);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "menucut", menucut,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (menucut);
  gtk_container_add (GTK_CONTAINER (edit1_menu), menucut);

  menucopy = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (menucopy)->child),
                                   _("_Copy"));
  gtk_widget_add_accelerator (menucopy, "activate_item", edit1_menu_accels,
                              tmp_key, 0, 0);
  gtk_widget_ref (menucopy);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "menucopy", menucopy,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (menucopy);
  gtk_container_add (GTK_CONTAINER (edit1_menu), menucopy);

  menupaste = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (menupaste)->child),
                                   _("_Paste"));
  gtk_widget_add_accelerator (menupaste, "activate_item", edit1_menu_accels,
                              tmp_key, 0, 0);
  gtk_widget_ref (menupaste);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "menupaste", menupaste,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (menupaste);
  gtk_container_add (GTK_CONTAINER (edit1_menu), menupaste);

  menuselectall = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (menuselectall)->child),
                                   _("_Select All"));
  gtk_widget_add_accelerator (menuselectall, "activate_item", edit1_menu_accels,
                              tmp_key, 0, 0);
  gtk_widget_ref (menuselectall);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "menuselectall", menuselectall,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (menuselectall);
  gtk_container_add (GTK_CONTAINER (edit1_menu), menuselectall);

  menuclear = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (menuclear)->child),
                                   _("C_lear"));
  gtk_widget_add_accelerator (menuclear, "activate_item", edit1_menu_accels,
                              tmp_key, 0, 0);
  gtk_widget_ref (menuclear);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "menuclear", menuclear,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (menuclear);
  gtk_container_add (GTK_CONTAINER (edit1_menu), menuclear);

  settings1 = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (settings1)->child),
                                   _("_Settings"));
  gtk_widget_add_accelerator (settings1, "activate_item", accel_group,
                              tmp_key, GDK_MOD1_MASK, 0);
  gtk_widget_ref (settings1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "settings1", settings1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (settings1);
  gtk_container_add (GTK_CONTAINER (menubar1), settings1);

  settings1_menu = gtk_menu_new ();
  gtk_widget_ref (settings1_menu);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "settings1_menu", settings1_menu,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (settings1), settings1_menu);
  settings1_menu_accels = gtk_menu_ensure_uline_accel_group (GTK_MENU (settings1_menu));

  menuprefs = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (menuprefs)->child),
                                   _("_Preferences..."));
  gtk_widget_add_accelerator (menuprefs, "activate_item", settings1_menu_accels,
                              tmp_key, 0, 0);
  gtk_widget_ref (menuprefs);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "menuprefs", menuprefs,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (menuprefs);
  gtk_container_add (GTK_CONTAINER (settings1_menu), menuprefs);

  bookmarks = gtk_menu_item_new_with_label (_("Bookmarks"));
  gtk_widget_ref (bookmarks);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "bookmarks", bookmarks,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bookmarks);
  gtk_container_add (GTK_CONTAINER (menubar1), bookmarks);

  bookmarks_menu = gtk_menu_new ();
  gtk_widget_ref (bookmarks_menu);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "bookmarks_menu", bookmarks_menu,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (bookmarks), bookmarks_menu);
  bookmarks_menu_accels = gtk_menu_ensure_uline_accel_group (GTK_MENU (bookmarks_menu));

  add_bookmark_default = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (add_bookmark_default)->child),
                                   _("_Add"));
  gtk_widget_add_accelerator (add_bookmark_default, "activate_item", bookmarks_menu_accels,
                              tmp_key, 0, 0);
  gtk_widget_ref (add_bookmark_default);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "add_bookmark_default", add_bookmark_default,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (add_bookmark_default);
  gtk_container_add (GTK_CONTAINER (bookmarks_menu), add_bookmark_default);

  add_bookmark = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (add_bookmark)->child),
                                   _("Add _To"));
  gtk_widget_add_accelerator (add_bookmark, "activate_item", bookmarks_menu_accels,
                              tmp_key, 0, 0);
  gtk_widget_ref (add_bookmark);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "add_bookmark", add_bookmark,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (add_bookmark);
  gtk_container_add (GTK_CONTAINER (bookmarks_menu), add_bookmark);

  insert_bookmark = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (insert_bookmark)->child),
                                   _("_Insert"));
  gtk_widget_add_accelerator (insert_bookmark, "activate_item", bookmarks_menu_accels,
                              tmp_key, 0, 0);
  gtk_widget_ref (insert_bookmark);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "insert_bookmark", insert_bookmark,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (insert_bookmark);
  gtk_container_add (GTK_CONTAINER (bookmarks_menu), insert_bookmark);

  run_bookmark = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (run_bookmark)->child),
                                   _("_Run"));
  gtk_widget_add_accelerator (run_bookmark, "activate_item", bookmarks_menu_accels,
                              tmp_key, 0, 0);
  gtk_widget_ref (run_bookmark);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "run_bookmark", run_bookmark,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (run_bookmark);
  gtk_container_add (GTK_CONTAINER (bookmarks_menu), run_bookmark);

  organise_bookmark = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (organise_bookmark)->child),
                                   _("_Organise"));
  gtk_widget_add_accelerator (organise_bookmark, "activate_item", bookmarks_menu_accels,
                              tmp_key, 0, 0);
  gtk_widget_ref (organise_bookmark);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "organise_bookmark", organise_bookmark,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (organise_bookmark);
  gtk_container_add (GTK_CONTAINER (bookmarks_menu), organise_bookmark);

  help = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (help)->child),
                                   _("_Help"));
  gtk_widget_add_accelerator (help, "activate_item", accel_group,
                              tmp_key, GDK_MOD1_MASK, 0);
  gtk_widget_ref (help);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "help", help,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (help);
  gtk_container_add (GTK_CONTAINER (menubar1), help);
  gtk_menu_item_right_justify (GTK_MENU_ITEM (help));

  help_menu = gtk_menu_new ();
  gtk_widget_ref (help_menu);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "help_menu", help_menu,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (help), help_menu);
  help_menu_accels = gtk_menu_ensure_uline_accel_group (GTK_MENU (help_menu));

  about = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (about)->child),
                                   _("_About"));
  gtk_widget_add_accelerator (about, "activate_item", help_menu_accels,
                              tmp_key, 0, 0);
  gtk_widget_ref (about);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "about", about,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (about);
  gtk_container_add (GTK_CONTAINER (help_menu), about);

  toolbar1 = gtk_toolbar_new (GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH);
  gtk_widget_ref (toolbar1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "toolbar1", toolbar1,
                            (GtkDestroyNotify) gtk_widget_unref);
  tbstyle = prefs_get_int_var(PREF_TBSTYLE);
  if (tbstyle != -1) {
      gtk_toolbar_set_style(GTK_TOOLBAR(toolbar1), tbstyle);
      gtk_widget_show (toolbar1);
  }
  
  gtk_box_pack_start (GTK_BOX (vbox1), toolbar1, FALSE, FALSE, 0);
  gtk_toolbar_set_button_relief (GTK_TOOLBAR (toolbar1), GTK_RELIEF_NONE);

  tmp_toolbar_icon = create_pixmap (main_window, "new.xpm");
  new_query_tbb = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                _("New"),
                                _("Start a new query"), NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref (new_query_tbb);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "new_query_tbb", new_query_tbb,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (new_query_tbb);

  tmp_toolbar_icon = create_pixmap (main_window, "open.xpm");
  open_query_tbb = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                _("Open"),
                                _("Read qiuery from file"), NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref (open_query_tbb);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "open_query_tbb", open_query_tbb,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (open_query_tbb);

  tmp_toolbar_icon = create_pixmap (main_window, "save.xpm");
  save_query_tbb = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                _("Save"),
                                _("Save query to file"), NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref (save_query_tbb);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "save_query_tbb", save_query_tbb,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (save_query_tbb);

  tmp_toolbar_icon = create_pixmap (main_window, "print.xpm");
  print_query_tbb = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                _("Print"),
                                _("Send a copy of the query to the printer"), NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref (print_query_tbb);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "print_query_tbb", print_query_tbb,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (print_query_tbb);

  tbb_vseparator1 = gtk_vseparator_new ();
  gtk_widget_ref (tbb_vseparator1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "tbb_vseparator1", tbb_vseparator1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (tbb_vseparator1);
  gtk_toolbar_append_widget (GTK_TOOLBAR (toolbar1), tbb_vseparator1, NULL, NULL);
  gtk_widget_set_usize (tbb_vseparator1, -2, 20);

  tmp_toolbar_icon = create_pixmap (main_window, "submit.xpm");
  exec_tbb = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                _("Execute"),
                                _("Run the current query"), NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref (exec_tbb);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "exec_tbb", exec_tbb,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (exec_tbb);

  tmp_toolbar_icon = create_pixmap (main_window, "stop.xpm");
  stop_tbb = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                _("Stop"),
                                _("Stop a long running query"), NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref (stop_tbb);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "stop_tbb", stop_tbb,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (stop_tbb);
  gtk_widget_set_sensitive (stop_tbb, FALSE);

  tbb_vseparator2 = gtk_vseparator_new ();
  gtk_widget_ref (tbb_vseparator2);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "tbb_vseparator2", tbb_vseparator2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (tbb_vseparator2);
  gtk_toolbar_append_widget (GTK_TOOLBAR (toolbar1), tbb_vseparator2, NULL, NULL);
  gtk_widget_set_usize (tbb_vseparator2, -2, 20);

  tmp_toolbar_icon = create_pixmap (main_window, "print.xpm");
  print_output_tbb = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                _("Print Output"),
                                _("Print the results of the query"), NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref (print_output_tbb);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "print_output_tbb", print_output_tbb,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (print_output_tbb);
  gtk_widget_set_sensitive (print_output_tbb, FALSE);

  tmp_toolbar_icon = create_pixmap (main_window, "save.xpm");
  export_tbb = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar1),
                                GTK_TOOLBAR_CHILD_BUTTON,
                                NULL,
                                _("Export"),
                                _("Save the results of the query to a file"), NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref (export_tbb);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "export_tbb", export_tbb,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (export_tbb);
  gtk_widget_set_sensitive (export_tbb, FALSE);

  notebook1 = gtk_notebook_new ();
  gtk_widget_ref (notebook1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "notebook1", notebook1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (notebook1);
  gtk_box_pack_start (GTK_BOX (vbox1), notebook1, TRUE, TRUE, 0);
  GTK_WIDGET_UNSET_FLAGS (notebook1, GTK_CAN_FOCUS);
  gtk_notebook_set_show_tabs (GTK_NOTEBOOK (notebook1), FALSE);
  gtk_notebook_set_show_border (GTK_NOTEBOOK (notebook1), FALSE);

  nb_dummy = gtk_frame_new (NULL);
  gtk_widget_ref (nb_dummy);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "nb_dummy", nb_dummy,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (nb_dummy);
  gtk_container_add (GTK_CONTAINER (notebook1), nb_dummy);
  gtk_widget_set_usize (nb_dummy, -2, 200);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "hbox1", hbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);

  current_login_combo = gtk_combo_new ();
  gtk_widget_ref (current_login_combo);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "current_login_combo", current_login_combo,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (current_login_combo);
  gtk_box_pack_start (GTK_BOX (hbox1), current_login_combo, FALSE, TRUE, 0);
  gtk_combo_set_value_in_list (GTK_COMBO (current_login_combo), TRUE, FALSE);

  current_login_ent = GTK_COMBO (current_login_combo)->entry;
  gtk_widget_ref (current_login_ent);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "current_login_ent", current_login_ent,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (current_login_ent);
  gtk_widget_set_usize (current_login_ent, 240, -2);
  gtk_entry_set_editable (GTK_ENTRY (current_login_ent), FALSE);

  statusbar2 = gtk_statusbar_new ();
  gtk_widget_ref (statusbar2);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "statusbar2", statusbar2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (statusbar2);
  gtk_box_pack_start (GTK_BOX (hbox1), statusbar2, TRUE, TRUE, 0);

  pixmap1 = create_pixmap (main_window, NULL);
  gtk_widget_ref (pixmap1);
  gtk_object_set_data_full (GTK_OBJECT (main_window), "pixmap1", pixmap1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap1);
  gtk_box_pack_start (GTK_BOX (hbox1), pixmap1, FALSE, FALSE, 0);

  gtk_signal_connect (GTK_OBJECT (main_window), "delete_event",
                      GTK_SIGNAL_FUNC (on_main_window_delete_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (main_window), "map",
                      GTK_SIGNAL_FUNC (on_main_window_map),
                      main_window);
  gtk_signal_connect (GTK_OBJECT (menuopen), "activate",
                      GTK_SIGNAL_FUNC (on_menuopen_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (menusave), "activate",
                      GTK_SIGNAL_FUNC (on_menusave_activate),
                      main_window);
  gtk_signal_connect (GTK_OBJECT (menusaveas), "activate",
                      GTK_SIGNAL_FUNC (on_menusaveas_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (add_connection), "activate",
                      GTK_SIGNAL_FUNC (on_add_connection_activate),
                      main_window);
  gtk_signal_connect (GTK_OBJECT (save_connecion), "activate",
                      GTK_SIGNAL_FUNC (on_save_connecion_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (close_connection), "activate",
                      GTK_SIGNAL_FUNC (on_close_connection_activate),
                      main_window);
  gtk_signal_connect (GTK_OBJECT (menuexit), "activate",
                      GTK_SIGNAL_FUNC (on_menuexit_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (menucut), "activate",
                      GTK_SIGNAL_FUNC (on_menucut_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (menucopy), "activate",
                      GTK_SIGNAL_FUNC (on_menucopy_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (menupaste), "activate",
                      GTK_SIGNAL_FUNC (on_menupaste_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (menuselectall), "activate",
                      GTK_SIGNAL_FUNC (on_menuselectall_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (menuclear), "activate",
                      GTK_SIGNAL_FUNC (on_menuclear_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (menuprefs), "activate",
                      GTK_SIGNAL_FUNC (on_menuprefs_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (add_bookmark_default), "activate",
                      GTK_SIGNAL_FUNC (on_default_add_bookm_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (organise_bookmark), "activate",
                      GTK_SIGNAL_FUNC (on_organise_bookmark_activate),
                      main_window);
  gtk_signal_connect (GTK_OBJECT (help), "activate",
                      GTK_SIGNAL_FUNC (on_menuhelp_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (about), "activate",
                      GTK_SIGNAL_FUNC (on_menuabout_activate),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (new_query_tbb), "clicked",
                      GTK_SIGNAL_FUNC (on_new_query_tbb_clicked),
                      main_window);
  gtk_signal_connect (GTK_OBJECT (open_query_tbb), "clicked",
                      GTK_SIGNAL_FUNC (on_open_query_tbb_clicked),
                      main_window);
  gtk_signal_connect (GTK_OBJECT (save_query_tbb), "clicked",
                      GTK_SIGNAL_FUNC (on_save_query_tbb_clicked),
                      main_window);
  gtk_signal_connect (GTK_OBJECT (print_query_tbb), "clicked",
                      GTK_SIGNAL_FUNC (on_print_query_tbb_clicked),
                      main_window);
  gtk_signal_connect (GTK_OBJECT (exec_tbb), "clicked",
                      GTK_SIGNAL_FUNC (on_exec_tbb_clicked),
                      notebook1);
  gtk_signal_connect (GTK_OBJECT (stop_tbb), "clicked",
                      GTK_SIGNAL_FUNC (on_stop_tbb_clicked),
                      notebook1);
  gtk_signal_connect (GTK_OBJECT (print_output_tbb), "clicked",
                      GTK_SIGNAL_FUNC (on_print_output_tbb_clicked),
                      notebook1);
  gtk_signal_connect (GTK_OBJECT (export_tbb), "clicked",
                      GTK_SIGNAL_FUNC (on_export_tbb_clicked),
                      notebook1);
  gtk_signal_connect (GTK_OBJECT (current_login_ent), "changed",
                      GTK_SIGNAL_FUNC (on_current_login_ent_changed),
                      notebook1);

  gtk_window_add_accel_group (GTK_WINDOW (main_window), accel_group);

  return main_window;
}

GtkWidget*
create_login_window (void)
{
  GtkWidget *login_window;
  GtkWidget *vbox8;
  GtkWidget *hbox9;
  GtkWidget *vbox9;
  GtkWidget *pixmap4;
  GtkWidget *vbox11;
  GtkWidget *pixmap6;
  GtkWidget *dbtype_combo;
  GtkWidget *dbtype_entry;
  GtkWidget *vbox10;
  GtkWidget *pixmap5;
  GtkWidget *login_auth_nb;
  GtkWidget *Oracle;
  GtkWidget *ora_label12;
  GtkWidget *ora_hbox3;
  GtkWidget *ora_vseparator1;
  GtkWidget *ora_login_username;
  GtkWidget *ora_vseparator2;
  GtkWidget *ora_label13;
  GtkWidget *ora_hbox4;
  GtkWidget *ora_vseparator4;
  GtkWidget *ora_login_passwd;
  GtkWidget *ora_vseparator3;
  GtkWidget *ora_label14;
  GtkWidget *ora_hbox5;
  GtkWidget *ora_vseparator5;
  GtkWidget *ora_login_tnsname;
  GtkWidget *ora_vseparator6;
  GtkWidget *label23;
  GtkWidget *MySql;
  GtkWidget *mysql_label12;
  GtkWidget *mysql_hbox3;
  GtkWidget *mysql_vseparator1;
  GtkWidget *mysql_login_username;
  GtkWidget *mysql_vseparator2;
  GtkWidget *mysql_label13;
  GtkWidget *mysql_hbox4;
  GtkWidget *mysql_vseparator4;
  GtkWidget *mysql_login_passwd;
  GtkWidget *mysql_vseparator3;
  GtkWidget *mysql_label14;
  GtkWidget *mysql_hbox5;
  GtkWidget *mysql_vseparator5;
  GtkWidget *mysql_login_hostname;
  GtkWidget *mysql_vseparator6;
  GtkWidget *my_sqllabel26;
  GtkWidget *mysql_hbox10;
  GtkWidget *vseparator1;
  GtkWidget *mysql_login_database;
  GtkWidget *entry1;
  GtkWidget *vseparator2;
  GtkWidget *label24;
  GtkWidget *hbuttonbox4;
  GtkWidget *login_ok;
  GtkWidget *login_cancel;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  login_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_object_set_data (GTK_OBJECT (login_window), "login_window", login_window);
  gtk_window_set_title (GTK_WINDOW (login_window), _("SQLWork 5.0 Login"));
  gtk_window_set_modal (GTK_WINDOW (login_window), TRUE);
  gtk_window_set_policy (GTK_WINDOW (login_window), FALSE, FALSE, FALSE);

  vbox8 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox8);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "vbox8", vbox8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox8);
  gtk_container_add (GTK_CONTAINER (login_window), vbox8);

  hbox9 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox9);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "hbox9", hbox9,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox9);
  gtk_box_pack_start (GTK_BOX (vbox8), hbox9, TRUE, TRUE, 0);

  vbox9 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox9);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "vbox9", vbox9,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox9);
  gtk_box_pack_start (GTK_BOX (hbox9), vbox9, TRUE, TRUE, 0);

  pixmap4 = create_pixmap (login_window, "logintopleft.xpm");
  gtk_widget_ref (pixmap4);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "pixmap4", pixmap4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap4);
  gtk_box_pack_start (GTK_BOX (vbox9), pixmap4, FALSE, FALSE, 0);
  gtk_misc_set_alignment (GTK_MISC (pixmap4), 0, 0);

  vbox11 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox11);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "vbox11", vbox11,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox11);
  gtk_box_pack_start (GTK_BOX (hbox9), vbox11, TRUE, TRUE, 0);

  pixmap6 = create_pixmap (login_window, "logintopmid.xpm");
  gtk_widget_ref (pixmap6);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "pixmap6", pixmap6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap6);
  gtk_box_pack_start (GTK_BOX (vbox11), pixmap6, TRUE, TRUE, 0);
  gtk_misc_set_alignment (GTK_MISC (pixmap6), 0, 0);

  dbtype_combo = gtk_combo_new ();
  gtk_widget_ref (dbtype_combo);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "dbtype_combo", dbtype_combo,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (dbtype_combo);
  gtk_box_pack_start (GTK_BOX (vbox11), dbtype_combo, FALSE, FALSE, 0);
  gtk_combo_set_value_in_list (GTK_COMBO (dbtype_combo), TRUE, FALSE);

  dbtype_entry = GTK_COMBO (dbtype_combo)->entry;
  gtk_widget_ref (dbtype_entry);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "dbtype_entry", dbtype_entry,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (dbtype_entry);
  gtk_tooltips_set_tip (tooltips, dbtype_entry, _("Selet the database type you wish to connect to"), NULL);
  gtk_entry_set_editable (GTK_ENTRY (dbtype_entry), FALSE);

  vbox10 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox10);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "vbox10", vbox10,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox10);
  gtk_box_pack_start (GTK_BOX (hbox9), vbox10, TRUE, TRUE, 0);

  pixmap5 = create_pixmap (login_window, "logintopright.xpm");
  gtk_widget_ref (pixmap5);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "pixmap5", pixmap5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap5);
  gtk_box_pack_start (GTK_BOX (vbox10), pixmap5, FALSE, FALSE, 0);
  gtk_misc_set_alignment (GTK_MISC (pixmap5), 0, 7.45058e-09);

  login_auth_nb = gtk_notebook_new ();
  gtk_widget_ref (login_auth_nb);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "login_auth_nb", login_auth_nb,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (login_auth_nb);
  gtk_box_pack_start (GTK_BOX (vbox8), login_auth_nb, TRUE, TRUE, 0);
  GTK_WIDGET_UNSET_FLAGS (login_auth_nb, GTK_CAN_FOCUS);
  gtk_notebook_set_show_tabs (GTK_NOTEBOOK (login_auth_nb), FALSE);
  gtk_notebook_set_show_border (GTK_NOTEBOOK (login_auth_nb), FALSE);

  Oracle = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (Oracle);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "Oracle", Oracle,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (Oracle);
  gtk_container_add (GTK_CONTAINER (login_auth_nb), Oracle);

  ora_label12 = gtk_label_new (_("Username"));
  gtk_widget_ref (ora_label12);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "ora_label12", ora_label12,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ora_label12);
  gtk_box_pack_start (GTK_BOX (Oracle), ora_label12, FALSE, FALSE, 0);

  ora_hbox3 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (ora_hbox3);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "ora_hbox3", ora_hbox3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ora_hbox3);
  gtk_box_pack_start (GTK_BOX (Oracle), ora_hbox3, FALSE, FALSE, 0);

  ora_vseparator1 = gtk_vseparator_new ();
  gtk_widget_ref (ora_vseparator1);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "ora_vseparator1", ora_vseparator1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (ora_hbox3), ora_vseparator1, FALSE, FALSE, 0);
  gtk_widget_set_sensitive (ora_vseparator1, FALSE);

  ora_login_username = gtk_entry_new ();
  gtk_widget_ref (ora_login_username);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "ora_login_username", ora_login_username,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ora_login_username);
  gtk_box_pack_start (GTK_BOX (ora_hbox3), ora_login_username, TRUE, FALSE, 0);
  gtk_widget_set_usize (ora_login_username, 240, -2);

  ora_vseparator2 = gtk_vseparator_new ();
  gtk_widget_ref (ora_vseparator2);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "ora_vseparator2", ora_vseparator2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (ora_hbox3), ora_vseparator2, FALSE, FALSE, 0);
  gtk_widget_set_sensitive (ora_vseparator2, FALSE);

  ora_label13 = gtk_label_new (_("Password"));
  gtk_widget_ref (ora_label13);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "ora_label13", ora_label13,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ora_label13);
  gtk_box_pack_start (GTK_BOX (Oracle), ora_label13, FALSE, FALSE, 0);

  ora_hbox4 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (ora_hbox4);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "ora_hbox4", ora_hbox4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ora_hbox4);
  gtk_box_pack_start (GTK_BOX (Oracle), ora_hbox4, FALSE, FALSE, 0);

  ora_vseparator4 = gtk_vseparator_new ();
  gtk_widget_ref (ora_vseparator4);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "ora_vseparator4", ora_vseparator4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (ora_hbox4), ora_vseparator4, FALSE, FALSE, 0);
  gtk_widget_set_sensitive (ora_vseparator4, FALSE);

  ora_login_passwd = gtk_entry_new ();
  gtk_widget_ref (ora_login_passwd);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "ora_login_passwd", ora_login_passwd,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ora_login_passwd);
  gtk_box_pack_start (GTK_BOX (ora_hbox4), ora_login_passwd, TRUE, FALSE, 0);
  gtk_widget_set_usize (ora_login_passwd, 240, -2);
  gtk_entry_set_visibility (GTK_ENTRY (ora_login_passwd), FALSE);

  ora_vseparator3 = gtk_vseparator_new ();
  gtk_widget_ref (ora_vseparator3);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "ora_vseparator3", ora_vseparator3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (ora_hbox4), ora_vseparator3, FALSE, FALSE, 0);
  gtk_widget_set_sensitive (ora_vseparator3, FALSE);

  ora_label14 = gtk_label_new (_("TNS Name"));
  gtk_widget_ref (ora_label14);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "ora_label14", ora_label14,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ora_label14);
  gtk_box_pack_start (GTK_BOX (Oracle), ora_label14, FALSE, FALSE, 0);

  ora_hbox5 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (ora_hbox5);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "ora_hbox5", ora_hbox5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ora_hbox5);
  gtk_box_pack_start (GTK_BOX (Oracle), ora_hbox5, FALSE, FALSE, 0);

  ora_vseparator5 = gtk_vseparator_new ();
  gtk_widget_ref (ora_vseparator5);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "ora_vseparator5", ora_vseparator5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (ora_hbox5), ora_vseparator5, FALSE, FALSE, 0);
  gtk_widget_set_sensitive (ora_vseparator5, FALSE);

  ora_login_tnsname = gtk_entry_new ();
  gtk_widget_ref (ora_login_tnsname);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "ora_login_tnsname", ora_login_tnsname,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ora_login_tnsname);
  gtk_box_pack_start (GTK_BOX (ora_hbox5), ora_login_tnsname, TRUE, FALSE, 0);
  gtk_widget_set_usize (ora_login_tnsname, 240, -2);

  ora_vseparator6 = gtk_vseparator_new ();
  gtk_widget_ref (ora_vseparator6);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "ora_vseparator6", ora_vseparator6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (ora_hbox5), ora_vseparator6, FALSE, FALSE, 0);
  gtk_widget_set_sensitive (ora_vseparator6, FALSE);

  label23 = gtk_label_new (_("label23"));
  gtk_widget_ref (label23);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "label23", label23,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label23);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (login_auth_nb), gtk_notebook_get_nth_page (GTK_NOTEBOOK (login_auth_nb), 0), label23);

  MySql = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (MySql);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "MySql", MySql,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (MySql);
  gtk_container_add (GTK_CONTAINER (login_auth_nb), MySql);

  mysql_label12 = gtk_label_new (_("Username"));
  gtk_widget_ref (mysql_label12);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "mysql_label12", mysql_label12,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (mysql_label12);
  gtk_box_pack_start (GTK_BOX (MySql), mysql_label12, FALSE, FALSE, 0);

  mysql_hbox3 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (mysql_hbox3);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "mysql_hbox3", mysql_hbox3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (mysql_hbox3);
  gtk_box_pack_start (GTK_BOX (MySql), mysql_hbox3, FALSE, FALSE, 0);

  mysql_vseparator1 = gtk_vseparator_new ();
  gtk_widget_ref (mysql_vseparator1);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "mysql_vseparator1", mysql_vseparator1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (mysql_hbox3), mysql_vseparator1, FALSE, FALSE, 0);
  gtk_widget_set_sensitive (mysql_vseparator1, FALSE);

  mysql_login_username = gtk_entry_new ();
  gtk_widget_ref (mysql_login_username);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "mysql_login_username", mysql_login_username,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (mysql_login_username);
  gtk_box_pack_start (GTK_BOX (mysql_hbox3), mysql_login_username, TRUE, FALSE, 0);
  gtk_widget_set_usize (mysql_login_username, 240, -2);

  mysql_vseparator2 = gtk_vseparator_new ();
  gtk_widget_ref (mysql_vseparator2);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "mysql_vseparator2", mysql_vseparator2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (mysql_hbox3), mysql_vseparator2, FALSE, FALSE, 0);
  gtk_widget_set_sensitive (mysql_vseparator2, FALSE);

  mysql_label13 = gtk_label_new (_("Password"));
  gtk_widget_ref (mysql_label13);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "mysql_label13", mysql_label13,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (mysql_label13);
  gtk_box_pack_start (GTK_BOX (MySql), mysql_label13, FALSE, FALSE, 0);

  mysql_hbox4 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (mysql_hbox4);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "mysql_hbox4", mysql_hbox4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (mysql_hbox4);
  gtk_box_pack_start (GTK_BOX (MySql), mysql_hbox4, FALSE, FALSE, 0);

  mysql_vseparator4 = gtk_vseparator_new ();
  gtk_widget_ref (mysql_vseparator4);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "mysql_vseparator4", mysql_vseparator4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (mysql_hbox4), mysql_vseparator4, FALSE, FALSE, 0);
  gtk_widget_set_sensitive (mysql_vseparator4, FALSE);

  mysql_login_passwd = gtk_entry_new ();
  gtk_widget_ref (mysql_login_passwd);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "mysql_login_passwd", mysql_login_passwd,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (mysql_login_passwd);
  gtk_box_pack_start (GTK_BOX (mysql_hbox4), mysql_login_passwd, TRUE, FALSE, 0);
  gtk_widget_set_usize (mysql_login_passwd, 240, -2);
  gtk_entry_set_visibility (GTK_ENTRY (mysql_login_passwd), FALSE);

  mysql_vseparator3 = gtk_vseparator_new ();
  gtk_widget_ref (mysql_vseparator3);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "mysql_vseparator3", mysql_vseparator3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (mysql_hbox4), mysql_vseparator3, FALSE, FALSE, 0);
  gtk_widget_set_sensitive (mysql_vseparator3, FALSE);

  mysql_label14 = gtk_label_new (_("Host Name"));
  gtk_widget_ref (mysql_label14);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "mysql_label14", mysql_label14,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (mysql_label14);
  gtk_box_pack_start (GTK_BOX (MySql), mysql_label14, FALSE, FALSE, 0);

  mysql_hbox5 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (mysql_hbox5);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "mysql_hbox5", mysql_hbox5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (mysql_hbox5);
  gtk_box_pack_start (GTK_BOX (MySql), mysql_hbox5, FALSE, FALSE, 0);

  mysql_vseparator5 = gtk_vseparator_new ();
  gtk_widget_ref (mysql_vseparator5);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "mysql_vseparator5", mysql_vseparator5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (mysql_hbox5), mysql_vseparator5, FALSE, FALSE, 0);
  gtk_widget_set_sensitive (mysql_vseparator5, FALSE);

  mysql_login_hostname = gtk_entry_new ();
  gtk_widget_ref (mysql_login_hostname);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "mysql_login_hostname", mysql_login_hostname,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (mysql_login_hostname);
  gtk_box_pack_start (GTK_BOX (mysql_hbox5), mysql_login_hostname, TRUE, FALSE, 0);
  gtk_widget_set_usize (mysql_login_hostname, 240, -2);

  mysql_vseparator6 = gtk_vseparator_new ();
  gtk_widget_ref (mysql_vseparator6);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "mysql_vseparator6", mysql_vseparator6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (mysql_hbox5), mysql_vseparator6, FALSE, FALSE, 0);
  gtk_widget_set_sensitive (mysql_vseparator6, FALSE);

  my_sqllabel26 = gtk_label_new (_("Database"));
  gtk_widget_ref (my_sqllabel26);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "my_sqllabel26", my_sqllabel26,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (my_sqllabel26);
  gtk_box_pack_start (GTK_BOX (MySql), my_sqllabel26, FALSE, FALSE, 0);

  mysql_hbox10 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (mysql_hbox10);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "mysql_hbox10", mysql_hbox10,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (mysql_hbox10);
  gtk_box_pack_start (GTK_BOX (MySql), mysql_hbox10, FALSE, FALSE, 0);

  vseparator1 = gtk_vseparator_new ();
  gtk_widget_ref (vseparator1);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "vseparator1", vseparator1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (mysql_hbox10), vseparator1, FALSE, FALSE, 0);

  mysql_login_database = gtk_combo_new ();
  gtk_widget_ref (mysql_login_database);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "mysql_login_database", mysql_login_database,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (mysql_login_database);
  gtk_box_pack_start (GTK_BOX (mysql_hbox10), mysql_login_database, TRUE, FALSE, 0);
  gtk_widget_set_usize (mysql_login_database, 240, -2);
  GTK_WIDGET_SET_FLAGS (mysql_login_database, GTK_CAN_FOCUS);
  gtk_combo_set_use_arrows_always (GTK_COMBO (mysql_login_database), TRUE);

  entry1 = GTK_COMBO (mysql_login_database)->entry;
  gtk_widget_ref (entry1);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "entry1", entry1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry1);

  vseparator2 = gtk_vseparator_new ();
  gtk_widget_ref (vseparator2);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "vseparator2", vseparator2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_box_pack_start (GTK_BOX (mysql_hbox10), vseparator2, FALSE, FALSE, 0);

  label24 = gtk_label_new (_("label24"));
  gtk_widget_ref (label24);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "label24", label24,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label24);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (login_auth_nb), gtk_notebook_get_nth_page (GTK_NOTEBOOK (login_auth_nb), 1), label24);

  hbuttonbox4 = gtk_hbutton_box_new ();
  gtk_widget_ref (hbuttonbox4);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "hbuttonbox4", hbuttonbox4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbuttonbox4);
  gtk_box_pack_start (GTK_BOX (vbox8), hbuttonbox4, TRUE, TRUE, 0);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox4), GTK_BUTTONBOX_SPREAD);

  login_ok = gtk_button_new_with_label (_("Login"));
  gtk_widget_ref (login_ok);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "login_ok", login_ok,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (login_ok);
  gtk_container_add (GTK_CONTAINER (hbuttonbox4), login_ok);
  GTK_WIDGET_SET_FLAGS (login_ok, GTK_CAN_DEFAULT);

  login_cancel = gtk_button_new_with_pixmap (login_window, "gnome-cancel.xpm", _("Cancel"));
  gtk_widget_ref (login_cancel);
  gtk_object_set_data_full (GTK_OBJECT (login_window), "login_cancel", login_cancel,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (login_cancel);
  gtk_container_add (GTK_CONTAINER (hbuttonbox4), login_cancel);
  GTK_WIDGET_SET_FLAGS (login_cancel, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (login_window), "delete_event",
                      GTK_SIGNAL_FUNC (on_login_window_delete_event),
                      login_window);
  gtk_signal_connect (GTK_OBJECT (dbtype_entry), "changed",
                      GTK_SIGNAL_FUNC (on_dbtype_entry_changed),
                      login_window);
#ifdef MYSQL_DB
  gtk_signal_connect (GTK_OBJECT (mysql_login_username), "focus_out_event",
                      GTK_SIGNAL_FUNC (on_mysql_login_entry_focus_out_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (mysql_login_passwd), "focus_out_event",
                      GTK_SIGNAL_FUNC (on_mysql_login_entry_focus_out_event),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (mysql_login_hostname), "focus_out_event",
                      GTK_SIGNAL_FUNC (on_mysql_login_entry_focus_out_event),
                      NULL);
#endif
  gtk_signal_connect (GTK_OBJECT (login_ok), "clicked",
                      GTK_SIGNAL_FUNC (on_login_ok_clicked),
                      login_window);
  gtk_signal_connect (GTK_OBJECT (login_cancel), "clicked",
                      GTK_SIGNAL_FUNC (on_login_cancel_clicked),
                      login_window);

  gtk_widget_grab_default (login_ok);
  gtk_object_set_data (GTK_OBJECT (login_window), "tooltips", tooltips);

  return login_window;
}

/*
 */
GtkWidget*
create_work_pane (void)
{
  GtkWidget *work_pane;
  GtkWidget *results_notebook;
  GtkWidget *hbox6;
  GtkWidget *scrolledwindow2;
  GtkWidget *text_view;
  GtkWidget *vscrollbar1;
  GtkWidget *label7;
  GtkWidget *grid_frame;
  GtkWidget *label8;
  GtkWidget *plan_frame;
  GtkWidget *label9;
  GtkWidget *scrolledwindow1;
  GtkWidget *sql_query;

  work_pane = gtk_vpaned_new ();
  gtk_widget_show (work_pane);

  results_notebook = gtk_notebook_new ();
  gtk_widget_ref (results_notebook);
  gtk_object_set_data_full (GTK_OBJECT (work_pane), "results_notebook", results_notebook,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (results_notebook);
  gtk_paned_pack1 (GTK_PANED (work_pane), results_notebook, FALSE, TRUE);
  gtk_widget_set_usize (results_notebook, -2, 240);

  hbox6 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox6);
  gtk_object_set_data_full (GTK_OBJECT (work_pane), "hbox6", hbox6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox6);
  gtk_container_add (GTK_CONTAINER (results_notebook), hbox6);

  scrolledwindow2 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow2);
  gtk_object_set_data_full (GTK_OBJECT (work_pane), "scrolledwindow2", scrolledwindow2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow2);
  gtk_box_pack_start (GTK_BOX (hbox6), scrolledwindow2, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow2), GTK_POLICY_ALWAYS, GTK_POLICY_NEVER);

  text_view = gtk_text_new (NULL, NULL);
  gtk_widget_ref (text_view);
  gtk_object_set_data_full (GTK_OBJECT (work_pane), "text_view", text_view,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_text_set_word_wrap(GTK_TEXT(text_view), FALSE);
  gtk_text_set_line_wrap(GTK_TEXT(text_view), FALSE);
  gtk_widget_show (text_view);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolledwindow2), text_view);

  vscrollbar1 = gtk_vscrollbar_new(GTK_TEXT(text_view)->vadj);
  gtk_widget_ref (vscrollbar1);
  gtk_object_set_data_full (GTK_OBJECT (work_pane), "vscrollbar1", vscrollbar1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vscrollbar1);
  gtk_box_pack_start (GTK_BOX (hbox6), vscrollbar1, FALSE, FALSE, 0);

  label7 = gtk_label_new (_("Text View"));
  gtk_widget_ref (label7);
  gtk_object_set_data_full (GTK_OBJECT (work_pane), "label7", label7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label7);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (results_notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (results_notebook), 0), label7);

  grid_frame = gtk_frame_new (NULL);
  gtk_widget_ref (grid_frame);
  gtk_object_set_data_full (GTK_OBJECT (work_pane), "grid_frame", grid_frame,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (grid_frame);
  gtk_container_add (GTK_CONTAINER (results_notebook), grid_frame);
  gtk_frame_set_shadow_type (GTK_FRAME (grid_frame), GTK_SHADOW_NONE);

  label8 = gtk_label_new (_("Grid View"));
  gtk_widget_ref (label8);
  gtk_object_set_data_full (GTK_OBJECT (work_pane), "label8", label8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label8);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (results_notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (results_notebook), 1), label8);

  plan_frame = gtk_frame_new (NULL);
  gtk_widget_ref (plan_frame);
  gtk_object_set_data_full (GTK_OBJECT (work_pane), "plan_frame", plan_frame,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (plan_frame);
  gtk_container_add (GTK_CONTAINER (results_notebook), plan_frame);
  gtk_frame_set_shadow_type (GTK_FRAME (plan_frame), GTK_SHADOW_NONE);

  label9 = gtk_label_new (_("Query Plan"));
  gtk_widget_ref (label9);
  gtk_object_set_data_full (GTK_OBJECT (work_pane), "label9", label9,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label9);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (results_notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (results_notebook), 2), label9);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow1);
  gtk_object_set_data_full (GTK_OBJECT (work_pane), "scrolledwindow1", scrolledwindow1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow1);
  gtk_paned_pack2 (GTK_PANED (work_pane), scrolledwindow1, TRUE, TRUE);
  gtk_widget_set_usize (scrolledwindow1, -2, 160);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

  sql_query = gtk_text_new (NULL, NULL);
  gtk_widget_ref (sql_query);
  gtk_object_set_data_full (GTK_OBJECT (work_pane), "sql_query", sql_query,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (sql_query);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), sql_query);
  gtk_text_set_editable (GTK_TEXT (sql_query), TRUE);

  gtk_signal_connect (GTK_OBJECT (results_notebook), "switch_page",
                      GTK_SIGNAL_FUNC (on_results_notebook_switch_page),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (sql_query), "activate",
                      GTK_SIGNAL_FUNC (on_sql_query_activate),
                      NULL);

  return work_pane;
}

GtkWidget*
create_prefs_window (void)
{
  GtkWidget *prefs_window;
  GtkWidget *dialog_vbox1;
  GtkWidget *notebook2;
  GtkWidget *vbox4;
  GtkWidget *frame5;
  GtkWidget *hbox8;
  GSList *pref_tb_rbgp_group = NULL;
  GtkWidget *prefs_tb_hide;
  GtkWidget *prefs_tb_icons;
  GtkWidget *prefs_tb_text;
  GtkWidget *prefs_tb_both;
  GtkWidget *frame4;
  GtkWidget *fixed5;
  GtkWidget *prefs_export_sep;
  GtkWidget *label16;
  GtkWidget *frame7;
  GtkWidget *fixed6;
  GtkWidget *prefs_save;
  GtkWidget *label20;
  GtkWidget *vbox5;
  GtkWidget *frame3;
  GtkWidget *fixed4;
  GtkWidget *prefs_max_rows;
  GtkWidget *prefs_q_commit;
  GtkWidget *label1;
  GtkWidget *label21;
  GtkWidget *vbox6;
  GtkWidget *frame1;
  GtkWidget *fixed2;
  GtkWidget *prefs_textv_font;
  GtkWidget *prefs_sel_font;
  GtkObject *prefs_text_colwid_adj;
  GtkWidget *prefs_text_colwid;
  GtkWidget *label17;
  GtkWidget *prefs_textv_upd;
  GtkWidget *label18;
  GtkWidget *frame2;
  GtkWidget *fixed3;
  GtkWidget *prefs_gridv_autos;
  GtkWidget *prefs_gridv_upd;
  GtkWidget *label22;
  GtkWidget *vbox12;
  GtkWidget *frame8;
  GtkWidget *fixed7;
  GtkWidget *sel_icon_path;
  GtkWidget *button2;
  GtkWidget *prefs_icon_path;
  GtkWidget *prefs_bm_path;
  GtkWidget *label26;
  GtkWidget *label27;
  GtkWidget *label24;
  GtkWidget *vbox13;
  GtkWidget *scrolledwindow3;
  GtkWidget *prefs_saved_conn_list;
  GtkWidget *label29;
  GtkWidget *label30;
  GtkWidget *hbuttonbox5;
  GtkWidget *prefs_add_conn;
  GtkWidget *prefs_edit_conn;
  GtkWidget *prefs_delete_conn;
  GtkWidget *label28;
  GtkWidget *vbox21;
  GtkWidget *hbox19;
  GtkWidget *label40;
  GtkWidget *prefs_psize;
  GList *prefs_psize_items = NULL;
  GtkWidget *prefs_psize_ent;
  GtkWidget *hbox20;
  GtkWidget *label42;
  GtkWidget *prefs_font;
  GList *prefs_font_items = NULL;
  GtkWidget *prefs_font_ent;
  GtkWidget *label41;
  GtkWidget *prefs_fsize;
  GList *prefs_fsize_items = NULL;
  GtkWidget *prefs_fsize_ent;
  GtkWidget *label39;
  GtkWidget *dialog_action_area1;
  GtkWidget *hbuttonbox2;
  GtkWidget *prefs_ok;
  GtkWidget *prefs_apply;
  GtkWidget *prefs_cancel;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  prefs_window = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (prefs_window), "prefs_window", prefs_window);
  gtk_window_set_title (GTK_WINDOW (prefs_window), _("SQLWork 5.0 Preferences"));
  gtk_window_set_policy (GTK_WINDOW (prefs_window), FALSE, FALSE, FALSE);

  dialog_vbox1 = GTK_DIALOG (prefs_window)->vbox;
  gtk_object_set_data (GTK_OBJECT (prefs_window), "dialog_vbox1", dialog_vbox1);
  gtk_widget_show (dialog_vbox1);

  notebook2 = gtk_notebook_new ();
  gtk_widget_ref (notebook2);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "notebook2", notebook2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (notebook2);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), notebook2, TRUE, TRUE, 0);

  vbox4 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox4);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "vbox4", vbox4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox4);
  gtk_container_add (GTK_CONTAINER (notebook2), vbox4);

  frame5 = gtk_frame_new (_("Toolbar"));
  gtk_widget_ref (frame5);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "frame5", frame5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame5);
  gtk_box_pack_start (GTK_BOX (vbox4), frame5, FALSE, FALSE, 0);

  hbox8 = gtk_hbox_new (FALSE, 2);
  gtk_widget_ref (hbox8);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "hbox8", hbox8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox8);
  gtk_container_add (GTK_CONTAINER (frame5), hbox8);

  prefs_tb_hide = gtk_radio_button_new_with_label (pref_tb_rbgp_group, _("Hide"));
  pref_tb_rbgp_group = gtk_radio_button_group (GTK_RADIO_BUTTON (prefs_tb_hide));
  gtk_widget_ref (prefs_tb_hide);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_tb_hide", prefs_tb_hide,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_tb_hide);
  gtk_box_pack_start (GTK_BOX (hbox8), prefs_tb_hide, TRUE, FALSE, 0);

  prefs_tb_icons = gtk_radio_button_new_with_label (pref_tb_rbgp_group, _("Icons"));
  pref_tb_rbgp_group = gtk_radio_button_group (GTK_RADIO_BUTTON (prefs_tb_icons));
  gtk_widget_ref (prefs_tb_icons);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_tb_icons", prefs_tb_icons,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_tb_icons);
  gtk_box_pack_start (GTK_BOX (hbox8), prefs_tb_icons, TRUE, FALSE, 0);

  prefs_tb_text = gtk_radio_button_new_with_label (pref_tb_rbgp_group, _("Text"));
  pref_tb_rbgp_group = gtk_radio_button_group (GTK_RADIO_BUTTON (prefs_tb_text));
  gtk_widget_ref (prefs_tb_text);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_tb_text", prefs_tb_text,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_tb_text);
  gtk_box_pack_start (GTK_BOX (hbox8), prefs_tb_text, TRUE, FALSE, 0);

  prefs_tb_both = gtk_radio_button_new_with_label (pref_tb_rbgp_group, _("Both"));
  pref_tb_rbgp_group = gtk_radio_button_group (GTK_RADIO_BUTTON (prefs_tb_both));
  gtk_widget_ref (prefs_tb_both);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_tb_both", prefs_tb_both,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_tb_both);
  gtk_box_pack_start (GTK_BOX (hbox8), prefs_tb_both, TRUE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (prefs_tb_both), TRUE);

  frame4 = gtk_frame_new (_("Export"));
  gtk_widget_ref (frame4);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "frame4", frame4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame4);
  gtk_box_pack_start (GTK_BOX (vbox4), frame4, FALSE, FALSE, 0);
  gtk_widget_set_usize (frame4, 448, 64);

  fixed5 = gtk_fixed_new ();
  gtk_widget_ref (fixed5);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "fixed5", fixed5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (fixed5);
  gtk_container_add (GTK_CONTAINER (frame4), fixed5);

  prefs_export_sep = gtk_entry_new ();
  gtk_widget_ref (prefs_export_sep);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_export_sep", prefs_export_sep,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_export_sep);
  gtk_fixed_put (GTK_FIXED (fixed5), prefs_export_sep, -1, -1);
  gtk_widget_set_uposition (prefs_export_sep, 256, 0);
  gtk_widget_set_usize (prefs_export_sep, 96, 32);
  gtk_tooltips_set_tip (tooltips, prefs_export_sep, _("Separator character for \"export delimited\""), NULL);
  gtk_entry_set_text (GTK_ENTRY (prefs_export_sep), _("|"));

  label16 = gtk_label_new (_("Separator character"));
  gtk_widget_ref (label16);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "label16", label16,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label16);
  gtk_fixed_put (GTK_FIXED (fixed5), label16, -1, -1);
  gtk_widget_set_uposition (label16, 8, 0);
  gtk_widget_set_usize (label16, 224, 32);
  gtk_misc_set_alignment (GTK_MISC (label16), 7.45058e-09, 0.5);

  frame7 = gtk_frame_new (_("Preferences"));
  gtk_widget_ref (frame7);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "frame7", frame7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame7);
  gtk_box_pack_start (GTK_BOX (vbox4), frame7, TRUE, TRUE, 0);

  fixed6 = gtk_fixed_new ();
  gtk_widget_ref (fixed6);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "fixed6", fixed6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (fixed6);
  gtk_container_add (GTK_CONTAINER (frame7), fixed6);

  prefs_save = gtk_check_button_new_with_label (_("Save preferences on exit"));
  gtk_widget_ref (prefs_save);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_save", prefs_save,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_save);
  gtk_fixed_put (GTK_FIXED (fixed6), prefs_save, -1, -1);
  gtk_widget_set_uposition (prefs_save, 8, 8);
  gtk_widget_set_usize (prefs_save, 216, 40);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (prefs_save), TRUE);

  label20 = gtk_label_new (_("General"));
  gtk_widget_ref (label20);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "label20", label20,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label20);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook2), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook2), 0), label20);

  vbox5 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox5);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "vbox5", vbox5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox5);
  gtk_container_add (GTK_CONTAINER (notebook2), vbox5);

  frame3 = gtk_frame_new (_("Query"));
  gtk_widget_ref (frame3);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "frame3", frame3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame3);
  gtk_box_pack_start (GTK_BOX (vbox5), frame3, TRUE, TRUE, 0);
  gtk_widget_set_usize (frame3, 448, 120);

  fixed4 = gtk_fixed_new ();
  gtk_widget_ref (fixed4);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "fixed4", fixed4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (fixed4);
  gtk_container_add (GTK_CONTAINER (frame3), fixed4);

  prefs_max_rows = gtk_entry_new ();
  gtk_widget_ref (prefs_max_rows);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_max_rows", prefs_max_rows,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_max_rows);
  gtk_fixed_put (GTK_FIXED (fixed4), prefs_max_rows, -1, -1);
  gtk_widget_set_uposition (prefs_max_rows, 256, 0);
  gtk_widget_set_usize (prefs_max_rows, 96, 32);
  gtk_tooltips_set_tip (tooltips, prefs_max_rows, _("The maximum number of rows returned from the database. 0 or \"Unlimited\" means return all the data"), NULL);
  gtk_entry_set_text (GTK_ENTRY (prefs_max_rows), _("Unlimited"));

  prefs_q_commit = gtk_check_button_new_with_label (_("Commit work after each query"));
  gtk_widget_ref (prefs_q_commit);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_q_commit", prefs_q_commit,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_q_commit);
  gtk_fixed_put (GTK_FIXED (fixed4), prefs_q_commit, -1, -1);
  gtk_widget_set_uposition (prefs_q_commit, 8, 48);
  gtk_widget_set_usize (prefs_q_commit, 256, 32);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (prefs_q_commit), TRUE);

  label1 = gtk_label_new (_("Max number of rows to return"));
  gtk_widget_ref (label1);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "label1", label1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label1);
  gtk_fixed_put (GTK_FIXED (fixed4), label1, -1, -1);
  gtk_widget_set_uposition (label1, 8, 0);
  gtk_widget_set_usize (label1, 232, 32);
  gtk_misc_set_alignment (GTK_MISC (label1), 7.45058e-09, 0.5);

  label21 = gtk_label_new (_("Query"));
  gtk_widget_ref (label21);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "label21", label21,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label21);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook2), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook2), 1), label21);

  vbox6 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox6);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "vbox6", vbox6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox6);
  gtk_container_add (GTK_CONTAINER (notebook2), vbox6);

  frame1 = gtk_frame_new (_("Text View"));
  gtk_widget_ref (frame1);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "frame1", frame1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame1);
  gtk_box_pack_start (GTK_BOX (vbox6), frame1, TRUE, TRUE, 0);
  gtk_widget_set_usize (frame1, 448, 132);

  fixed2 = gtk_fixed_new ();
  gtk_widget_ref (fixed2);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "fixed2", fixed2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (fixed2);
  gtk_container_add (GTK_CONTAINER (frame1), fixed2);

  prefs_textv_font = gtk_entry_new ();
  gtk_widget_ref (prefs_textv_font);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_textv_font", prefs_textv_font,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_textv_font);
  gtk_fixed_put (GTK_FIXED (fixed2), prefs_textv_font, -1, -1);
  gtk_widget_set_uposition (prefs_textv_font, 56, 8);
  gtk_widget_set_usize (prefs_textv_font, 312, 32);
  gtk_tooltips_set_tip (tooltips, prefs_textv_font, _("The font which will be used for the text view. Monospaced fonts are best"), NULL);
  gtk_entry_set_editable (GTK_ENTRY (prefs_textv_font), FALSE);

  prefs_sel_font = gtk_button_new_with_label (_("Select"));
  gtk_widget_ref (prefs_sel_font);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_sel_font", prefs_sel_font,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_sel_font);
  gtk_fixed_put (GTK_FIXED (fixed2), prefs_sel_font, -1, -1);
  gtk_widget_set_uposition (prefs_sel_font, 376, 8);
  gtk_widget_set_usize (prefs_sel_font, 59, 32);

  prefs_text_colwid_adj = gtk_adjustment_new (32, 8, 64, 8, 16, 16);
  prefs_text_colwid = gtk_spin_button_new (GTK_ADJUSTMENT (prefs_text_colwid_adj), 1, 0);
  gtk_widget_ref (prefs_text_colwid);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_text_colwid", prefs_text_colwid,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_text_colwid);
  gtk_fixed_put (GTK_FIXED (fixed2), prefs_text_colwid, -1, -1);
  gtk_widget_set_uposition (prefs_text_colwid, 384, 56);
  gtk_widget_set_usize (prefs_text_colwid, 48, 32);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (prefs_text_colwid), TRUE);
  gtk_spin_button_set_snap_to_ticks (GTK_SPIN_BUTTON (prefs_text_colwid), TRUE);

  label17 = gtk_label_new (_("Font"));
  gtk_widget_ref (label17);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "label17", label17,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label17);
  gtk_fixed_put (GTK_FIXED (fixed2), label17, -1, -1);
  gtk_widget_set_uposition (label17, 8, 8);
  gtk_widget_set_usize (label17, 48, 32);
  gtk_misc_set_alignment (GTK_MISC (label17), 7.45058e-09, 0.5);

  prefs_textv_upd = gtk_check_button_new_with_label (_("Dynamically Update"));
  gtk_widget_ref (prefs_textv_upd);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_textv_upd", prefs_textv_upd,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_textv_upd);
  gtk_fixed_put (GTK_FIXED (fixed2), prefs_textv_upd, -1, -1);
  gtk_widget_set_uposition (prefs_textv_upd, 8, 56);
  gtk_widget_set_usize (prefs_textv_upd, 176, 32);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (prefs_textv_upd), TRUE);

  label18 = gtk_label_new (_("Max column width"));
  gtk_widget_ref (label18);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "label18", label18,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label18);
  gtk_fixed_put (GTK_FIXED (fixed2), label18, -1, -1);
  gtk_widget_set_uposition (label18, 240, 56);
  gtk_widget_set_usize (label18, 144, 32);
  gtk_misc_set_alignment (GTK_MISC (label18), 7.45058e-09, 0.5);

  frame2 = gtk_frame_new (_("Grid View"));
  gtk_widget_ref (frame2);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "frame2", frame2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame2);
  gtk_box_pack_start (GTK_BOX (vbox6), frame2, TRUE, TRUE, 0);
  gtk_widget_set_usize (frame2, 448, 72);

  fixed3 = gtk_fixed_new ();
  gtk_widget_ref (fixed3);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "fixed3", fixed3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (fixed3);
  gtk_container_add (GTK_CONTAINER (frame2), fixed3);

  prefs_gridv_autos = gtk_check_button_new_with_label (_("Auto Size Columns"));
  gtk_widget_ref (prefs_gridv_autos);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_gridv_autos", prefs_gridv_autos,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_gridv_autos);
  gtk_fixed_put (GTK_FIXED (fixed3), prefs_gridv_autos, -1, -1);
  gtk_widget_set_uposition (prefs_gridv_autos, 264, 8);
  gtk_widget_set_usize (prefs_gridv_autos, 176, 32);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (prefs_gridv_autos), TRUE);

  prefs_gridv_upd = gtk_check_button_new_with_label (_("Dynamically Update"));
  gtk_widget_ref (prefs_gridv_upd);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_gridv_upd", prefs_gridv_upd,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_gridv_upd);
  gtk_fixed_put (GTK_FIXED (fixed3), prefs_gridv_upd, -1, -1);
  gtk_widget_set_uposition (prefs_gridv_upd, 8, 8);
  gtk_widget_set_usize (prefs_gridv_upd, 192, 32);

  label22 = gtk_label_new (_("Data Views"));
  gtk_widget_ref (label22);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "label22", label22,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label22);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook2), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook2), 2), label22);

  vbox12 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox12);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "vbox12", vbox12,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox12);
  gtk_container_add (GTK_CONTAINER (notebook2), vbox12);

  frame8 = gtk_frame_new (_("Paths"));
  gtk_widget_ref (frame8);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "frame8", frame8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame8);
  gtk_box_pack_start (GTK_BOX (vbox12), frame8, TRUE, TRUE, 0);

  fixed7 = gtk_fixed_new ();
  gtk_widget_ref (fixed7);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "fixed7", fixed7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (fixed7);
  gtk_container_add (GTK_CONTAINER (frame8), fixed7);

  sel_icon_path = gtk_button_new_with_label (_("Select"));
  gtk_widget_ref (sel_icon_path);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "sel_icon_path", sel_icon_path,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (sel_icon_path);
  gtk_fixed_put (GTK_FIXED (fixed7), sel_icon_path, -1, -1);
  gtk_widget_set_uposition (sel_icon_path, 376, 8);
  gtk_widget_set_usize (sel_icon_path, 59, 32);

  button2 = gtk_button_new_with_label (_("Select"));
  gtk_widget_ref (button2);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "button2", button2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (button2);
  gtk_fixed_put (GTK_FIXED (fixed7), button2, -1, -1);
  gtk_widget_set_uposition (button2, 376, 72);
  gtk_widget_set_usize (button2, 59, 32);

  prefs_icon_path = gtk_entry_new ();
  gtk_widget_ref (prefs_icon_path);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_icon_path", prefs_icon_path,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_icon_path);
  gtk_fixed_put (GTK_FIXED (fixed7), prefs_icon_path, -1, -1);
  gtk_widget_set_uposition (prefs_icon_path, 96, 8);
  gtk_widget_set_usize (prefs_icon_path, 272, 32);

  prefs_bm_path = gtk_entry_new ();
  gtk_widget_ref (prefs_bm_path);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_bm_path", prefs_bm_path,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_bm_path);
  gtk_fixed_put (GTK_FIXED (fixed7), prefs_bm_path, -1, -1);
  gtk_widget_set_uposition (prefs_bm_path, 96, 72);
  gtk_widget_set_usize (prefs_bm_path, 272, 32);

  label26 = gtk_label_new (_("Icons"));
  gtk_widget_ref (label26);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "label26", label26,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label26);
  gtk_fixed_put (GTK_FIXED (fixed7), label26, -1, -1);
  gtk_widget_set_uposition (label26, 8, 8);
  gtk_widget_set_usize (label26, 64, 32);
  gtk_misc_set_alignment (GTK_MISC (label26), 0, 0.5);

  label27 = gtk_label_new (_("BookMarks"));
  gtk_widget_ref (label27);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "label27", label27,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label27);
  gtk_fixed_put (GTK_FIXED (fixed7), label27, -1, -1);
  gtk_widget_set_uposition (label27, 8, 72);
  gtk_widget_set_usize (label27, 88, 32);
  gtk_misc_set_alignment (GTK_MISC (label27), 7.45058e-09, 0.5);

  label24 = gtk_label_new (_("Paths"));
  gtk_widget_ref (label24);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "label24", label24,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label24);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook2), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook2), 3), label24);

  vbox13 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox13);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "vbox13", vbox13,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox13);
  gtk_container_add (GTK_CONTAINER (notebook2), vbox13);

  scrolledwindow3 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow3);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "scrolledwindow3", scrolledwindow3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow3);
  gtk_box_pack_start (GTK_BOX (vbox13), scrolledwindow3, TRUE, TRUE, 0);

  prefs_saved_conn_list = gtk_clist_new (2);
  gtk_widget_ref (prefs_saved_conn_list);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_saved_conn_list", prefs_saved_conn_list,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_saved_conn_list);
  gtk_container_add (GTK_CONTAINER (scrolledwindow3), prefs_saved_conn_list);
  gtk_tooltips_set_tip (tooltips, prefs_saved_conn_list, _("List of connections which will be made when SQLWork starts"), NULL);
  gtk_clist_set_column_width (GTK_CLIST (prefs_saved_conn_list), 0, 80);
  gtk_clist_set_column_width (GTK_CLIST (prefs_saved_conn_list), 1, 80);
  gtk_clist_column_titles_show (GTK_CLIST (prefs_saved_conn_list));

  label29 = gtk_label_new (_("Database"));
  gtk_widget_ref (label29);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "label29", label29,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label29);
  gtk_clist_set_column_widget (GTK_CLIST (prefs_saved_conn_list), 0, label29);

  label30 = gtk_label_new (_("Connection String"));
  gtk_widget_ref (label30);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "label30", label30,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label30);
  gtk_clist_set_column_widget (GTK_CLIST (prefs_saved_conn_list), 1, label30);

  hbuttonbox5 = gtk_hbutton_box_new ();
  gtk_widget_ref (hbuttonbox5);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "hbuttonbox5", hbuttonbox5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbuttonbox5);
  gtk_box_pack_start (GTK_BOX (vbox13), hbuttonbox5, FALSE, FALSE, 0);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox5), GTK_BUTTONBOX_SPREAD);
  gtk_button_box_set_child_ipadding (GTK_BUTTON_BOX (hbuttonbox5), 0, 0);

  prefs_add_conn = gtk_button_new_with_label (_("Add"));
  gtk_widget_ref (prefs_add_conn);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_add_conn", prefs_add_conn,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_add_conn);
  gtk_container_add (GTK_CONTAINER (hbuttonbox5), prefs_add_conn);
  GTK_WIDGET_SET_FLAGS (prefs_add_conn, GTK_CAN_DEFAULT);

  prefs_edit_conn = gtk_button_new_with_label (_("Edit"));
  gtk_widget_ref (prefs_edit_conn);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_edit_conn", prefs_edit_conn,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_edit_conn);
  gtk_container_add (GTK_CONTAINER (hbuttonbox5), prefs_edit_conn);
  GTK_WIDGET_SET_FLAGS (prefs_edit_conn, GTK_CAN_DEFAULT);

  prefs_delete_conn = gtk_button_new_with_label (_("Remove"));
  gtk_widget_ref (prefs_delete_conn);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_delete_conn", prefs_delete_conn,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_delete_conn);
  gtk_container_add (GTK_CONTAINER (hbuttonbox5), prefs_delete_conn);
  GTK_WIDGET_SET_FLAGS (prefs_delete_conn, GTK_CAN_DEFAULT);

  label28 = gtk_label_new (_("Connections"));
  gtk_widget_ref (label28);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "label28", label28,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label28);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook2), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook2), 4), label28);

  vbox21 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox21);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "vbox21", vbox21,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox21);
  gtk_container_add (GTK_CONTAINER (notebook2), vbox21);

  hbox19 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox19);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "hbox19", hbox19,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox19);
  gtk_box_pack_start (GTK_BOX (vbox21), hbox19, FALSE, FALSE, 0);

  label40 = gtk_label_new (_("Paper Size:"));
  gtk_widget_ref (label40);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "label40", label40,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label40);
  gtk_box_pack_start (GTK_BOX (hbox19), label40, FALSE, FALSE, 0);

  prefs_psize = gtk_combo_new ();
  gtk_widget_ref (prefs_psize);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_psize", prefs_psize,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_psize);
  gtk_box_pack_start (GTK_BOX (hbox19), prefs_psize, TRUE, TRUE, 0);
  gtk_combo_set_value_in_list (GTK_COMBO (prefs_psize), TRUE, FALSE);
  prefs_psize_items = g_list_append (prefs_psize_items, _("11x17"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("a0"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("a1"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("a2"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("a3"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("a4"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("a5"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("archA"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("archB"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("archC"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("archD"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("archE"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("b0"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("b1"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("b2"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("b3"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("b4"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("b5"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("executive"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("flsa"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("flse"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("halfletter"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("ledger"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("legal"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("letter"));
  prefs_psize_items = g_list_append (prefs_psize_items, _("note"));
  gtk_combo_set_popdown_strings (GTK_COMBO (prefs_psize), prefs_psize_items);
  g_list_free (prefs_psize_items);

  prefs_psize_ent = GTK_COMBO (prefs_psize)->entry;
  gtk_widget_ref (prefs_psize_ent);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_psize_ent", prefs_psize_ent,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_psize_ent);
  gtk_entry_set_text (GTK_ENTRY (prefs_psize_ent), _("a4"));

  hbox20 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox20);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "hbox20", hbox20,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox20);
  gtk_box_pack_start (GTK_BOX (vbox21), hbox20, TRUE, TRUE, 0);

  label42 = gtk_label_new (_("Default Font:"));
  gtk_widget_ref (label42);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "label42", label42,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label42);
  gtk_box_pack_start (GTK_BOX (hbox20), label42, FALSE, FALSE, 0);

  prefs_font = gtk_combo_new ();
  gtk_widget_ref (prefs_font);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_font", prefs_font,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_font);
  gtk_box_pack_start (GTK_BOX (hbox20), prefs_font, TRUE, TRUE, 0);
  prefs_font_items = g_list_append (prefs_font_items, _("Helvetica"));
  prefs_font_items = g_list_append (prefs_font_items, _("Times Roman"));
  prefs_font_items = g_list_append (prefs_font_items, _("Courier"));
  gtk_combo_set_popdown_strings (GTK_COMBO (prefs_font), prefs_font_items);
  g_list_free (prefs_font_items);

  prefs_font_ent = GTK_COMBO (prefs_font)->entry;
  gtk_widget_ref (prefs_font_ent);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_font_ent", prefs_font_ent,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_font_ent);
  gtk_entry_set_text (GTK_ENTRY (prefs_font_ent), _("Helvetica"));

  label41 = gtk_label_new (_("Size"));
  gtk_widget_ref (label41);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "label41", label41,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label41);
  gtk_box_pack_start (GTK_BOX (hbox20), label41, FALSE, FALSE, 0);

  prefs_fsize = gtk_combo_new ();
  gtk_widget_ref (prefs_fsize);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_fsize", prefs_fsize,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_fsize);
  gtk_box_pack_start (GTK_BOX (hbox20), prefs_fsize, FALSE, FALSE, 0);
  gtk_combo_set_value_in_list (GTK_COMBO (prefs_fsize), TRUE, FALSE);
  prefs_fsize_items = g_list_append (prefs_fsize_items, _("8pt"));
  prefs_fsize_items = g_list_append (prefs_fsize_items, _("9pt"));
  prefs_fsize_items = g_list_append (prefs_fsize_items, _("10pt"));
  prefs_fsize_items = g_list_append (prefs_fsize_items, _("11pt"));
  prefs_fsize_items = g_list_append (prefs_fsize_items, _("12pt"));
  prefs_fsize_items = g_list_append (prefs_fsize_items, _("14pt"));
  prefs_fsize_items = g_list_append (prefs_fsize_items, _("16pt"));
  prefs_fsize_items = g_list_append (prefs_fsize_items, _("18pt"));
  gtk_combo_set_popdown_strings (GTK_COMBO (prefs_fsize), prefs_fsize_items);
  g_list_free (prefs_fsize_items);

  prefs_fsize_ent = GTK_COMBO (prefs_fsize)->entry;
  gtk_widget_ref (prefs_fsize_ent);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_fsize_ent", prefs_fsize_ent,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_fsize_ent);
  gtk_entry_set_text (GTK_ENTRY (prefs_fsize_ent), _("12pt"));

  label39 = gtk_label_new (_("Postscript"));
  gtk_widget_ref (label39);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "label39", label39,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label39);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook2), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook2), 5), label39);

  dialog_action_area1 = GTK_DIALOG (prefs_window)->action_area;
  gtk_object_set_data (GTK_OBJECT (prefs_window), "dialog_action_area1", dialog_action_area1);
  gtk_widget_show (dialog_action_area1);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area1), 10);

  hbuttonbox2 = gtk_hbutton_box_new ();
  gtk_widget_ref (hbuttonbox2);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "hbuttonbox2", hbuttonbox2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbuttonbox2);
  gtk_box_pack_start (GTK_BOX (dialog_action_area1), hbuttonbox2, TRUE, TRUE, 0);

  prefs_ok = gtk_button_new_with_pixmap (prefs_window, "gnome-ok.xpm", _("OK"));
  gtk_widget_ref (prefs_ok);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_ok", prefs_ok,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_ok);
  gtk_container_add (GTK_CONTAINER (hbuttonbox2), prefs_ok);
  GTK_WIDGET_SET_FLAGS (prefs_ok, GTK_CAN_DEFAULT);

  prefs_apply = gtk_button_new_with_pixmap (prefs_window, "gnome-apply.xpm", _("Apply"));
  gtk_widget_ref (prefs_apply);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_apply", prefs_apply,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_apply);
  gtk_container_add (GTK_CONTAINER (hbuttonbox2), prefs_apply);
  GTK_WIDGET_SET_FLAGS (prefs_apply, GTK_CAN_DEFAULT);

  prefs_cancel = gtk_button_new_with_pixmap (prefs_window, "gnome-cancel.xpm", _("Cancel"));
  gtk_widget_ref (prefs_cancel);
  gtk_object_set_data_full (GTK_OBJECT (prefs_window), "prefs_cancel", prefs_cancel,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (prefs_cancel);
  gtk_container_add (GTK_CONTAINER (hbuttonbox2), prefs_cancel);
  GTK_WIDGET_SET_FLAGS (prefs_cancel, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (prefs_window), "delete_event",
                      GTK_SIGNAL_FUNC (on_dialog_delete_event),
                      prefs_window);
  gtk_signal_connect (GTK_OBJECT (prefs_sel_font), "clicked",
                      GTK_SIGNAL_FUNC (on_prefs_sel_font_clicked),
                      prefs_window);
  gtk_signal_connect (GTK_OBJECT (sel_icon_path), "clicked",
                      GTK_SIGNAL_FUNC (on_prefs_sel_iconpath_clicked),
                      prefs_window);
  gtk_signal_connect (GTK_OBJECT (button2), "clicked",
                      GTK_SIGNAL_FUNC (on_prefs_sel_bmpath_clicked),
                      prefs_window);
  gtk_signal_connect (GTK_OBJECT (prefs_add_conn), "clicked",
                      GTK_SIGNAL_FUNC (on_prefs_add_conn_clicked),
                      prefs_saved_conn_list);
  gtk_signal_connect (GTK_OBJECT (prefs_edit_conn), "clicked",
                      GTK_SIGNAL_FUNC (on_prefs_edit_conn_clicked),
                      prefs_saved_conn_list);
  gtk_signal_connect (GTK_OBJECT (prefs_delete_conn), "clicked",
                      GTK_SIGNAL_FUNC (on_prefs_delete_conn_clicked),
                      prefs_saved_conn_list);
  gtk_signal_connect (GTK_OBJECT (prefs_ok), "clicked",
                      GTK_SIGNAL_FUNC (on_prefs_ok_clicked),
                      prefs_window);
  gtk_signal_connect (GTK_OBJECT (prefs_apply), "clicked",
                      GTK_SIGNAL_FUNC (on_prefs_apply_clicked),
                      prefs_window);
  gtk_signal_connect (GTK_OBJECT (prefs_cancel), "clicked",
                      GTK_SIGNAL_FUNC (on_dialog_cancel_clicked),
                      prefs_window);

  gtk_object_set_data (GTK_OBJECT (prefs_window), "tooltips", tooltips);

  return prefs_window;
}

GtkWidget*
create_fontselectiondialog (void)
{
  GtkWidget *fontselectiondialog;
  GtkWidget *fontsel_ok_button;
  GtkWidget *fontsel_cancel_button;
  GtkWidget *fontsel_apply_button;

  fontselectiondialog = gtk_font_selection_dialog_new (_("Select Font"));
  gtk_object_set_data (GTK_OBJECT (fontselectiondialog), "fontselectiondialog", fontselectiondialog);
  gtk_container_set_border_width (GTK_CONTAINER (fontselectiondialog), 4);
  gtk_window_set_policy (GTK_WINDOW (fontselectiondialog), FALSE, TRUE, TRUE);

  fontsel_ok_button = GTK_FONT_SELECTION_DIALOG (fontselectiondialog)->ok_button;
  gtk_object_set_data (GTK_OBJECT (fontselectiondialog), "fontsel_ok_button", fontsel_ok_button);
  gtk_widget_show (fontsel_ok_button);
  GTK_WIDGET_SET_FLAGS (fontsel_ok_button, GTK_CAN_DEFAULT);

  fontsel_cancel_button = GTK_FONT_SELECTION_DIALOG (fontselectiondialog)->cancel_button;
  gtk_object_set_data (GTK_OBJECT (fontselectiondialog), "fontsel_cancel_button", fontsel_cancel_button);
  gtk_widget_show (fontsel_cancel_button);
  GTK_WIDGET_SET_FLAGS (fontsel_cancel_button, GTK_CAN_DEFAULT);

  fontsel_apply_button = GTK_FONT_SELECTION_DIALOG (fontselectiondialog)->apply_button;
  gtk_object_set_data (GTK_OBJECT (fontselectiondialog), "fontsel_apply_button", fontsel_apply_button);
  gtk_widget_show (fontsel_apply_button);
  GTK_WIDGET_SET_FLAGS (fontsel_apply_button, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (fontsel_ok_button), "clicked",
                      GTK_SIGNAL_FUNC (on_fontsel_ok_button_clicked),
                      fontselectiondialog);
  gtk_signal_connect (GTK_OBJECT (fontsel_cancel_button), "clicked",
                      GTK_SIGNAL_FUNC (on_fontsel_cancel_button_clicked),
                      fontselectiondialog);
  gtk_signal_connect (GTK_OBJECT (fontsel_apply_button), "clicked",
                      GTK_SIGNAL_FUNC (on_fontsel_apply_button_clicked),
                      fontselectiondialog);

  return fontselectiondialog;
}

GtkWidget*
create_export_fileselector (void)
{
  GSList *export_format_group = NULL;
  GtkWidget *cancel_button2;
  GtkWidget *export_csv;
  GtkWidget *export_delimit;
  GtkWidget *export_html;
  GtkWidget *export_text;
  GtkWidget *export_sql;
  GtkWidget *export_fileselector;
  GtkWidget *frame2;
  GtkWidget *hbox1;
  GtkWidget *hbox2;
  GtkWidget *vbox1;
  GtkWidget *ok_button2;
  GtkWidget *label1;
  GtkWidget *export_tablename;
  GtkWidget *export_table_create;

  export_fileselector = gtk_file_selection_new (_("Select File"));
  gtk_object_set_data (GTK_OBJECT (export_fileselector), "export_fileselector", export_fileselector);
  gtk_container_set_border_width (GTK_CONTAINER (export_fileselector), 10);

  ok_button2 = GTK_FILE_SELECTION (export_fileselector)->ok_button;
  gtk_button_set_pixmap(ok_button2, "gnome-ok.xpm", _("OK"));
  gtk_object_set_data (GTK_OBJECT (export_fileselector), "ok_button2", ok_button2);
  gtk_widget_show (ok_button2);
  GTK_WIDGET_SET_FLAGS (ok_button2, GTK_CAN_DEFAULT);

  cancel_button2 = GTK_FILE_SELECTION (export_fileselector)->cancel_button;
  gtk_button_set_pixmap(cancel_button2, "gnome-cancel.xpm", _("Cancel"));
  gtk_object_set_data (GTK_OBJECT (export_fileselector), "cancel_button2", cancel_button2);
  gtk_widget_show (cancel_button2);
  GTK_WIDGET_SET_FLAGS (cancel_button2, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (export_fileselector), "delete_event",
                      GTK_SIGNAL_FUNC (on_dialog_delete_event),
                      export_fileselector);
  gtk_signal_connect (GTK_OBJECT (ok_button2), "clicked",
                      GTK_SIGNAL_FUNC (on_export_ok_button_clicked),
                      export_fileselector);
  gtk_signal_connect (GTK_OBJECT (cancel_button2), "clicked",
                      GTK_SIGNAL_FUNC (on_dialog_cancel_clicked),
                      export_fileselector);

  frame2 = gtk_frame_new (_("Format"));
  gtk_widget_ref (frame2);
  gtk_object_set_data_full (GTK_OBJECT (export_fileselector), "frame2", frame2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame2);
  gtk_box_pack_end (GTK_BOX(GTK_FILE_SELECTION(export_fileselector)->main_vbox),
		    frame2, TRUE, TRUE, 0);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox1);
  gtk_object_set_data_full (GTK_OBJECT (export_fileselector), "vbox1", vbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (frame2), vbox1);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox1);
  gtk_object_set_data_full (GTK_OBJECT (export_fileselector), "hbox1", hbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox1, TRUE, TRUE, 0);

  export_sql = gtk_radio_button_new_with_label (export_format_group, _("sql"));
  export_format_group = gtk_radio_button_group (GTK_RADIO_BUTTON (export_sql));
  gtk_widget_ref (export_sql);
  gtk_object_set_data_full (GTK_OBJECT (export_fileselector), "export_sql", export_sql,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (export_sql);
  gtk_box_pack_start (GTK_BOX (hbox1), export_sql, TRUE, TRUE, 0);

  export_text = gtk_radio_button_new_with_label (export_format_group, _("text"));
  export_format_group = gtk_radio_button_group (GTK_RADIO_BUTTON (export_text));
  gtk_widget_ref (export_text);
  gtk_object_set_data_full (GTK_OBJECT (export_fileselector), "export_text", export_text,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (export_text);
  gtk_box_pack_start (GTK_BOX (hbox1), export_text, TRUE, TRUE, 0);

  export_html = gtk_radio_button_new_with_label (export_format_group, _("html"));
  export_format_group = gtk_radio_button_group (GTK_RADIO_BUTTON (export_html));
  gtk_widget_ref (export_html);
  gtk_object_set_data_full (GTK_OBJECT (export_fileselector), "export_html", export_html,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (export_html);
  gtk_box_pack_start (GTK_BOX (hbox1), export_html, TRUE, TRUE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (export_html), TRUE);

  export_csv = gtk_radio_button_new_with_label (export_format_group, _("csv"));
  export_format_group = gtk_radio_button_group (GTK_RADIO_BUTTON (export_csv));
  gtk_widget_ref (export_csv);
  gtk_object_set_data_full (GTK_OBJECT (export_fileselector), "export_csv", export_csv,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (export_csv);
  gtk_box_pack_start (GTK_BOX (hbox1), export_csv, TRUE, TRUE, 0);

  export_delimit = gtk_radio_button_new_with_label (export_format_group, _("delimited"));
  export_format_group = gtk_radio_button_group (GTK_RADIO_BUTTON (export_delimit));
  gtk_widget_ref (export_delimit);
  gtk_object_set_data_full (GTK_OBJECT (export_fileselector), "export_delimit", export_delimit,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (export_delimit);
  gtk_box_pack_start (GTK_BOX (hbox1), export_delimit, TRUE, TRUE, 0);

  hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox2);
  gtk_object_set_data_full (GTK_OBJECT (export_fileselector), "hbox2", hbox2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox2, TRUE, TRUE, 0);

  label1 = gtk_label_new (_("Table:"));
  gtk_widget_ref (label1);
  gtk_object_set_data_full (GTK_OBJECT (export_fileselector), "label1", label1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_set_sensitive (label1, FALSE);
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (hbox2), label1, FALSE, FALSE, 0);

  export_tablename = gtk_entry_new ();
  gtk_widget_ref (export_tablename);
  gtk_object_set_data_full (GTK_OBJECT (export_fileselector), "export_tablename", export_tablename,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_set_sensitive (export_tablename, FALSE);
  gtk_widget_show (export_tablename);
  gtk_box_pack_start (GTK_BOX (hbox2), export_tablename, TRUE, TRUE, 0);
  gtk_widget_set_usize (export_tablename, 240, -2);

  export_table_create = gtk_check_button_new_with_label (_("Create"));
  gtk_widget_ref (export_table_create);
  gtk_object_set_data_full (GTK_OBJECT (export_fileselector), "export_table_create", export_table_create,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_set_sensitive (export_table_create, FALSE);
  gtk_widget_show (export_table_create);
  gtk_box_pack_start (GTK_BOX (hbox2), export_table_create, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (export_table_create), FALSE);

  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION(export_fileselector)->selection_entry),
		      "changed",
                      GTK_SIGNAL_FUNC (on_filesel_entry_changed),
                      export_fileselector);

  gtk_signal_connect (GTK_OBJECT (export_sql), "toggled",
                      GTK_SIGNAL_FUNC (on_export_as_sql_toggled),
                      NULL);

  return export_fileselector;
}

GtkWidget*
create_help_window (void)
{
  GtkWidget *help_window;
  GtkWidget *dialog_vbox2;
  GtkWidget *hbox7;
  GtkWidget *pixmap3;
  GtkWidget *dialog_action_area2;
  GtkWidget *hbuttonbox3;
  GtkWidget *help_ok;

  help_window = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (help_window), "help_window", help_window);
  gtk_window_set_title (GTK_WINDOW (help_window), _("About SQLWork"));
  gtk_window_set_policy (GTK_WINDOW (help_window), TRUE, TRUE, FALSE);

  dialog_vbox2 = GTK_DIALOG (help_window)->vbox;
  gtk_object_set_data (GTK_OBJECT (help_window), "dialog_vbox2", dialog_vbox2);
  gtk_widget_show (dialog_vbox2);

  hbox7 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox7);
  gtk_object_set_data_full (GTK_OBJECT (help_window), "hbox7", hbox7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox7);
  gtk_box_pack_start (GTK_BOX (dialog_vbox2), hbox7, TRUE, TRUE, 0);

  pixmap3 = create_pixmap (help_window, "orasoft_banner.xpm");
  gtk_widget_ref (pixmap3);
  gtk_object_set_data_full (GTK_OBJECT (help_window), "pixmap3", pixmap3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap3);
  gtk_box_pack_start (GTK_BOX (hbox7), pixmap3, FALSE, FALSE, 0);

  dialog_action_area2 = GTK_DIALOG (help_window)->action_area;
  gtk_object_set_data (GTK_OBJECT (help_window), "dialog_action_area2", dialog_action_area2);
  gtk_widget_show (dialog_action_area2);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area2), 10);

  hbuttonbox3 = gtk_hbutton_box_new ();
  gtk_widget_ref (hbuttonbox3);
  gtk_object_set_data_full (GTK_OBJECT (help_window), "hbuttonbox3", hbuttonbox3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbuttonbox3);
  gtk_box_pack_start (GTK_BOX (dialog_action_area2), hbuttonbox3, TRUE, TRUE, 0);

  help_ok =  gtk_button_new_with_pixmap (help_window, "gnome-ok.xpm", _("OK"));
  gtk_widget_ref (help_ok);
  gtk_object_set_data_full (GTK_OBJECT (help_window), "help_ok", help_ok,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (help_ok);
  gtk_container_add (GTK_CONTAINER (hbuttonbox3), help_ok);
  GTK_WIDGET_SET_FLAGS (help_ok, GTK_CAN_DEFAULT);

  return help_window;
}

GtkWidget*
create_save_query_fileselector (void)
{
  GtkWidget *save_query_fileselector;
  GtkWidget *ok_button3;
  GtkWidget *cancel_button3;

  save_query_fileselector = gtk_file_selection_new (_("Select File"));
  gtk_object_set_data (GTK_OBJECT (save_query_fileselector), "save_query_fileselector", save_query_fileselector);
  gtk_container_set_border_width (GTK_CONTAINER (save_query_fileselector), 10);

  ok_button3 = GTK_FILE_SELECTION (save_query_fileselector)->ok_button;
  gtk_button_set_pixmap(ok_button3, "gnome-ok.xpm", _("OK"));
  gtk_object_set_data (GTK_OBJECT (save_query_fileselector), "ok_button3", ok_button3);
  gtk_widget_show (ok_button3);
  GTK_WIDGET_SET_FLAGS (ok_button3, GTK_CAN_DEFAULT);

  cancel_button3 = GTK_FILE_SELECTION (save_query_fileselector)->cancel_button;
  gtk_button_set_pixmap(cancel_button3, "gnome-cancel.xpm", _("Cancel"));
  gtk_object_set_data (GTK_OBJECT (save_query_fileselector), "cancel_button3", cancel_button3);
  gtk_widget_show (cancel_button3);
  GTK_WIDGET_SET_FLAGS (cancel_button3, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (ok_button3), "clicked",
                      GTK_SIGNAL_FUNC (on_saveq_ok_button_clicked),
                      save_query_fileselector);
  gtk_signal_connect (GTK_OBJECT (cancel_button3), "clicked",
                      GTK_SIGNAL_FUNC (on_dialog_cancel_clicked),
                      save_query_fileselector);

  return save_query_fileselector;
}

GtkWidget*
create_directory_fileselector (void)
{
  GtkWidget *directory_fileselector;
  GtkWidget *dirsel_ok_button;
  GtkWidget *dirsel_cancel_button;

  directory_fileselector = gtk_file_selection_new (_("Select Directory"));
  gtk_object_set_data (GTK_OBJECT (directory_fileselector), "directory_fileselector", directory_fileselector);
  gtk_container_set_border_width (GTK_CONTAINER (directory_fileselector), 10);
  gtk_file_selection_hide_fileop_buttons (GTK_FILE_SELECTION (directory_fileselector));

  /* zap the file list so that it's only possible to select directories */
  gtk_widget_hide(GTK_WIDGET(GTK_FILE_SELECTION(directory_fileselector)->file_list)->parent);

  dirsel_ok_button = GTK_FILE_SELECTION (directory_fileselector)->ok_button;
  gtk_button_set_pixmap(dirsel_ok_button, "gnome-ok.xpm", _("OK"));
  gtk_object_set_data (GTK_OBJECT (directory_fileselector), "dirsel_ok_button", dirsel_ok_button);
  gtk_widget_show (dirsel_ok_button);
  GTK_WIDGET_SET_FLAGS (dirsel_ok_button, GTK_CAN_DEFAULT);

  dirsel_cancel_button = GTK_FILE_SELECTION (directory_fileselector)->cancel_button;
  gtk_button_set_pixmap(dirsel_cancel_button, "gnome-cancel.xpm", _("Cancel"));
  gtk_object_set_data (GTK_OBJECT (directory_fileselector), "dirsel_cancel_button", dirsel_cancel_button);
  gtk_widget_show (dirsel_cancel_button);
  GTK_WIDGET_SET_FLAGS (dirsel_cancel_button, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (dirsel_ok_button), "clicked",
                      GTK_SIGNAL_FUNC (on_dirsel_ok_button_clicked),
                      directory_fileselector);
  gtk_signal_connect (GTK_OBJECT (dirsel_cancel_button), "clicked",
                      GTK_SIGNAL_FUNC (on_dialog_cancel_clicked),
                      directory_fileselector);

  return directory_fileselector;
}

GtkWidget*
create_bookmark_organiser (void)
{
  GtkWidget *bookmark_organiser;
  GtkWidget *vbox14;
  GtkWidget *scrolledwindow4;
  GtkWidget *bookmark_list;
  GtkWidget *label32;
  GtkWidget *label33;
  GtkWidget *hbuttonbox6;
  GtkWidget *bookm_org_nfolder;
  GtkWidget *bookm_org_edit;
  GtkWidget *bookm_org_delete;
  GtkWidget *bookm_org_done;

  bookmark_organiser = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_object_set_data (GTK_OBJECT (bookmark_organiser), "bookmark_organiser", bookmark_organiser);
  gtk_window_set_title (GTK_WINDOW (bookmark_organiser), _("Organise Bookmarks"));

  vbox14 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox14);
  gtk_object_set_data_full (GTK_OBJECT (bookmark_organiser), "vbox14", vbox14,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox14);
  gtk_container_add (GTK_CONTAINER (bookmark_organiser), vbox14);

  scrolledwindow4 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow4);
  gtk_object_set_data_full (GTK_OBJECT (bookmark_organiser), "scrolledwindow4", scrolledwindow4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow4);
  gtk_box_pack_start (GTK_BOX (vbox14), scrolledwindow4, TRUE, TRUE, 0);
  gtk_widget_set_usize (scrolledwindow4, -2, 240);

  bookmark_list = gtk_ctree_new (2, 0);
  gtk_widget_ref (bookmark_list);
  gtk_object_set_data_full (GTK_OBJECT (bookmark_organiser), "bookmark_list", bookmark_list,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bookmark_list);
  gtk_container_add (GTK_CONTAINER (scrolledwindow4), bookmark_list);
  gtk_clist_set_column_width (GTK_CLIST (bookmark_list), 0, 80);
  gtk_clist_set_column_width (GTK_CLIST (bookmark_list), 1, 80);
  gtk_clist_column_titles_show (GTK_CLIST (bookmark_list));

  label32 = gtk_label_new (_("BookMark"));
  gtk_widget_ref (label32);
  gtk_object_set_data_full (GTK_OBJECT (bookmark_organiser), "label32", label32,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label32);
  gtk_clist_set_column_widget (GTK_CLIST (bookmark_list), 0, label32);

  label33 = gtk_label_new (_("Description"));
  gtk_widget_ref (label33);
  gtk_object_set_data_full (GTK_OBJECT (bookmark_organiser), "label33", label33,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label33);
  gtk_clist_set_column_widget (GTK_CLIST (bookmark_list), 1, label33);

  hbuttonbox6 = gtk_hbutton_box_new ();
  gtk_widget_ref (hbuttonbox6);
  gtk_object_set_data_full (GTK_OBJECT (bookmark_organiser), "hbuttonbox6", hbuttonbox6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbuttonbox6);
  gtk_box_pack_start (GTK_BOX (vbox14), hbuttonbox6, FALSE, FALSE, 0);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox6), GTK_BUTTONBOX_SPREAD);
  gtk_button_box_set_spacing (GTK_BUTTON_BOX (hbuttonbox6), 16);

  bookm_org_nfolder = gtk_button_new_with_label (_("New Folder"));
  gtk_widget_ref (bookm_org_nfolder);
  gtk_object_set_data_full (GTK_OBJECT (bookmark_organiser), "bookm_org_nfolder", bookm_org_nfolder,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bookm_org_nfolder);
  gtk_container_add (GTK_CONTAINER (hbuttonbox6), bookm_org_nfolder);
  GTK_WIDGET_SET_FLAGS (bookm_org_nfolder, GTK_CAN_DEFAULT);

  bookm_org_edit = gtk_button_new_with_label (_("Edit"));
  gtk_widget_ref (bookm_org_edit);
  gtk_object_set_data_full (GTK_OBJECT (bookmark_organiser), "bookm_org_edit", bookm_org_edit,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bookm_org_edit);
  gtk_container_add (GTK_CONTAINER (hbuttonbox6), bookm_org_edit);
  GTK_WIDGET_SET_FLAGS (bookm_org_edit, GTK_CAN_DEFAULT);

  bookm_org_delete = gtk_button_new_with_label (_("Delete"));
  gtk_widget_ref (bookm_org_delete);
  gtk_object_set_data_full (GTK_OBJECT (bookmark_organiser), "bookm_org_delete", bookm_org_delete,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bookm_org_delete);
  gtk_container_add (GTK_CONTAINER (hbuttonbox6), bookm_org_delete);
  GTK_WIDGET_SET_FLAGS (bookm_org_delete, GTK_CAN_DEFAULT);

  bookm_org_done = gtk_button_new_with_label (_("Done"));
  gtk_widget_ref (bookm_org_done);
  gtk_object_set_data_full (GTK_OBJECT (bookmark_organiser), "bookm_org_done", bookm_org_done,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (bookm_org_done);
  gtk_container_add (GTK_CONTAINER (hbuttonbox6), bookm_org_done);
  GTK_WIDGET_SET_FLAGS (bookm_org_done, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (bookmark_organiser), "delete_event",
                      GTK_SIGNAL_FUNC (on_bookmark_organiser_delete_event),
                      bookmark_organiser);
  gtk_signal_connect (GTK_OBJECT (bookm_org_nfolder), "clicked",
                      GTK_SIGNAL_FUNC (on_bookm_org_nfolder_clicked),
                      bookmark_list);
  gtk_signal_connect (GTK_OBJECT (bookm_org_edit), "clicked",
                      GTK_SIGNAL_FUNC (on_bookm_org_edit_clicked),
                      bookmark_list);
  gtk_signal_connect (GTK_OBJECT (bookm_org_delete), "clicked",
                      GTK_SIGNAL_FUNC (on_bookm_org_delete_clicked),
                      bookmark_list);
  gtk_signal_connect (GTK_OBJECT (bookm_org_done), "clicked",
                      GTK_SIGNAL_FUNC (on_bookm_org_done_clicked),
                      bookmark_organiser);

  return bookmark_organiser;
}

GtkWidget*
create_add_bm_dialog (void)
{
  GtkWidget *add_bm_dialog;
  GtkWidget *dialog_vbox3;
  GtkWidget *vbox15;
  GtkWidget *hbox10;
  GtkWidget *label34;
  GtkWidget *add_bm_name;
  GtkWidget *hbox11;
  GtkWidget *label35;
  GtkWidget *add_bm_descr;
  GtkWidget *dialog_action_area3;
  GtkWidget *hbuttonbox7;
  GtkWidget *add_bm_ok;
  GtkWidget *add_bm_cancel;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  add_bm_dialog = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (add_bm_dialog), "add_bm_dialog", add_bm_dialog);
  gtk_window_set_title (GTK_WINDOW (add_bm_dialog), _("Add Bookmark"));
  gtk_window_set_policy (GTK_WINDOW (add_bm_dialog), FALSE, TRUE, TRUE);

  dialog_vbox3 = GTK_DIALOG (add_bm_dialog)->vbox;
  gtk_object_set_data (GTK_OBJECT (add_bm_dialog), "dialog_vbox3", dialog_vbox3);
  gtk_widget_show (dialog_vbox3);

  vbox15 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox15);
  gtk_object_set_data_full (GTK_OBJECT (add_bm_dialog), "vbox15", vbox15,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox15);
  gtk_box_pack_start (GTK_BOX (dialog_vbox3), vbox15, TRUE, TRUE, 0);

  hbox10 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox10);
  gtk_object_set_data_full (GTK_OBJECT (add_bm_dialog), "hbox10", hbox10,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox10);
  gtk_box_pack_start (GTK_BOX (vbox15), hbox10, TRUE, TRUE, 0);

  label34 = gtk_label_new (_("Name"));
  gtk_widget_ref (label34);
  gtk_object_set_data_full (GTK_OBJECT (add_bm_dialog), "label34", label34,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label34);
  gtk_box_pack_start (GTK_BOX (hbox10), label34, FALSE, FALSE, 0);
  gtk_widget_set_usize (label34, 100, -2);
  gtk_misc_set_alignment (GTK_MISC (label34), 1, 0.5);

  add_bm_name = gtk_entry_new ();
  gtk_widget_ref (add_bm_name);
  gtk_object_set_data_full (GTK_OBJECT (add_bm_dialog), "add_bm_name", add_bm_name,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (add_bm_name);
  gtk_box_pack_start (GTK_BOX (hbox10), add_bm_name, TRUE, TRUE, 0);
  gtk_tooltips_set_tip (tooltips, add_bm_name, _("The name of the new bookmark"), NULL);

  hbox11 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox11);
  gtk_object_set_data_full (GTK_OBJECT (add_bm_dialog), "hbox11", hbox11,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox11);
  gtk_box_pack_start (GTK_BOX (vbox15), hbox11, TRUE, TRUE, 0);

  label35 = gtk_label_new (_("Description"));
  gtk_widget_ref (label35);
  gtk_object_set_data_full (GTK_OBJECT (add_bm_dialog), "label35", label35,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label35);
  gtk_box_pack_start (GTK_BOX (hbox11), label35, FALSE, FALSE, 0);
  gtk_widget_set_usize (label35, 100, -2);
  gtk_misc_set_alignment (GTK_MISC (label35), 1, 0.5);

  add_bm_descr = gtk_entry_new ();
  gtk_widget_ref (add_bm_descr);
  gtk_object_set_data_full (GTK_OBJECT (add_bm_dialog), "add_bm_descr", add_bm_descr,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (add_bm_descr);
  gtk_box_pack_start (GTK_BOX (hbox11), add_bm_descr, TRUE, TRUE, 0);
  gtk_tooltips_set_tip (tooltips, add_bm_descr, _("One line description of the bookmark"), NULL);

  dialog_action_area3 = GTK_DIALOG (add_bm_dialog)->action_area;
  gtk_object_set_data (GTK_OBJECT (add_bm_dialog), "dialog_action_area3", dialog_action_area3);
  gtk_widget_show (dialog_action_area3);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area3), 10);

  hbuttonbox7 = gtk_hbutton_box_new ();
  gtk_widget_ref (hbuttonbox7);
  gtk_object_set_data_full (GTK_OBJECT (add_bm_dialog), "hbuttonbox7", hbuttonbox7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbuttonbox7);
  gtk_box_pack_start (GTK_BOX (dialog_action_area3), hbuttonbox7, TRUE, TRUE, 0);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox7), GTK_BUTTONBOX_SPREAD);

  add_bm_ok = gtk_button_new_with_pixmap (add_bm_dialog, "gnome-ok.xpm", _("OK"));
  gtk_widget_ref (add_bm_ok);
  gtk_object_set_data_full (GTK_OBJECT (add_bm_dialog), "add_bm_ok", add_bm_ok,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (add_bm_ok);
  gtk_container_add (GTK_CONTAINER (hbuttonbox7), add_bm_ok);
  GTK_WIDGET_SET_FLAGS (add_bm_ok, GTK_CAN_DEFAULT);

  add_bm_cancel = gtk_button_new_with_pixmap (add_bm_dialog, "gnome-cancel.xpm", _("Cancel"));
  gtk_widget_ref (add_bm_cancel);
  gtk_object_set_data_full (GTK_OBJECT (add_bm_dialog), "add_bm_cancel", add_bm_cancel,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (add_bm_cancel);
  gtk_container_add (GTK_CONTAINER (hbuttonbox7), add_bm_cancel);
  GTK_WIDGET_SET_FLAGS (add_bm_cancel, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (add_bm_dialog), "delete_event",
                      GTK_SIGNAL_FUNC (on_add_bm_dialog_delete_event),
                      add_bm_dialog);
  gtk_signal_connect (GTK_OBJECT (add_bm_ok), "clicked",
                      GTK_SIGNAL_FUNC (on_add_bm_ok_clicked),
                      add_bm_dialog);
  gtk_signal_connect (GTK_OBJECT (add_bm_cancel), "clicked",
                      GTK_SIGNAL_FUNC (on_add_bm_cancel_clicked),
                      add_bm_dialog);

  gtk_object_set_data (GTK_OBJECT (add_bm_dialog), "tooltips", tooltips);

  return add_bm_dialog;
}

GtkWidget*
create_edit_bm_dialog (void)
{
  GtkWidget *edit_bm_dialog;
  GtkWidget *dialog_vbox4;
  GtkWidget *vbox16;
  GtkWidget *hbox12;
  GtkWidget *label36;
  GtkWidget *edit_bm_name;
  GtkWidget *hbox13;
  GtkWidget *label37;
  GtkWidget *edit_bm_descr;
  GtkWidget *scrolledwindow5;
  GtkWidget *edit_bm_text;
  GtkWidget *dialog_action_area4;
  GtkWidget *hbuttonbox8;
  GtkWidget *edit_bm_ok;
  GtkWidget *edit_bm_cancel;

  edit_bm_dialog = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (edit_bm_dialog), "edit_bm_dialog", edit_bm_dialog);
  gtk_window_set_title (GTK_WINDOW (edit_bm_dialog), _("Edit Bookmark"));
  gtk_window_set_policy (GTK_WINDOW (edit_bm_dialog), TRUE, TRUE, FALSE);

  dialog_vbox4 = GTK_DIALOG (edit_bm_dialog)->vbox;
  gtk_object_set_data (GTK_OBJECT (edit_bm_dialog), "dialog_vbox4", dialog_vbox4);
  gtk_widget_show (dialog_vbox4);

  vbox16 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox16);
  gtk_object_set_data_full (GTK_OBJECT (edit_bm_dialog), "vbox16", vbox16,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox16);
  gtk_box_pack_start (GTK_BOX (dialog_vbox4), vbox16, TRUE, TRUE, 0);

  hbox12 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox12);
  gtk_object_set_data_full (GTK_OBJECT (edit_bm_dialog), "hbox12", hbox12,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox12);
  gtk_box_pack_start (GTK_BOX (vbox16), hbox12, FALSE, TRUE, 0);

  label36 = gtk_label_new (_("Name"));
  gtk_widget_ref (label36);
  gtk_object_set_data_full (GTK_OBJECT (edit_bm_dialog), "label36", label36,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label36);
  gtk_box_pack_start (GTK_BOX (hbox12), label36, FALSE, FALSE, 0);
  gtk_widget_set_usize (label36, 100, -2);
  gtk_misc_set_alignment (GTK_MISC (label36), 1, 0.5);

  edit_bm_name = gtk_entry_new ();
  gtk_widget_ref (edit_bm_name);
  gtk_object_set_data_full (GTK_OBJECT (edit_bm_dialog), "edit_bm_name", edit_bm_name,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (edit_bm_name);
  gtk_box_pack_start (GTK_BOX (hbox12), edit_bm_name, TRUE, TRUE, 0);
  gtk_entry_set_editable (GTK_ENTRY (edit_bm_name), FALSE);

  hbox13 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox13);
  gtk_object_set_data_full (GTK_OBJECT (edit_bm_dialog), "hbox13", hbox13,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox13);
  gtk_box_pack_start (GTK_BOX (vbox16), hbox13, FALSE, TRUE, 0);

  label37 = gtk_label_new (_("Desscription"));
  gtk_widget_ref (label37);
  gtk_object_set_data_full (GTK_OBJECT (edit_bm_dialog), "label37", label37,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label37);
  gtk_box_pack_start (GTK_BOX (hbox13), label37, FALSE, FALSE, 0);
  gtk_widget_set_usize (label37, 100, -2);
  gtk_misc_set_alignment (GTK_MISC (label37), 1, 0.5);

  edit_bm_descr = gtk_entry_new ();
  gtk_widget_ref (edit_bm_descr);
  gtk_object_set_data_full (GTK_OBJECT (edit_bm_dialog), "edit_bm_descr", edit_bm_descr,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (edit_bm_descr);
  gtk_box_pack_start (GTK_BOX (hbox13), edit_bm_descr, TRUE, TRUE, 0);

  scrolledwindow5 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow5);
  gtk_object_set_data_full (GTK_OBJECT (edit_bm_dialog), "scrolledwindow5", scrolledwindow5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow5);
  gtk_box_pack_start (GTK_BOX (vbox16), scrolledwindow5, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow5), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

  edit_bm_text = gtk_text_new (NULL, NULL);
  gtk_widget_ref (edit_bm_text);
  gtk_object_set_data_full (GTK_OBJECT (edit_bm_dialog), "edit_bm_text", edit_bm_text,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (edit_bm_text);
  gtk_container_add (GTK_CONTAINER (scrolledwindow5), edit_bm_text);
  gtk_widget_set_usize (edit_bm_text, 400, 200);
  gtk_text_set_editable (GTK_TEXT (edit_bm_text), TRUE);

  dialog_action_area4 = GTK_DIALOG (edit_bm_dialog)->action_area;
  gtk_object_set_data (GTK_OBJECT (edit_bm_dialog), "dialog_action_area4", dialog_action_area4);
  gtk_widget_show (dialog_action_area4);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area4), 10);

  hbuttonbox8 = gtk_hbutton_box_new ();
  gtk_widget_ref (hbuttonbox8);
  gtk_object_set_data_full (GTK_OBJECT (edit_bm_dialog), "hbuttonbox8", hbuttonbox8,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbuttonbox8);
  gtk_box_pack_start (GTK_BOX (dialog_action_area4), hbuttonbox8, TRUE, TRUE, 0);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox8), GTK_BUTTONBOX_SPREAD);

  edit_bm_ok = gtk_button_new_with_pixmap (edit_bm_dialog, "gnome-ok.xpm", _("OK"));
  gtk_widget_ref (edit_bm_ok);
  gtk_object_set_data_full (GTK_OBJECT (edit_bm_dialog), "edit_bm_ok", edit_bm_ok,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (edit_bm_ok);
  gtk_container_add (GTK_CONTAINER (hbuttonbox8), edit_bm_ok);
  GTK_WIDGET_SET_FLAGS (edit_bm_ok, GTK_CAN_DEFAULT);

  edit_bm_cancel = gtk_button_new_with_pixmap (edit_bm_dialog, "gnome-cancel.xpm", _("Cancel"));
  gtk_widget_ref (edit_bm_cancel);
  gtk_object_set_data_full (GTK_OBJECT (edit_bm_dialog), "edit_bm_cancel", edit_bm_cancel,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (edit_bm_cancel);
  gtk_container_add (GTK_CONTAINER (hbuttonbox8), edit_bm_cancel);
  GTK_WIDGET_SET_FLAGS (edit_bm_cancel, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (edit_bm_dialog), "delete_event",
                      GTK_SIGNAL_FUNC (on_edit_bm_dialog_delete_event),
                      edit_bm_dialog);
  gtk_signal_connect (GTK_OBJECT (edit_bm_ok), "clicked",
                      GTK_SIGNAL_FUNC (on_edit_bm_ok_clicked),
                      edit_bm_dialog);
  gtk_signal_connect (GTK_OBJECT (edit_bm_cancel), "clicked",
                      GTK_SIGNAL_FUNC (on_edit_bm_cancel_clicked),
                      edit_bm_dialog);

  return edit_bm_dialog;
}

GtkWidget*
create_create_bmdir_dialog (void)
{
  GtkWidget *create_bmdir_dialog;
  GtkWidget *dialog_vbox5;
  GtkWidget *vbox17;
  GtkWidget *hbox14;
  GtkWidget *pixmap7;
  GtkWidget *label38;
  GtkWidget *create_bm_subfolders_cb;
  GtkWidget *dialog_action_area5;
  GtkWidget *hbuttonbox9;
  GtkWidget *create_bmdir_ok;
  GtkWidget *create_bmdir_cancel;

  create_bmdir_dialog = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (create_bmdir_dialog), "create_bmdir_dialog", create_bmdir_dialog);
  gtk_window_set_title (GTK_WINDOW (create_bmdir_dialog), _("Create Bookmarks Directory?"));
  gtk_window_set_policy (GTK_WINDOW (create_bmdir_dialog), TRUE, TRUE, FALSE);

  dialog_vbox5 = GTK_DIALOG (create_bmdir_dialog)->vbox;
  gtk_object_set_data (GTK_OBJECT (create_bmdir_dialog), "dialog_vbox5", dialog_vbox5);
  gtk_widget_show (dialog_vbox5);

  vbox17 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox17);
  gtk_object_set_data_full (GTK_OBJECT (create_bmdir_dialog), "vbox17", vbox17,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox17);
  gtk_box_pack_start (GTK_BOX (dialog_vbox5), vbox17, TRUE, TRUE, 0);

  hbox14 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox14);
  gtk_object_set_data_full (GTK_OBJECT (create_bmdir_dialog), "hbox14", hbox14,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox14);
  gtk_box_pack_start (GTK_BOX (vbox17), hbox14, TRUE, TRUE, 0);

  pixmap7 = create_pixmap (create_bmdir_dialog, "gnome-question.xpm");
  gtk_widget_ref (pixmap7);
  gtk_object_set_data_full (GTK_OBJECT (create_bmdir_dialog), "pixmap7", pixmap7,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pixmap7);
  gtk_box_pack_start (GTK_BOX (hbox14), pixmap7, FALSE, FALSE, 0);

  label38 = gtk_label_new (_("Bookmarks directory doesn't exist\n- Create?"));
  gtk_widget_ref (label38);
  gtk_object_set_data_full (GTK_OBJECT (create_bmdir_dialog), "label38", label38,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label38);
  gtk_box_pack_start (GTK_BOX (hbox14), label38, TRUE, TRUE, 0);
  gtk_label_set_justify (GTK_LABEL (label38), GTK_JUSTIFY_LEFT);

  create_bm_subfolders_cb = gtk_check_button_new_with_label (_("Also create folder for each database type"));
  gtk_widget_ref (create_bm_subfolders_cb);
  gtk_object_set_data_full (GTK_OBJECT (create_bmdir_dialog), "create_bm_subfolders_cb", create_bm_subfolders_cb,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (create_bm_subfolders_cb);
  gtk_box_pack_start (GTK_BOX (vbox17), create_bm_subfolders_cb, FALSE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (create_bm_subfolders_cb), TRUE);

  dialog_action_area5 = GTK_DIALOG (create_bmdir_dialog)->action_area;
  gtk_object_set_data (GTK_OBJECT (create_bmdir_dialog), "dialog_action_area5", dialog_action_area5);
  gtk_widget_show (dialog_action_area5);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area5), 10);

  hbuttonbox9 = gtk_hbutton_box_new ();
  gtk_widget_ref (hbuttonbox9);
  gtk_object_set_data_full (GTK_OBJECT (create_bmdir_dialog), "hbuttonbox9", hbuttonbox9,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbuttonbox9);
  gtk_box_pack_start (GTK_BOX (dialog_action_area5), hbuttonbox9, TRUE, TRUE, 0);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox9), GTK_BUTTONBOX_SPREAD);

  create_bmdir_ok = gtk_button_new_with_pixmap (create_bmdir_dialog, "gnome-ok.xpm", _("OK"));
  gtk_widget_ref (create_bmdir_ok);
  gtk_object_set_data_full (GTK_OBJECT (create_bmdir_dialog), "create_bmdir_ok", create_bmdir_ok,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (create_bmdir_ok);
  gtk_container_add (GTK_CONTAINER (hbuttonbox9), create_bmdir_ok);
  GTK_WIDGET_SET_FLAGS (create_bmdir_ok, GTK_CAN_DEFAULT);

  create_bmdir_cancel = gtk_button_new_with_pixmap (create_bmdir_dialog, "gnome-cancel.xpm", _("Cancel"));
  gtk_widget_ref (create_bmdir_cancel);
  gtk_object_set_data_full (GTK_OBJECT (create_bmdir_dialog), "create_bmdir_cancel", create_bmdir_cancel,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (create_bmdir_cancel);
  gtk_container_add (GTK_CONTAINER (hbuttonbox9), create_bmdir_cancel);
  GTK_WIDGET_SET_FLAGS (create_bmdir_cancel, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (create_bmdir_dialog), "delete_event",
                      GTK_SIGNAL_FUNC (on_create_bmdir_dialog_delete_event),
                      create_bmdir_dialog);
  gtk_signal_connect (GTK_OBJECT (create_bmdir_ok), "clicked",
                      GTK_SIGNAL_FUNC (on_create_bmdir_ok_clicked),
                      create_bmdir_dialog);
  gtk_signal_connect (GTK_OBJECT (create_bmdir_cancel), "clicked",
                      GTK_SIGNAL_FUNC (on_create_bmdir_cancel_clicked),
                      create_bmdir_dialog);

  return create_bmdir_dialog;
}

GtkWidget*
create_print_dialog (void)
{
  GSList *printfmt_group = NULL;
  GtkWidget *frame9;
  GtkWidget *hbox15;
  GtkWidget *hbox17;
  GtkWidget *hbox18;
  GtkWidget *hbuttonbox10;
  GtkWidget *print_cancel;
  GtkWidget *print_cmd;
  GtkWidget *print_cmd_lab;
  GtkWidget *print_dialog;
  GtkWidget *print_ok;
  GtkWidget *printfmt_html;
  GtkWidget *printfmt_ps;
  GtkWidget *printfmt_text;
  GtkWidget *vbox18;
  GtkWidget *vbox19;
  GtkWidget *vbox20;

  print_dialog = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (print_dialog), "print_dialog", print_dialog);
  gtk_window_set_title (GTK_WINDOW (print_dialog), _("Print Results"));
  gtk_window_set_policy (GTK_WINDOW (print_dialog), TRUE, TRUE, FALSE);

  vbox18 = GTK_DIALOG (print_dialog)->vbox;
  gtk_object_set_data (GTK_OBJECT (print_dialog), "vbox18", vbox18);
  gtk_widget_show (vbox18);

  vbox19 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox19);
  gtk_object_set_data_full (GTK_OBJECT (print_dialog), "vbox19", vbox19,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox19);
  gtk_box_pack_start (GTK_BOX (vbox18), vbox19, TRUE, TRUE, 0);

  vbox20 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox20);
  gtk_object_set_data_full (GTK_OBJECT (print_dialog), "vbox20", vbox20,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox20);
  gtk_box_pack_start (GTK_BOX (vbox19), vbox20, TRUE, TRUE, 0);

  hbox15 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox15);
  gtk_object_set_data_full (GTK_OBJECT (print_dialog), "hbox15", hbox15,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox15);
  gtk_box_pack_start (GTK_BOX (vbox20), hbox15, TRUE, TRUE, 0);

  print_cmd_lab = gtk_label_new (_("Print Command"));
  gtk_widget_ref (print_cmd_lab);
  gtk_object_set_data_full (GTK_OBJECT (print_dialog), "print_cmd_lab", print_cmd_lab,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (print_cmd_lab);
  gtk_box_pack_start (GTK_BOX (hbox15), print_cmd_lab, FALSE, FALSE, 0);
  gtk_widget_set_usize (print_cmd_lab, 140, -2);
  gtk_misc_set_alignment (GTK_MISC (print_cmd_lab), 7.45058e-09, 0.5);
  gtk_misc_set_padding (GTK_MISC (print_cmd_lab), 5, 0);

  print_cmd = gtk_entry_new ();
  gtk_widget_ref (print_cmd);
  gtk_object_set_data_full (GTK_OBJECT (print_dialog), "print_cmd", print_cmd,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (print_cmd);
  gtk_box_pack_start (GTK_BOX (hbox15), print_cmd, TRUE, TRUE, 0);
  gtk_entry_set_text (GTK_ENTRY (print_cmd), _("lpr"));

  frame9 = gtk_frame_new (_("Format"));
  gtk_widget_ref (frame9);
  gtk_object_set_data_full (GTK_OBJECT (print_dialog), "frame9", frame9,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame9);
  gtk_box_pack_start (GTK_BOX (vbox19), frame9, FALSE, TRUE, 0);

  hbox17 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox17);
  gtk_object_set_data_full (GTK_OBJECT (print_dialog), "hbox17", hbox17,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox17);
  gtk_container_add (GTK_CONTAINER (frame9), hbox17);

  printfmt_text = gtk_radio_button_new_with_label (printfmt_group, _("Text"));
  printfmt_group = gtk_radio_button_group (GTK_RADIO_BUTTON (printfmt_text));
  gtk_widget_ref (printfmt_text);
  gtk_object_set_data_full (GTK_OBJECT (print_dialog), "printfmt_text", printfmt_text,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (printfmt_text);
  gtk_box_pack_start (GTK_BOX (hbox17), printfmt_text, TRUE, FALSE, 0);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (printfmt_text), TRUE);

  printfmt_html = gtk_radio_button_new_with_label (printfmt_group, _("Html"));
  printfmt_group = gtk_radio_button_group (GTK_RADIO_BUTTON (printfmt_html));
  gtk_widget_ref (printfmt_html);
  gtk_object_set_data_full (GTK_OBJECT (print_dialog), "printfmt_html", printfmt_html,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (printfmt_html);
  gtk_box_pack_start (GTK_BOX (hbox17), printfmt_html, TRUE, FALSE, 0);

  printfmt_ps = gtk_radio_button_new_with_label (printfmt_group, _("Postscript"));
  printfmt_group = gtk_radio_button_group (GTK_RADIO_BUTTON (printfmt_ps));
  gtk_widget_ref (printfmt_ps);
  gtk_object_set_data_full (GTK_OBJECT (print_dialog), "printfmt_ps", printfmt_ps,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (printfmt_ps);
  gtk_box_pack_start (GTK_BOX (hbox17), printfmt_ps, TRUE, FALSE, 0);

  hbox18 = GTK_DIALOG (print_dialog)->action_area;
  gtk_object_set_data (GTK_OBJECT (print_dialog), "hbox18", hbox18);
  gtk_widget_show (hbox18);
  gtk_container_set_border_width (GTK_CONTAINER (hbox18), 10);

  hbuttonbox10 = gtk_hbutton_box_new ();
  gtk_widget_ref (hbuttonbox10);
  gtk_object_set_data_full (GTK_OBJECT (print_dialog), "hbuttonbox10", hbuttonbox10,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbuttonbox10);
  gtk_box_pack_start (GTK_BOX (hbox18), hbuttonbox10, TRUE, TRUE, 0);

  print_ok = gtk_button_new_with_pixmap (print_dialog, "gnome-ok.xpm", _("Print"));
  gtk_widget_ref (print_ok);
  gtk_object_set_data_full (GTK_OBJECT (print_dialog), "print_ok", print_ok,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (print_ok);
  gtk_container_add (GTK_CONTAINER (hbuttonbox10), print_ok);
  GTK_WIDGET_SET_FLAGS (print_ok, GTK_CAN_DEFAULT);

  print_cancel = gtk_button_new_with_pixmap (print_dialog, "gnome-cancel.xpm", _("Cancel"));
  gtk_widget_ref (print_cancel);
  gtk_object_set_data_full (GTK_OBJECT (print_dialog), "print_cancel", print_cancel,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (print_cancel);
  gtk_container_add (GTK_CONTAINER (hbuttonbox10), print_cancel);
  GTK_WIDGET_SET_FLAGS (print_cancel, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (print_dialog), "delete_event",
                      GTK_SIGNAL_FUNC (on_dialog_delete_event),
                      print_dialog);
  gtk_signal_connect (GTK_OBJECT (print_ok), "clicked",
                      GTK_SIGNAL_FUNC (on_print_ok_clicked),
                      print_dialog);
  gtk_signal_connect (GTK_OBJECT (print_cancel), "clicked",
                      GTK_SIGNAL_FUNC (on_dialog_cancel_clicked),
                      print_dialog);

  return print_dialog;
}

