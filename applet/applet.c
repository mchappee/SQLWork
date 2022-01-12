#include <gnome.h>
#include <applet-widget.h>
#include <libgnomeui/gnome-window-icon.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../src/prefs.h"
#include "../config.h"

GtkWidget    *applet;
char         *c_dbid;
gint         app_width;
gint         app_height;
GList        *db_glist=NULL;
GtkWidget    *dblist;
GtkWidget    *dblist_entry;

void 
db_finalise()
{

}

static void
free_glist_string_item(
    gpointer list_data,
    gpointer user_data)
{
    g_free(list_data);
}


static void
get_dblist()
{
    char    *connections;
    char    **tok_connections;
    int     a = 0;

    g_list_foreach (db_glist, free_glist_string_item, 0);
    g_list_free (db_glist);
    db_glist = NULL;    

    connections = prefs_get_char_var(PREF_CONNECT);
    if (connections && *connections) {
	    
	tok_connections = g_strsplit(connections, ":", 1000);

        while (tok_connections[a]){
                printf("\ntok_connections[%d]=%s", a, tok_connections[a]);
                fflush(stdout); 
	        db_glist = g_list_append(db_glist, g_strdup(tok_connections[a]));
		a++;
        }
	
        gtk_combo_set_popdown_strings (GTK_COMBO (dblist), db_glist);
        c_dbid = db_glist->data;

    }

}

static void
on_execute_button_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
    char    *argv[5];
    char    *sql;

    if(!c_dbid)
       return;

    sql  =  gtk_editable_get_chars(GTK_EDITABLE(user_data), 0,
                                gtk_text_get_length(GTK_TEXT(user_data)));

    if(strlen(sql) > 1){
        argv[0] = "sqlwork";
        argv[1] = c_dbid;
        argv[2] = "-x";
        argv[3] = sql;
        argv[4] = NULL;
        gnome_execute_async (NULL, 4, argv);
    }

    g_free(sql);
}

static void
on_dblist_changed                   (GtkEditable     *editable,
                                     gpointer         user_data)
{
    char *entry = NULL;

    entry = gtk_entry_get_text(GTK_ENTRY(editable));

    if(strlen(entry)>0)
        c_dbid = entry;

}

static void
about_cb (AppletWidget *widget, gpointer data)
{
        static GtkWidget *about = NULL;
        static const gchar *authors[2] =
        {"Matthew Chappee <matthew@advantio.com>", NULL};

        if (about != NULL)
        {
                gdk_window_show(about->window);
                gdk_window_raise(about->window);
                return;
        }
        about = gnome_about_new (_("The SQLWork Controller"),
                        "1.0",
                        "(C) 2000 Orasoft",
                        authors,
                        _("Hey, this is my first Gnome applet.  Whoo Hoo!"),
                        NULL);
        gtk_signal_connect( GTK_OBJECT(about), "destroy",
                            GTK_SIGNAL_FUNC(gtk_widget_destroyed), &about );
        gtk_widget_show (about);

        return;
        widget = NULL;
        data = NULL;
}

void
notebook_switch_page(
    GtkNotebook     *notebook,
    GtkNotebookPage *page,
    gint             page_num,
    gpointer         user_data)
{
    /*get_dblist();*/
}

static void
phelp_cb (GtkWidget *w, gint tab, gpointer data)
{
        GnomeHelpMenuEntry help_entry = {
                "sqlwork_applet", "index.html#SQLWORK-APPLET-PREFS"
        };
        gnome_help_display (NULL, &help_entry);
}


static void
create_sqlwork_applet (void)
{

  GtkWidget *handlebox1;
  GtkWidget *hbox1;
  GtkWidget *hbox2;
  GtkWidget *notebook1;
  GtkWidget *sql_buffer_scrolled;
  GtkWidget *sql_buffer;
  GtkWidget *sql_tab;
  GtkWidget *vbox1;
  GtkWidget *label3;
  GtkWidget *label2;
  GtkWidget *pixmap2;
  GtkWidget *pixmap1;
  GtkWidget *execute_button;
  GtkStyle  *style;

  style = gtk_style_new ();
  gdk_font_unref ( style->font );
  style->font = gdk_font_load ("-hp-antique olive-medium-r-normal-*-*-80-*-*-p-*-iso8859-9" );
  if ( style->font == NULL )
      style->font = gdk_font_load ( "fixed" );
  gtk_widget_push_style ( style );

  handlebox1 = gtk_handle_box_new ();
  gtk_widget_ref (handlebox1);
  gtk_widget_show (handlebox1);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox1);
  gtk_widget_show (hbox1);
  gtk_widget_set_usize(hbox1, 300, 45);
  gtk_container_add (GTK_CONTAINER (handlebox1), hbox1);  

  hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox2);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (hbox1), hbox2, FALSE, TRUE, 0);

  notebook1 = gtk_notebook_new ();
  gtk_widget_ref (notebook1);
  gtk_widget_show (notebook1);
  gtk_box_pack_start (GTK_BOX (hbox2), notebook1, TRUE, TRUE, 0);
  gtk_notebook_set_show_border (GTK_NOTEBOOK (notebook1), FALSE);
  gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook1), GTK_POS_RIGHT);
  gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook1), TRUE);
  gtk_notebook_set_tab_hborder (GTK_NOTEBOOK (notebook1), 0);
  gtk_notebook_set_tab_vborder (GTK_NOTEBOOK (notebook1), 0);
  gtk_notebook_popup_enable (GTK_NOTEBOOK (notebook1));

  sql_buffer_scrolled = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (sql_buffer_scrolled);
  gtk_widget_show (sql_buffer_scrolled);
  gtk_container_add (GTK_CONTAINER (notebook1), sql_buffer_scrolled);
  gtk_widget_set_usize (sql_buffer_scrolled, 229, -2);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sql_buffer_scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  sql_buffer = gtk_text_new (NULL, NULL);
  gtk_widget_ref (sql_buffer);
  gtk_widget_show (sql_buffer);
  gtk_container_add (GTK_CONTAINER (sql_buffer_scrolled), sql_buffer);
  gtk_text_set_editable (GTK_TEXT (sql_buffer), TRUE);
  gtk_widget_set_style(sql_buffer, style);

  sql_tab = gtk_label_new (_("sql"));
  gtk_widget_ref (sql_tab);
  gtk_widget_show (sql_tab);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 0), sql_tab);
  gtk_widget_set_style(sql_tab, style);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox1);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (notebook1), vbox1);

  label3 = gtk_label_new (_("Database ID:"));
  gtk_widget_ref (label3);
  gtk_widget_show (label3);
  gtk_box_pack_start (GTK_BOX (vbox1), label3, FALSE, FALSE, 0);

  dblist = gtk_combo_new ();
  gtk_widget_ref (dblist);
  gtk_widget_show (dblist);
  gtk_box_pack_start (GTK_BOX (vbox1), dblist, FALSE, FALSE, 0);

  dblist_entry = GTK_COMBO (dblist)->entry;
  gtk_widget_ref (dblist_entry);
  gtk_widget_show (dblist_entry);

  label2 = gtk_label_new (_("dbid"));
  gtk_widget_ref (label2);
  gtk_widget_show (label2);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 1), label2);
  gtk_widget_set_style(label2, style);

  pixmap2 = create_pixmap (applet, "swpa.xpm");
  gtk_widget_ref (pixmap2);
  gtk_widget_show (pixmap2);
  gtk_container_add (GTK_CONTAINER (notebook1), pixmap2);

  pixmap1 = create_pixmap (applet, "smiley.xpm");
  gtk_widget_ref (pixmap1);
  gtk_widget_show (pixmap1);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 2), pixmap1);

  execute_button = gtk_button_new_with_label (_(" G \n o "));
  gtk_widget_ref (execute_button);
  gtk_widget_show (execute_button);
  gtk_box_pack_start (GTK_BOX (hbox2), execute_button, FALSE, FALSE, 0);

  gtk_signal_connect (GTK_OBJECT (execute_button), "clicked",
                      GTK_SIGNAL_FUNC (on_execute_button_clicked),
                      (gpointer)sql_buffer);

  gtk_signal_connect (GTK_OBJECT (notebook1), "switch_page",
                        GTK_SIGNAL_FUNC (notebook_switch_page),
                        NULL);
  gtk_signal_connect (GTK_OBJECT (dblist_entry), "changed",
                      GTK_SIGNAL_FUNC (on_dblist_changed),
                      NULL);
		      
  applet_widget_add (APPLET_WIDGET (applet), handlebox1);

}

int main(
        int argc,
        char *argv[])
{
    char    *connections;
    char    *icondir;

    applet_widget_init("sqlwork_applet", "1.0", argc, argv,
                                    NULL, 0, NULL);
    gnome_window_icon_set_default_from_file ("/usr/src/redhat/SOURCES/gnome-applets-1.2.1/gweather/pixmaps/fog-mini.xpm");

    add_pixmap_directory (PACKAGE_DATA_DIR "/pixmaps");
    add_pixmap_directory (PACKAGE_SOURCE_DIR "/xpm");
   
    icondir = prefs_get_char_var(PREF_ICONPATH);
    if (icondir && *icondir)
        add_pixmap_directory (icondir);

    applet = applet_widget_new("sqlwork_applet");
    create_sqlwork_applet();

    get_dblist();

    gtk_widget_show (applet);
    applet_widget_gtk_main ();

}

