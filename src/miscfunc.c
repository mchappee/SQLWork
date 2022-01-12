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
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#endif

#include <pwd.h>

#include <gtk/gtk.h>

#include "common.h"
#include "support.h"
#include "miscfunc.h"

/* Routines for manipulating the string representations of database
   connections. There are two of these a "connection specifier" which has the
   form Driver(connection_string). Driver is the backend driver name (e.g
   Oracle) and connection_string is a driver specifig string encoding user
   name, password, specific database instance etc. Routines for manipulating
   connection strings in the form user[/pass][@host[:db]] are also provided 

   Finally a tag called the dbid is extracted from the connection string - this
   is used to identify individual connections when calling the driver routines.
   It is also used on the UI to identify which connection is active. */

/* connection spec manipulation */
char *
conn_getdbtype(
    const char *l)
{
    char *sep = strstr(l, "(");
    if (sep) {
	int len = sep - l;
	char *dbtype = g_malloc(len+1);
	
	memcpy(dbtype, l, len);
	dbtype[len] = '\0';
	return dbtype;
    }
    return 0;
}

char *
conn_getcstring(
    const char *l)
{
    char *s1 = strstr(l, "(");
    char *s2 = s1 ? strstr(s1+1, ")") : 0;

    if (s1 && s2) {
	int len = s2 - s1 - 1;
	char *cstring = g_malloc(len+1);
	
	memcpy(cstring, s1+1, len);
	cstring[len] = '\0';
	return cstring;
    }
    return 0;
}

/* cstring manipulation */
/* Extract user name from connection string */
char *
cstring_getusername(
    const char *l)
{
    char *tmp;
    char *sep;

    /* copy input so that we may modify it */
    tmp=g_strdup(l);

    /* Accept "user[/pass][@host[:db]]", return user */
    if((sep = strstr(tmp,"/")) ||
       (sep = strstr(tmp,"@"))) {
	*sep = '\0';
    }
    return tmp;
}

/* Extract password from connection string */
char *
cstring_getpassword(
    const char *l)
{
    char *passwd;
    char *tmp;
    char *sep1;
    char *sep2;

    /* copy input so that we may modify it */
    tmp=g_strdup(l);

    /* Accept "user[/pass][@host[:db]]", return password */
    if((sep1 = strstr(tmp,"/")) &&
       (sep2 = strstr(sep1+1,"@"))) {
	*sep2 = '\0';
	passwd = g_strdup(sep1+1);
	g_free(tmp);
	return passwd;
	
    }
    g_free(tmp);
    return 0;
}

/* Extract host from connection string */
char *
cstring_gethost(
    const char *l)
{
    char *host;
    char *tmp;
    char *sep;

    /* copy input so that we may modify it */
    tmp=g_strdup(l);

    /* Accept "user[/pass][@host[:db]]", return host */
    if((sep = strstr(tmp, "@")) != 0) {
	host = g_strdup(sep+1);
	if ((sep = strstr(host, ":")) != 0) {
	    *sep = '\0';
	}
	g_free(tmp);
	return host;
    }
    g_free(tmp);
    return 0;
}

/* Extract database instance from connection string */
char *
cstring_getdb(
    const char *l)
{
    char *host;
    char *db = 0;
    char *tmp;
    char *sep;

    /* copy input so that we may modify it */
    tmp=g_strdup(l);

    /* Accept "user[/pass][@host[:db]]", return db */
    if((sep = strstr(tmp, "@"))) {
	host = sep+1;
	if ((sep = strstr(tmp, ":")) != 0) {
	    db = g_strdup(sep+1);
	}
	g_free(tmp);
	return db;
    }
    g_free(tmp);
    return 0;
}

/* Return a database id from a connection string.  This code returns
   user@host which is ok for both MySQL and Oracle */

char *
cstring_getdbid(
    const char *l)
{
    char    *user  = cstring_getusername(l);
    char    *host  = cstring_gethost(l);
    char    *db    = cstring_getdb(l);
    GString *dbids = g_string_new(user);
    char    *dbid;

    if (host) {
	g_string_sprintfa(dbids, "@%s", host);
    } else {
	g_string_sprintfa(dbids, "@local");
    }
    if (db) {
	g_string_sprintfa(dbids, ":%s", db);
    }
    g_free(user);
    g_free(host);
    g_free(db);
    
    dbid = dbids->str;
    g_string_free(dbids, 0);
    return dbid;
}


/* Return the users's home directory */
char *user_home_dir()
{
    static char *result = 0;
    char *env_home;
    
    if (result) {
	return result;
    }

    env_home=getenv("HOME");
    if (env_home) {
	result = g_strdup(env_home);
    } else {
	/* HOME not set - get home dir for real uid from password
	   file. */
	struct passwd pwd;
	struct passwd *pwdr;
	int szpwbuf = sysconf(_SC_GETPW_R_SIZE_MAX);
	char *pwbuf = g_malloc(szpwbuf);

	if (getpwuid_r(getuid(), &pwd, pwbuf, szpwbuf, &pwdr) == 0) {
	    result = g_strdup(pwd.pw_dir);
	}
	g_free(pwbuf);
    }
    if (result == 0) {
	fatal_error("Your home directory cannot be located\n"
		    "I've checked both the \"HOME\" environment variable\n"
		    "and your password entry but I still can't figure it out.");
    }
    if (access(result, W_OK) == -1) {
	char *msg = g_malloc(strlen(result) + 256);
	sprintf(msg, "Your home directory (%s)\n"
		"isn't writeable by you - which is a bit odd if you ask me.\n%s"
		"There may be problems later because of this.",
		result, 
		env_home ? "Check whether the \"HOME\" environment variable is correctly set\n" : 0);
	show_warning_msg(msg);
	g_free(msg);
    }
    return result;
}

/* Calculate the width in pixels of the given text string expanding
   tabs to <tabwidth> spaces
   
   Actually, just guess - we'll do this properly before the code is
   released, honest :-) */

int
text_width(
    GdkFont        *font,
    const gchar    *text,
    gint            text_length,
    int             tabwidth)
{
    GString        *expanded;
    const char     *p = text;
    const char     *s = text;
    static char    spaces[] = "        ";
    int            col = 0;
    int            ntabs = 0;

    while(*p) {
	if (*p == '\t') {
	    s=p;
	    ++ntabs;
	}
	p++;
    }
    return 10*text_length + 70*ntabs;
}

#ifndef HAVE_SCANDIR
/* From glibc */
int
scandir (const char * dir,
	 struct dirent *** namelist,
	 int (*select) (const struct dirent *),
	 int (*cmp) (const void *, const void *))
{
    DIR *dp = opendir (dir);
    struct dirent **v = NULL;
    size_t vsize = 0, i;
    struct dirent *d;
    int save;
    
    if (dp == NULL)
	return -1;
    
    save = errno;
    errno = 0;
    
    i = 0;
    while ((d = readdir (dp)) != NULL)
	if (select == NULL || (*select) (d)) {
	    if (i == vsize) {
		struct dirent **new;
		if (vsize == 0)
		    vsize = 10;
		else
		    vsize *= 2;
		new = (struct dirent **) realloc (v, vsize * sizeof (*v));
		if (new == NULL) {
		  lose:
		    errno = ENOMEM;
		    break;
		}
		v = new;
	    }
	    
	    v[i] = (struct dirent *) malloc (sizeof (**v));
	    if (v[i] == NULL)
		goto lose;
	    
	    *v[i++] = *d;
	}
    
    if (errno != 0) {
	save = errno;
	(void) closedir (dp);
	while (i > 0)
	    free (v[--i]);
	free (v);
	errno = save;
	return -1;
    }
    
    (void) closedir (dp);
    errno = save;

    /* Sort the list if we have a comparison function to sort with.  */
    if (cmp != NULL)
	qsort (v, i, sizeof (*v), cmp);
    *namelist = v;
    return i;
}
#endif

#ifndef HAVE_ALPHASORT
/* from glibc FWIW */
int
alphasort(
    const void *a, 
    const void *b)
{
    return strcmp (((struct dirent *) a)->d_name,
		   ((struct dirent *) b)->d_name);
}
#endif

/* From autofs */
int 
mkdir_path(
    const char	 *path, 
    mode_t	 mode)
{
    char	 *buf	  = alloca(strlen(path)+1);
    const char	 *cp	  = path;
    const char	 *lcp	  = path;
    char	 *bp	  = buf;

    do {
	if (cp != path && (*cp == G_DIR_SEPARATOR || *cp == '\0')) {
	    memcpy(bp, lcp, cp-lcp);
	    bp += cp-lcp;
	    lcp = cp;
	    *bp = '\0';
	    if (mkdir(buf, mode) == -1) {
		/* If it already exists, make sure it's a directory */
		if (errno == EEXIST) {
		    struct stat st;
		    
		    if (stat(buf, &st) == 0 &&
			!S_ISDIR(st.st_mode))
			errno = ENOTDIR;
		    else
			continue;
		}
		return -1;
	    }
	}
    } while(*cp++ != '\0');
    return 0;
}

GtkWidget *
gtk_button_new_with_pixmap(
    GtkWidget	 *colormap_win,
    const char	 *pixmap_name,
    const char	 *label_text)
{
    GtkWidget	 *button;
    GtkWidget	 *hbox;
    GtkWidget	 *pixmap;
    GtkWidget	 *label;

    button = gtk_button_new ();
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (hbox);
    gtk_container_add(GTK_CONTAINER(button), hbox);
    pixmap = create_pixmap (colormap_win, pixmap_name);
    gtk_widget_show (pixmap);
    gtk_box_pack_start(GTK_BOX(hbox), pixmap, FALSE, FALSE, 0);
    label = gtk_label_new (label_text);
    gtk_widget_show (label);
    gtk_misc_set_alignment (GTK_MISC (label), 0.5, 0.5);
    gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);

    return button;
}

void
gtk_button_set_pixmap(
    GtkWidget	 *button,
    const char	 *pixmap_name,
    const char	 *label_text)
{
    GtkWidget	 *button_hbox;
    GtkWidget	 *button_pixmap;
    GtkWidget	 *button_label;

    gtk_container_remove(GTK_CONTAINER(button), GTK_BIN (button)->child);
    button_hbox = gtk_hbox_new (FALSE, 0);
    gtk_widget_show (button_hbox);
    gtk_container_add(GTK_CONTAINER(button), button_hbox);
    button_pixmap = create_pixmap (button, pixmap_name);
    gtk_widget_show (button_pixmap);
    gtk_box_pack_start(GTK_BOX(button_hbox), button_pixmap, FALSE, FALSE, 0);
    button_label = gtk_label_new (label_text);
    gtk_widget_show (button_label);
    gtk_misc_set_alignment (GTK_MISC (button_label), 0.5, 0.5);
    gtk_box_pack_start(GTK_BOX(button_hbox), button_label, TRUE, TRUE, 0);
}
