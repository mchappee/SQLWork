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

/* Definitions for common utility functions */

/* vmsgfmt.c */
extern char *vmsgfmt(const gchar *msg, va_list args);

/* export_*.c */
extern void export_as_csv(const char *fname, GtkCList *clist);
extern void export_as_sql(const char *fname, GtkCList *clist);
extern void export_as_html(const char *fname, GtkCList *clist);

/* errors.c */
extern void file_open_error(const char *fname, int e, char *rw);
extern void show_warning_msg(const char *msg, ...);
extern void fatal_error(const char *msg, ...); 

/* confirm.c */
extern int confirm(const char *msg, ...);

/* version.c */
const char *sqlwork_version();


