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
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "common.h"
#include "miscfunc.h"
#include "support.h"

static void
on_ok_button_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    int *response = (int *)gtk_object_get_data (GTK_OBJECT (user_data), "response");

    *response = 1;
    gtk_widget_destroy(GTK_WIDGET(user_data));
    gtk_main_quit();
}

static void
on_dialog_destroyed(
    GtkButton       *button,
    gpointer         user_data)
{
    gtk_widget_destroy(GTK_WIDGET(user_data));
    gtk_main_quit();
}

static void
on_cancel_button_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    gtk_widget_destroy(GTK_WIDGET(user_data));
    gtk_main_quit();
}

GtkWidget*
create_dialog (
    const char *msg,
    int *response)
{
  GtkWidget *confirm_win;
  GtkWidget *dialog_vbox1;
  GtkWidget *label1;
  GtkWidget *dialog_action_area1;
  GtkWidget *hbuttonbox1;
  GtkWidget *hbox1;
  GtkWidget *question_pixmap;
  GtkWidget *cancel_button;
  GtkWidget *ok_button;

  confirm_win = gtk_dialog_new ();
  gtk_object_set_data (GTK_OBJECT (confirm_win), "confirm_win", confirm_win);
  gtk_window_set_title (GTK_WINDOW (confirm_win), _("Confirmation required"));
  gtk_window_set_policy (GTK_WINDOW (confirm_win), TRUE, TRUE, FALSE);

  gtk_object_set_data (GTK_OBJECT (confirm_win), "response", response);

  dialog_vbox1 = GTK_DIALOG (confirm_win)->vbox;
  gtk_object_set_data (GTK_OBJECT (confirm_win), "dialog_vbox1", dialog_vbox1);
  gtk_widget_show (dialog_vbox1);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox1);
  gtk_object_set_data_full (GTK_OBJECT (confirm_win), "hbox1", hbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (dialog_vbox1), hbox1, FALSE, FALSE, 0);
  
  question_pixmap = create_pixmap (confirm_win, "gnome-question.xpm");
  gtk_widget_show (question_pixmap);
  gtk_box_pack_start(GTK_BOX(hbox1), question_pixmap, FALSE, FALSE, 0);

  label1 = gtk_label_new (msg);
  gtk_widget_ref (label1);
  gtk_object_set_data_full (GTK_OBJECT (confirm_win), "label1", label1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (hbox1), label1, FALSE, FALSE, 0);

  dialog_action_area1 = GTK_DIALOG (confirm_win)->action_area;
  gtk_object_set_data (GTK_OBJECT (confirm_win), "dialog_action_area1", dialog_action_area1);
  gtk_widget_show (dialog_action_area1);
  gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area1), 10);

  hbuttonbox1 = gtk_hbutton_box_new ();
  gtk_widget_ref (hbuttonbox1);
  gtk_object_set_data_full (GTK_OBJECT (confirm_win), "hbuttonbox1", hbuttonbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbuttonbox1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox1), GTK_BUTTONBOX_SPREAD);
  gtk_box_pack_start (GTK_BOX (dialog_action_area1), hbuttonbox1, TRUE, TRUE, 0);

  ok_button = gtk_button_new_with_pixmap (confirm_win, "gnome-ok.xpm", _("OK"));
  gtk_widget_ref (ok_button);
  gtk_object_set_data_full (GTK_OBJECT (confirm_win), "ok_button", ok_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  
  gtk_widget_show (ok_button);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), ok_button);
  GTK_WIDGET_SET_FLAGS (ok_button, GTK_CAN_DEFAULT);

  cancel_button = gtk_button_new_with_pixmap (confirm_win, "gnome-cancel.xpm", _("Cancel"));
  gtk_widget_ref (cancel_button);
  gtk_object_set_data_full (GTK_OBJECT (confirm_win), "cancel_button", cancel_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  
  gtk_widget_show (cancel_button);
  gtk_container_add (GTK_CONTAINER (hbuttonbox1), cancel_button);
  GTK_WIDGET_SET_FLAGS (cancel_button, GTK_CAN_DEFAULT);

  gtk_signal_connect (GTK_OBJECT (cancel_button), "clicked",
		      GTK_SIGNAL_FUNC (on_cancel_button_clicked),
		      (gpointer)confirm_win);
  gtk_signal_connect (GTK_OBJECT (ok_button), "clicked",
		      GTK_SIGNAL_FUNC (on_ok_button_clicked),
		      (gpointer)confirm_win);
  gtk_signal_connect(GTK_OBJECT(confirm_win), "destroy",
		     GTK_SIGNAL_FUNC(on_dialog_destroyed),
		     (gpointer)confirm_win);
  
  return confirm_win;
}

int
confirm(
    const char *msg,
    ...)
{
    va_list     args;
    char        *msgf;
    GtkWidget   *dialog;
    int         response = 0;

    va_start(args, msg);
    msgf = vmsgfmt(msg, args);
    va_end(args);

    dialog = create_dialog(msgf, &response);
    g_free(msgf);
    gtk_widget_show(dialog);
    gtk_main();
    return response;
}

