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

void
build_bookmarks_menus(GtkWidget *mainw);

void
on_bm_folder_menuselect(GtkMenuItem     *menuitem,
			 gpointer         user_data);

void
on_bm_folder_menudeselect(GtkMenuItem     *menuitem,
			   gpointer         user_data);

void
on_edit_bookmark_activate(GtkMenuItem     *menuitem,
			  gpointer         user_data);

void
on_bookmarks_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_organise_bookmark_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

gboolean
on_bookmark_organiser_delete_event     (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_bookm_org_edit_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_bookm_org_delete_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_bookm_org_done_clicked              (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_bookmark_organiser_delete_event     (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_bookm_org_nfolder_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_default_add_bookm_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

gboolean
on_add_bm_dialog_delete_event          (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_add_bm_ok_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_add_bm_cancel_clicked               (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_edit_bm_dialog_delete_event         (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_edit_bm_ok_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_edit_bm_cancel_clicked              (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_create_bmdir_dialog_delete_event    (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_create_bmdir_ok_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_create_bmdir_cancel_clicked         (GtkButton       *button,
                                        gpointer         user_data);
