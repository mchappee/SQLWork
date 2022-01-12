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
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "common.h"
#include "miscfunc.h"
#include "dbif.h"

extern DbInterface *dblist[];
static GList *dbif_name_list = 0;

void
db_initialise(void)
{
    DbInterface **dbif = dblist;
    while (*dbif) {
	if ((*dbif)->initialise() == DB_SUCCESS) {
	    dbif_name_list = g_list_prepend(dbif_name_list, (*dbif)->db_name);
	}
	++dbif;
    }
    dbif_name_list = g_list_reverse(dbif_name_list);
}


void
db_finalise(void)
{
    GList *dbif_name;

    for(dbif_name = dbif_name_list; dbif_name; dbif_name = g_list_next(dbif_name)) {
	const DbInterface *dbif = db_find(dbif_name->data);
	dbif->finalise();
    }
}

GList *
db_list(void)
{
    return dbif_name_list;
}

const DbInterface *
db_find(const char *name)
{
    DbInterface **dbif = dblist;
    while (*dbif) {
	if (g_strcasecmp((*dbif)->db_name, name) == 0)
	    return *dbif;
	++dbif;
    }
    return 0;
}
