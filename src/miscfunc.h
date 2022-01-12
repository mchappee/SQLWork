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

#include <sys/types.h>
#include <dirent.h>

/* Manipulation for connection specifiers - return values should be g_free()ed
   by the caller */
char * conn_getdbtype(const char *l);
char * conn_getcstring(const char *l);

/* Manipulation for connection strings in the form "user[/pass][@host[:db]]"
   return values should be g_free()ed by the caller */
extern char *cstring_getusername(const char *l);
extern char *cstring_getpassword(const char *l);
extern char *cstring_gethost(const char *l);
extern char *cstring_getdb(const char *l);
extern char *cstring_getdbid(const char *l);

extern char *user_home_dir();

/* Approximation of the character width of the specified string in the
   specified font */
extern int  text_width(GdkFont *font, const gchar *text, gint text_length, int tabwidth);

/*
extern int scandir (const char * dir, struct dirent *** namelist,
		    int (*select) (const struct dirent *),
		    int (*cmp) (const void *, const void *));


int alphasort(const void *a, const void *b);
*/

/* Equivalent to mkdir -p */
extern int mkdir_path(const char *path, mode_t mode);

/* since we're not using gnome we have our own function for pretty
   buttons */
extern GtkWidget *gtk_button_new_with_pixmap(GtkWidget *, const char *, const char *);
extern void gtk_button_set_pixmap(GtkWidget *button, const char *, const char *);
