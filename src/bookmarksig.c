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

#include "xpm/bmfolder.xpm"
#include "xpm/bmfolder_open.xpm"
#include "xpm/bookmark.xpm"

#define list_set_opt_col_width(clist, col)     \
    gtk_clist_set_column_width (GTK_CLIST(clist), (col), \
				gtk_clist_optimal_column_width (GTK_CLIST(clist), (col)))

typedef struct {
    GtkCTree     *tree;
    GtkCTreeNode *parent;
} BuildBMCTreeContext;

typedef struct {
    GtkMenu      *add_menu;
    GtkMenuItem  *add_mitem;
    gboolean     add_need_submenu;
    guint        add_mitem_handler_id;
    GtkMenu      *ins_menu;
    GtkMenuItem  *ins_mitem;
    gboolean     ins_need_submenu;
    GtkMenu      *run_menu;
    GtkMenuItem  *run_mitem;
    gboolean     run_need_submenu;
} BuildBMMenuContext;

typedef struct _element {
    char	     *tag;
    char	     *val;
    struct _element  *subelts;
    struct _element  *next;
} Element;
    
/* Pixmaps and masks for the bookmarks organiser */
static GdkPixmap *bmfolder_pixmap;
static GdkBitmap *bmfolder_mask;
static GdkPixmap *bmofolder_pixmap;
static GdkBitmap *bmofolder_mask;
static GdkPixmap *bookm_pixmap;
static GdkBitmap *bookm_mask;

static Element	  *parse_bookmark(const char *filename);
static Element	  *parse_bookmark_mem(const char *bookm);
static char	  *element_val(Element *root, const char *tag);
static const char *parse_tag(const char *text, Element **element_out);
static void	  element_free(Element *root);
static void	  html_dequote_string(char *str);
static void	  on_add_bookmark_activate(GtkMenuItem *, gpointer);
static void	  on_insert_bookmark_activate(GtkMenuItem *, gpointer);
static void	  on_run_bookmark_activate(GtkMenuItem *, gpointer);
static void       check_bookmarks_dir();
static const char *get_bmdir_descr(const char *);

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

/* Callback to build menus for bookmarks. This is very repetitive as
   there are three to be built (add, insert, run) of which two (run
   and insert) are the same apart from the activation callback. The
   add menu only includes the directories */
static void
build_bm_menu_cb(
    const char		*dir,
    const char		*file,
    struct stat		*st,
    gpointer		context,
    gpointer		ncontext)
{
    GtkWidget		*hbox;
    GtkWidget		*pm;
    GtkWidget		*label;
    GtkWidget		*item;
    BuildBMMenuContext	*c	 = (BuildBMMenuContext *)context;
    char                *pathname;


#define make_item(pixmap, mask, text)					\
    do { item = gtk_menu_item_new ();					\
	 hbox = gtk_hbox_new(FALSE, 0);					\
	 pm = gtk_pixmap_new(pixmap, mask);				\
	 gtk_widget_show(pm);						\
	 gtk_box_pack_start(GTK_BOX(hbox), pm, FALSE, FALSE, 0);	\
	 label = gtk_label_new(text);					\
	 gtk_widget_show(label);					\
	 gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);	\
	 gtk_widget_show(hbox);						\
	 gtk_container_add(GTK_CONTAINER(item), hbox);			\
	 gtk_widget_show (item); } while(0)

    /* Arrange for dummy entry to be added to empty folders */
    if (!file && (c->ins_need_submenu || c->run_need_submenu))
	file = "<empty>";

    if (!file)
	return;

    /* construct the full path of the bookmark */
    pathname = g_malloc(strlen(dir) + strlen(file) + 2);
    strcpy(pathname, dir);
    strcat(pathname, G_DIR_SEPARATOR_S);
    strcat(pathname, file);

    if (c->run_need_submenu) {
	c->run_menu = GTK_MENU(gtk_menu_new());
	gtk_menu_item_set_submenu(c->run_mitem, GTK_WIDGET(c->run_menu));
	c->run_need_submenu = FALSE;
	gtk_signal_connect (GTK_OBJECT (c->run_mitem), "select",
			    GTK_SIGNAL_FUNC (on_bm_folder_menuselect),
			    NULL);
	gtk_signal_connect (GTK_OBJECT (c->run_mitem), "deselect",
			    GTK_SIGNAL_FUNC (on_bm_folder_menudeselect),
			    NULL);

    }
    if (c->ins_need_submenu) {
	c->ins_menu = GTK_MENU(gtk_menu_new());
	gtk_menu_item_set_submenu(c->ins_mitem, GTK_WIDGET(c->ins_menu));
	c->ins_need_submenu = FALSE;
	gtk_signal_connect (GTK_OBJECT (c->ins_mitem), "select",
			    GTK_SIGNAL_FUNC (on_bm_folder_menuselect),
			    NULL);
	gtk_signal_connect (GTK_OBJECT (c->ins_mitem), "deselect",
			    GTK_SIGNAL_FUNC (on_bm_folder_menudeselect),
			    NULL);
    }

    if (st && S_ISDIR(st->st_mode)) {
	BuildBMMenuContext *n = (BuildBMMenuContext *)ncontext;

	make_item(bmfolder_pixmap, bmfolder_mask, file);
	gtk_menu_append(GTK_MENU (c->run_menu), item);
	n->run_mitem = GTK_MENU_ITEM(item);
	n->run_need_submenu = TRUE;

	make_item(bmfolder_pixmap, bmfolder_mask, file);
	gtk_menu_append(GTK_MENU (c->ins_menu), item);
	n->ins_mitem = GTK_MENU_ITEM(item);
	n->ins_need_submenu = TRUE;

	if (c->add_need_submenu) {
	    char *parent = g_basename(dir);

	    c->add_menu = GTK_MENU(gtk_menu_new());
	    gtk_signal_connect (GTK_OBJECT (c->add_mitem), "select",
				GTK_SIGNAL_FUNC (on_bm_folder_menuselect),
				NULL);
	    gtk_signal_connect (GTK_OBJECT (c->add_mitem), "deselect",
				GTK_SIGNAL_FUNC (on_bm_folder_menudeselect),
				NULL);
	    gtk_menu_item_set_submenu(c->add_mitem, GTK_WIDGET(c->add_menu));

	    make_item(bmfolder_pixmap, bmfolder_mask, parent);
	    gtk_menu_append(GTK_MENU (c->add_menu), item);
	    gtk_object_set_data_full (GTK_OBJECT (item), "pathname", 
				      g_strdup(dir), g_free);
	    gtk_signal_connect (GTK_OBJECT (item), "activate",
				GTK_SIGNAL_FUNC (on_add_bookmark_activate),
				NULL);
	    item = gtk_menu_item_new();
	    gtk_widget_show(item);
	    gtk_menu_append(GTK_MENU (c->add_menu), item);
	    
	    if (c->add_mitem_handler_id != 0)
		gtk_signal_disconnect(GTK_OBJECT (c->add_mitem), c->add_mitem_handler_id);
	    c->add_need_submenu = FALSE;
	}

	make_item(bmfolder_pixmap, bmfolder_mask, file);
	gtk_menu_append(GTK_MENU (c->add_menu), item);
	gtk_object_set_data_full (GTK_OBJECT (item), "pathname", 
				  g_strdup(pathname), g_free);
	n->add_mitem = GTK_MENU_ITEM(item);
	n->add_mitem_handler_id = 
	    gtk_signal_connect (GTK_OBJECT (item), "activate",
				GTK_SIGNAL_FUNC (on_add_bookmark_activate),
				NULL);
	n->add_need_submenu = TRUE;
    } else {
	make_item(bookm_pixmap, bookm_mask, file);
	gtk_menu_append(GTK_MENU (c->run_menu), item);
	gtk_object_set_data_full (GTK_OBJECT (item), "pathname", 
				  g_strdup(pathname), g_free);
	gtk_signal_connect (GTK_OBJECT (item), "activate",
			    GTK_SIGNAL_FUNC (on_run_bookmark_activate),
			    NULL);

	make_item(bookm_pixmap, bookm_mask, file);
	gtk_menu_append(GTK_MENU (c->ins_menu), item);
	gtk_object_set_data_full (GTK_OBJECT (item), "pathname", 
				  g_strdup(pathname), g_free);
	gtk_signal_connect (GTK_OBJECT (item), "activate",
			    GTK_SIGNAL_FUNC (on_insert_bookmark_activate),
			    NULL);
    }
    g_free(pathname);
#undef make_item
}

static void
build_bm_ctree_cb(
    const char		 *dir,
    const char		 *file,
    struct stat		 *st,
    gpointer		 context,
    gpointer		 ncontext)
{
    BuildBMCTreeContext	 *c	  = (BuildBMCTreeContext *)context;
    char		 *pathname;

    if (!file)
	return;
    
    /* construct the full path of the bookmark */
    pathname = g_malloc(strlen(dir) + strlen(file) + 2);
    strcpy(pathname, dir);
    strcat(pathname, G_DIR_SEPARATOR_S);
    strcat(pathname, file);

    if (S_ISDIR(st->st_mode)) {
	BuildBMCTreeContext *n = (BuildBMCTreeContext *)ncontext;
	const char *row[2];
	
	row[0] = file;
	row[1] = get_bmdir_descr(pathname);

	n->parent = gtk_ctree_insert_node(c->tree, c->parent, 
					  0 /* sibling */,
					  (char **)row,
					  8 /* spacing */,
					  bmfolder_pixmap, bmfolder_mask,
					  bmofolder_pixmap, bmofolder_mask,
					  FALSE /* is_leaf */, 
					  FALSE /* expanded */);
    } else if (S_ISREG(st->st_mode)) {
	const char *row[2];
	GtkCTreeNode *node;
	Element *e = parse_bookmark(pathname);
	char * description = e ? element_val(e, "description") : 0;

	row[0] = file;
	row[1] = description ? description : "";

	node = gtk_ctree_insert_node(c->tree, c->parent, 
				     0 /* sibling */,
				     (char **)row,
				     8 /* spacing */,
				     bookm_pixmap, bookm_mask,
				     0 /* pixmap_opened */, 
				     0 /* mask_opened */,
				     TRUE /* is_leaf */, 
				     FALSE /* expanded */);
	gtk_ctree_node_set_row_data_full(c->tree, node,
					 g_strdup(pathname), g_free);
					 
	if (e)
	    element_free(e);
    }
    g_free(pathname);
}
    
/* Select subdirs and files which don't start with "." */
static int
bm_scandir_select(
    const struct dirent *de)
{
    return de->d_name[0] != '.';
}

static void
dirtree_recurser(
    const char	   *rootdir, 
    void           (*callback)(const char *, const char *, struct stat *, gpointer, gpointer),
    gpointer       context,
    size_t         csize)
{
    struct dirent  **namelist  = 0;
    struct stat     *st;
    int		   ndents;
    int		   i;
    gpointer       ncontext;

    if ((ndents = scandir(rootdir, &namelist, bm_scandir_select, alphasort)) < 0) {
	return;
    }
    st = g_malloc(sizeof(struct stat) * ndents);

    ncontext = g_malloc(csize);
    memcpy(ncontext, context, csize);

    for (i = 0; i < ndents; i++) {
	char *pathname = g_malloc(strlen(rootdir) + strlen(namelist[i]->d_name) + 2);

	strcpy(pathname, rootdir);
	strcat(pathname, G_DIR_SEPARATOR_S);
	strcat(pathname, namelist[i]->d_name);
	stat(pathname, &st[i]);

	if (S_ISDIR(st[i].st_mode)) {
	    callback(rootdir, namelist[i]->d_name, &st[i], context, ncontext);
	    dirtree_recurser(pathname, callback, ncontext, csize);
	}
	g_free(pathname);
    }
    for (i = 0; i < ndents; i++) {
	if (S_ISREG(st[i].st_mode)) {
	    callback(rootdir, namelist[i]->d_name, &st[i], context, ncontext);
	}
	free(namelist[i]);
    }
    free(namelist);
    callback(rootdir, 0, 0, context, ncontext);
    g_free(st);
    g_free(ncontext);
}

void
build_bookmarks_menus(
    GtkWidget	 *mainw)
{
    GtkMenuItem	 *addbm	  = GTK_MENU_ITEM(lookup_widget(mainw, "add_bookmark"));
    GtkMenuItem	 *insbm	  = GTK_MENU_ITEM(lookup_widget(mainw, "insert_bookmark"));
    GtkMenuItem	 *runbm	  = GTK_MENU_ITEM(lookup_widget(mainw, "run_bookmark"));
    char	 *bmdir	  = prefs_get_char_var(PREF_BMKPATH);

    BuildBMMenuContext c = {0, addbm, TRUE, 0, 0, insbm, TRUE, 0, runbm, TRUE};

    if (!bmfolder_pixmap) {
	bmfolder_pixmap = gdk_pixmap_create_from_xpm_d(mainw->window, &bmfolder_mask, 
						       NULL, bmfolder);
	bmofolder_pixmap = gdk_pixmap_create_from_xpm_d(mainw->window, &bmofolder_mask, 
							NULL, bmfolder_open);
	bookm_pixmap = gdk_pixmap_create_from_xpm_d(mainw->window, &bookm_mask, 
						    NULL, bookm);
    }
    check_bookmarks_dir();
    dirtree_recurser(bmdir, build_bm_menu_cb, &c, sizeof(c));
}

static void
check_bookmarks_dir()
{
    char *bmdir = prefs_get_char_var(PREF_BMKPATH);
    struct stat st;

    if (stat(bmdir, &st) < 0) {
	if (errno == ENOENT) {
	    gtk_widget_show(create_create_bmdir_dialog());
	    gtk_main();
	} else {
	    show_warning_msg("Error while checking your bookmarks directory\n%s\n%e",
			     bmdir, errno);
	}
    } else {
	if (!S_ISDIR(st.st_mode)) {
	    show_warning_msg("Your bookmarks directory\n%s "
			     "exists but is not a directory",
			     bmdir);
	}
    }
}

/* Parse a bookmark file */
/* Remember this is not XML and any passing resemblance is a coincidence
   which should fool no-one. Not even at 100 paces on a dark night */
static Element *
parse_bookmark(
    const char *filename)
{
    FILE	 *inf	  = fopen(filename, "r");
    Element      *ret;

    if (inf) {
	struct stat  st;
	char	     *filebuf;

	if (fstat(fileno(inf), &st) < 0) {
	    show_warning_msg("Can't stat open file %s, *highly* wierd!", 
			     filename);
	    return 0;
	} 
	if (st.st_size > 0) {
	    filebuf = g_malloc(st.st_size+1);
	    fread(filebuf, 1, st.st_size, inf);
	    filebuf[st.st_size] = '\0';
	    fclose(inf);
	    ret = parse_bookmark_mem(filebuf);
	    g_free(filebuf);
	    return ret;
	}
	return 0;
    }
    file_open_error(filename, errno, "read");
    return 0;
}

static Element *
parse_bookmark_mem(
    const char	 *bookm)
{
    const char	 *c	  = bookm;
    Element	 *result;

    /* skip <?.....?> */
    if (c && *c == '<' && *(c+1) == '?') {
	c+=2;
	while (*c && !(*c == '?' && *(c+1) == '>'))
	    ++c;
	if (*c && *c == '?' && *(c+1) == '>')
	    c+=2;
    }
    while (*c && isspace(*c)) 
	++c;
    if (*c == '<') {
	c = parse_tag(c, &result);
	/* Old style file ? */
	if (strcmp(result->tag, "name") == 0) {
	    while (isspace(*c))
		++c;
	    if (*c == '<') {
		parse_tag(c, &result->subelts);
	    }
	}
	return result;
    }
    return 0;
}

/* parse a tag starting at text. Whitespace prior to the tag should
   already have been skipped. The tag can then be a list of sub-tags
   or a value. If a list of sub-tags recurse to read it.
*/
static const char *
parse_tag(
    const char	 *text,
    Element	 **element_out)
{
    Element	 *e;
    const char	 *c	  = text;
    const char	 *s;
    int		 taglen;
    int		 valuelen;

    *element_out = 0;
    if (*c != '<')
	return 0;
    
    s = ++c;
    while(*c && *c != '>')
	++c;
    /* check tag terminated ok */
    if (*c != '>')
	return 0;

    /* Allocate & initialise the returned element */
    e = g_malloc(sizeof(Element));
    taglen = c - s;
    e->tag = g_malloc(taglen+1);
    memcpy(e->tag, s, taglen);
    e->tag[taglen] = '\0';
    e->val = 0;
    e->next = 0;
    e->subelts = 0;

    /* skip newline after tag if present*/
    if (*++c == '\n')
	++c;
    s = c;
    while (isspace(*c))
	++c;
    if (*c != '<') {
	/* No sub-elements, pull out the value */
	while (1) {
	    while (*c && *c != '<')
		++c;
	    if (!*c) {
		g_free(e->tag);
		g_free(e);
		return 0;
	    }
	    /* The following is a bit dangerous - on malformed input it could
	       read past the end of the buffer */
	    if (*(c+1) == '/' && *(c+taglen+2) == '>' 
		&& strncmp(c+2, e->tag, taglen) == 0) {
		/* We're done */
		valuelen = c - s;
		e->val = g_malloc(valuelen+1);
		memcpy(e->val, s, valuelen);
		e->val[valuelen] = '\0';
		html_dequote_string(e->val);
		*element_out = e;
		return c+taglen+3;
	    }
	    /* The input is mal-formed - however be lenient and see if
	       we can find the end tag if we keep looking */
	    ++c;
	}
    } else {
	while (!(*(c+1) == '/' && *(c+taglen+2) == '>' 
		 && strncmp(c+2, e->tag, taglen) == 0)) {
	    Element *e1;
	    if ((c = parse_tag(c, &e1)) != 0) {
		e1->next = e->subelts;
		e->subelts = e1;
	    } else {
		return 0;
	    }
	    while(isspace(*c))
		++c;
	    if (*c != '<')
		return 0;
	}
	*element_out = e;
	return c+taglen+3;
    }
}

static void
html_dequote_string(
    char *str)
{
    char *s = str;
    
    while(str && *str) {
	switch(*str) {
	case '\r':
	    ++str;
	    break;
	case '&':
	    if (strncmp(str+1, "lt", 2) == 0) {
		*s++ = '<';
		str += 3;
	    } else if (strncmp(str+1, "gt", 2) == 0) {
		*s++ = '>';
		str += 3;
	    } else if (strncmp(str+1, "amp", 3) == 0) {
		*s++ = '&';
		str += 4;
	    } else if (strncmp(str+1, "quot", 4) == 0) {
		*s++ = '"';
		str += 5;
	    } 
	    if (*str)
		++str;
	    break;
	default:
	    if (s != str)
		*s = *str;
	    s++;
	    str++;
	}
    }
    *s = '\0';
}

static char *
element_val(
    Element *root,
    const char *tag)
{
    Element *p;

    if (strcmp(tag, root->tag) == 0) {
	return root->val;
    } else {
	for (p = root->subelts; p; p = p->next) {
	    char *val = element_val(p, tag);
	    if (val)
		return val;
	}
    }
    return 0;
}

static void
element_free(
    Element *root)
{
    Element *p;

    for (p = root->subelts; p; p = p->next) {
	element_free(p);
    }
    g_free(root->val);
    g_free(root->tag);
    g_free(root);
}

void
on_bm_folder_menudeselect(
    GtkMenuItem     *menuitem,
    gpointer         user_data)
{
    GtkWidget *hbox = GTK_BIN(menuitem)->child;
    if (GTK_IS_HBOX(hbox)) {
	GtkWidget *pixmap = GTK_WIDGET(gtk_container_children(GTK_CONTAINER(hbox))->data);


	gtk_pixmap_set(GTK_PIXMAP(pixmap), bmfolder_pixmap, bmfolder_mask);
    }
}

void
on_bm_folder_menuselect(
    GtkMenuItem     *menuitem,
    gpointer         user_data)
{
    GtkWidget *hbox = GTK_BIN(menuitem)->child;
    if (GTK_IS_HBOX(hbox)) {
	GtkWidget *pixmap = GTK_WIDGET(gtk_container_children(GTK_CONTAINER(hbox))->data);


	gtk_pixmap_set(GTK_PIXMAP(pixmap), bmofolder_pixmap, bmofolder_mask);
    }
}


static void
on_add_bookmark_activate(
    GtkMenuItem	 *menuitem,
    gpointer	 user_data)
{
    char	 *pathname	= gtk_object_get_data(GTK_OBJECT(menuitem), "pathname");
    GtkWidget	 *mainw	        = lookup_widget(GTK_WIDGET(menuitem), "main_window");
    GtkWidget	 *add_bm_dialog	= create_add_bm_dialog();

    gtk_object_set_data_full(GTK_OBJECT(add_bm_dialog), "pathname",
			     g_strdup(pathname), g_free);
    gtk_object_set_data(GTK_OBJECT(add_bm_dialog), "main_window", mainw);
    gtk_widget_show(add_bm_dialog);
}

static void
on_insert_bookmark_activate(
    GtkMenuItem	 *menuitem,
    gpointer	 user_data)
{
    char	 *pathname  = gtk_object_get_data(GTK_OBJECT(menuitem), "pathname");
    Element	 *e	    = parse_bookmark(pathname);
    GtkText	 *sql_query = find_query_text_widget(GTK_WIDGET(menuitem));
    char	 *query;

    if (!e) {
	show_warning_msg("No data read from bookmark file %s", pathname);
	return;
    }
    if ((query = element_val(e, "query")) == 0)
	query = element_val(e, "bookmark");
    if (!query) {
	show_warning_msg("Can't find query text in bookmark %s", pathname);
    } else {
	guint l = gtk_text_get_length(sql_query);

	gtk_text_set_point(sql_query, l);
	gtk_text_insert(sql_query, 0, 0, 0, query, -1);
	gtk_editable_select_region  (GTK_EDITABLE(sql_query), l,
				     gtk_text_get_length(sql_query));
    }
    element_free(e);
}

static void
on_run_bookmark_activate(
    GtkMenuItem     *menuitem,
    gpointer         user_data)
{
    char *pathname = gtk_object_get_data(GTK_OBJECT(menuitem), "pathname");

    fprintf(stderr, "on_run_bookmark_activate %s\n", pathname);
}

void
on_default_add_bookm_activate(
    GtkMenuItem     *menuitem,
    gpointer         user_data)
{

}

void
on_organise_bookmark_activate(
    GtkMenuItem	 *menuitem,
    gpointer	 user_data)
{
    GtkWidget		 *organiser = create_bookmark_organiser();
    GtkWidget		 *bm_tree   = lookup_widget(organiser, "bookmark_list");
    GtkWidget		 *mainw     = lookup_widget(GTK_WIDGET(menuitem), "main_window");
    char		 *bmdir	    = prefs_get_char_var(PREF_BMKPATH);
    BuildBMCTreeContext	 c	    = {GTK_CTREE(bm_tree), 0};

    /* The pixmaps should have been set up when the main window was mapped */
    assert(bmfolder_pixmap);

    dirtree_recurser(bmdir, build_bm_ctree_cb, &c, sizeof(c));
    list_set_opt_col_width(bm_tree, 0);

    gtk_object_set_data(GTK_OBJECT(organiser), "main_window", mainw);
    gtk_widget_show(organiser);
}

gboolean
on_bookmark_organiser_delete_event(
    GtkWidget       *widget,
    GdkEvent        *event,
    gpointer         user_data)
{
    gtk_widget_destroy(GTK_WIDGET(user_data));

    return FALSE;
}

void
on_bookm_org_edit_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    GtkCList *list = GTK_CLIST(user_data);

    if (list->selection) {
	GtkCTreeNode *row = list->selection->data;
	const char *pathname = gtk_ctree_node_get_row_data(GTK_CTREE(list), row);

	if (pathname) {
	    Element *e = parse_bookmark(pathname);
	    if (e) {
		char *query = element_val(e, "query");

		if (!query)
		    query = element_val(e, "bookmark");
		if (!query) {
		    show_warning_msg("Can't find query text in bookmark\n%s", pathname);
		} else {
		    GtkWidget *dialog = create_edit_bm_dialog ();
		    GtkWidget *descr = lookup_widget(dialog, "edit_bm_descr");
		    GtkWidget *name = lookup_widget(dialog, "edit_bm_name");
		    GtkWidget *text = lookup_widget(dialog, "edit_bm_text");
		    char *description = element_val(e, "description");
		    
		    gtk_entry_set_text(GTK_ENTRY(name),
				       g_basename(pathname));
		    if (description)
			gtk_entry_set_text(GTK_ENTRY(descr),
					   description);
		    gtk_text_insert(GTK_TEXT(text), 0, 0, 0,
				    query, -1);
		    gtk_object_set_data_full(GTK_OBJECT(dialog), "pathname",
					     g_strdup(pathname), g_free);
		    gtk_widget_show(dialog);
		}
		element_free(e);
	    } else {
		show_warning_msg("No data read from bookmark file\n%s", 
				 pathname);
	    }
	}
    }
}

void
on_bookm_org_delete_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    GtkCList *list = GTK_CLIST(user_data);
    
    if (list->selection) {
	GtkCTreeNode *row = list->selection->data;
	const char *pathname = gtk_ctree_node_get_row_data(GTK_CTREE(list), row);
	
	if (pathname) {
	    if (unlink(pathname) == 0) {
		gtk_ctree_remove_node(GTK_CTREE(list), row);
	    } else {
		show_warning_msg("Can't delete %s\n%e", pathname, errno);
	    }
	} else {
	    show_warning_msg("Folders can't be deleted yet.", pathname);
	}
    }
}


void
on_bookm_org_done_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    GtkWidget *mainw = gtk_object_get_data(GTK_OBJECT(user_data), "main_window");

    build_bookmarks_menus(mainw);
    gtk_widget_destroy(GTK_WIDGET(user_data));
}

void
on_bookm_org_nfolder_clicked(
    GtkButton       *button,
    gpointer         user_data)
{

}

gboolean
on_add_bm_dialog_delete_event(
    GtkWidget       *widget,
    GdkEvent        *event,
    gpointer         user_data)
{
    gtk_widget_destroy(GTK_WIDGET(user_data));

    return FALSE;
}


static void
save_bookmark(
    char *dir,
    char *bmfile,
    char *descr,
    char *text)
{
    char *pathname = g_malloc(strlen(dir) + strlen(bmfile) + 2);
    FILE *outf;

    if (dir && *dir) {
	strcpy(pathname, dir);
	strcat(pathname, G_DIR_SEPARATOR_S);
	strcat(pathname, bmfile);
    } else {
	strcpy(pathname, bmfile);
    }

    bmfile = html_quote_string(bmfile);
    descr = html_quote_string(descr);
    text = html_quote_string(text);
    outf = fopen(pathname, "w");
    if (outf) {
	size_t text_len = strlen(text);

	/* Output the XML version info and our root element
           '<SQLWork-Bookmark>'. */
	/* Note that the file isn't supposed to *be* XML, it's supposed to be
	   something close enough that we can make it real xml without too much
	   pain (should we choose)*/
	fprintf (outf, "<?xml version=\"1.0\"?>\n<SQLWork-Bookmark>\n");
	
	/* write the name, */
	fprintf (outf, "    <name>%s</name>\n", bmfile);

	/* the description */
	fprintf (outf, "    <description>%s</description>\n", descr);

	/* and finally the query */
	fprintf (outf, "    <query>\n%s%s</query>\n", text, 
		 text_len == 0 || text[text_len-1] != '\n' ? "\n" : "");

	/* Finish the root element. */
	fprintf (outf, "</SQLWork-Bookmark>\n");
	fclose(outf);
   } else {
	file_open_error(pathname, errno, "write");
   }
    g_free(bmfile);
    g_free(descr);
    g_free(text);
}

void
on_add_bm_ok_clicked(
    GtkButton	*button,
    gpointer	user_data)
{
    GtkWidget	*dialog	 = lookup_widget(GTK_WIDGET(button), "add_bm_dialog");
    GtkWidget	*name	 = lookup_widget(dialog, "add_bm_name");
    GtkWidget	*descr	 = lookup_widget(dialog, "add_bm_descr");
    GtkWidget	*mainw	 = gtk_object_get_data(GTK_OBJECT(dialog), "main_window");
    GtkText	*sql     = find_query_text_widget(mainw);
    char	*dir	 = gtk_object_get_data(GTK_OBJECT(dialog), "pathname");
    char	*bmname	 = gtk_entry_get_text(GTK_ENTRY(name));
    char	*bmdescr = gtk_entry_get_text(GTK_ENTRY(descr));
    char        *qtext   = gtk_editable_get_chars(GTK_EDITABLE(sql), 0,
						  gtk_text_get_length(sql));
    
    save_bookmark(dir, bmname, bmdescr, qtext);
    g_free(qtext);
    build_bookmarks_menus(mainw);
    gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_add_bm_cancel_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    gtk_widget_destroy(GTK_WIDGET(user_data));
}


gboolean
on_edit_bm_dialog_delete_event(
    GtkWidget       *widget,
    GdkEvent        *event,
    gpointer         user_data)
{
    gtk_widget_destroy(GTK_WIDGET(user_data));
    return FALSE;
}


void
on_edit_bm_ok_clicked(
    GtkButton	*button,
    gpointer	user_data)
{
    GtkWidget	*dialog	 = lookup_widget(GTK_WIDGET(button), "edit_bm_dialog");
    GtkWidget	*descr	 = lookup_widget(dialog, "edit_bm_descr");
    GtkWidget	*query	 = lookup_widget(dialog, "edit_bm_text");
    char	*dir	 = gtk_object_get_data(GTK_OBJECT(dialog), "pathname");
    char	*bmdescr = gtk_entry_get_text(GTK_ENTRY(descr));
    char	*qtext	 = gtk_editable_get_chars(GTK_EDITABLE(query), 0,
						  gtk_text_get_length(GTK_TEXT(query)));

    save_bookmark("", dir, bmdescr, qtext);
    g_free(qtext);
    gtk_widget_destroy(GTK_WIDGET(user_data));
}


void
on_edit_bm_cancel_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    gtk_widget_destroy(GTK_WIDGET(user_data));
}


gboolean
on_create_bmdir_dialog_delete_event(
    GtkWidget       *widget,
    GdkEvent        *event,
    gpointer         user_data)
{
    gtk_widget_destroy(GTK_WIDGET(user_data));
    gtk_main_quit();
    return FALSE;
}


void
on_create_bmdir_ok_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    char	 *bmdir	  = prefs_get_char_var(PREF_BMKPATH);
    GtkWidget    *subfolderscb = lookup_widget(GTK_WIDGET(button), 
					       "create_bm_subfolders_cb");
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(subfolderscb))) {
	GList	*dblist   = db_list();
	int	bmdirlen  = strlen(bmdir);

	while (dblist) {
	    char *folder = g_malloc(bmdirlen + strlen((char *)dblist->data) + 2);
	    int err;

	    strcpy(folder, bmdir);
	    strcpy(folder+bmdirlen, G_DIR_SEPARATOR_S);
	    strcat(folder+bmdirlen, (char *)dblist->data);
	    err = mkdir_path(folder, 0750);
	    g_free(folder);
	    if (err < 0) {
		show_warning_msg("Failed to make bookmarks directory\n%s\n\n"
				 "%e", folder, err);
		break;
	    }
	    dblist = g_list_next(dblist);
	}
    } else {
	if (mkdir_path(bmdir, 0750) < 0) {
	    show_warning_msg("Failed to make bookmarks directory\n%s\n\n"
			     "%e", bmdir, errno);
	}
    }
    gtk_widget_destroy(GTK_WIDGET(user_data));
    gtk_main_quit();
}


void
on_create_bmdir_cancel_clicked(
    GtkButton       *button,
    gpointer         user_data)
{
    gtk_widget_destroy(GTK_WIDGET(user_data));
    gtk_main_quit();
}

static const char *
get_bmdir_descr(
    const char *dir)
{
    /* construct the full path of the bookmark */
    char *pathname = g_malloc(strlen(dir) + 32);
    strcpy(pathname, dir);
    strcat(pathname, G_DIR_SEPARATOR_S);
    strcat(pathname, ".description");

    return "";
}
