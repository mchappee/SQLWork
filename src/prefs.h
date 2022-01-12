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

#define PREF_GRIDUPD  "grid_update"
#define PREF_GRIDAUT  "grid_autosize"
#define PREF_LOGIN    "login"
#define PREF_MAXROWS  "max_rows"
#define PREF_EXPMAXR  "export_max_rows"
#define PREF_TEXTCOLW "col_width"
#define PREF_TEXTFNT  "text_font"
#define PREF_TEXTUPD  "text_update"
#define PREF_CHARSEP  "char_sep"
#define PREF_AUTOCMT  "auto_commit"
#define PREF_SAVE     "prefs_save_on_exit"
#define PREF_TBSTYLE  "toolbar_display_style"
#define PREF_ICONPATH "icon_path"
#define PREF_BMKPATH  "bookmark_path"
#define PREF_CONNECT  "connections"
#define PREF_PSIZE    "ps_paper_size"
#define PREF_FSIZE    "ps_font_size"
#define PREF_FONT     "ps_font"

extern int   prefs_get_int_var(const char *varname);
extern char *prefs_get_char_var(const char *varname);

extern void  prefs_set_int_var(const char *varname, int value);
extern void  prefs_set_char_var(const char *varname, const char *value);

extern void  prefs_save_values(void);

