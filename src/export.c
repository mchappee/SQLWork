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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>

#include <gtk/gtk.h>

#include "common.h"
#include "export.h"
#include "dbif.h"

struct {
    char *name;
    double wx;
    double wy; } PaperSizes[] = {
	{"11x17", 27.94, 43.18},
	{"a0", 83.9611, 118.816},
	{"a1", 59.4078, 83.9611},
	{"a2", 41.9806, 59.4078},
	{"a3", 29.7039, 41.9806},
	{"a4", 20.9903, 29.7039},
	{"a5", 14.8519, 20.9903},
	{"archA", 22.86, 30.48},
	{"archB", 30.48, 45.72},
	{"archC", 45.72, 60.96},
	{"archD", 60.96, 91.44},
	{"archE", 91.44, 121.92},
	{"b0", 100.048, 141.393},
	{"b1", 70.6967, 100.048},
	{"b2", 50.0239, 70.6967},
	{"b3", 35.3483, 50.0239},
	{"b4", 25.0119, 35.3483},
	{"b5", 17.6742, 25.0119},
	{"executive", 190, 254},
	{"flsa", 21.59, 33.02},
	{"flse", 21.59, 33.02},
	{"halfletter", 13.97, 21.59},
	{"ledger", 43.18, 27.94},
	{"legal", 21.59, 35.56},
	{"letter", 21.59, 27.94},
	{"note", 19.05, 25.4},
	{0, 0, 0}};

/* Quote any html meta chars */
char *
html_quote_string(
    const char	     *str)
{
    char	     *html_str;
    char	     *s;
    const char	     *p;

    html_str = g_malloc(strlen(str) * 6 + 1);
    for (p = str, s = html_str; p && *p ; p++) {
	switch(*p) {
	case '<': strcpy(s, "&lt ");   s += 4; break;
	case '>': strcpy(s, "&gt ");   s += 4; break;
	case '&': strcpy(s, "&amp ");  s += 5; break;
	case '"': strcpy(s, "&quot "); s += 6; break;
	default: *s++ = *p; break;
	}
    }
    *s = '\0';
    return html_str;
}

/* Quote any postscript meta chars */
static char *
ps_quote_string(
    const char	     *str)
{
    char	     *ps_str;
    char	     *s;
    const char	     *p;

    ps_str = g_malloc(strlen(str) * 2 + 1);
    for (p = str, s = ps_str; p && *p ; p++) {
	switch(*p) {
	case '(': strcpy(s, "\\(");   s += 2; break;
	case ')': strcpy(s, "\\)");   s += 2; break;
	default: *s++ = *p; break;
	}
    }
    *s = '\0';
    return ps_str;
}

/* Save as comma separated values */
void
export_as_csv(
    const char	 *fname,
    GtkCList	 *clist)
{
    char	 *value;
    char	 *c;
    FILE	 *stream;
    int		 ncols=0;
    int		 nrows=0;

    assert(fname && *fname);

    /* fname is passed from the export function, 
       and is the full path to the export file */
    if (*fname == '|')
	stream = popen(fname+1, "w");
    else
	stream=fopen(fname,"w");

    if (stream) {
	while(nrows < clist->rows){
	    while(ncols < clist->columns){
		gtk_clist_get_text(clist, nrows, ncols, &value);
		ncols += 1;
	    
		/* This routine replaces all commas with semi-colons */
		while((c=strchr(value,','))!=NULL){
		    *c=';';
		}
	    
		/* We don't want a leading comma */
		if(ncols == 1)
		    fprintf(stream,"%s",value);
		else
		    fprintf(stream,",%s",value);
	    }
	    ncols = 0;
	    fprintf(stream,"\n");
	    nrows += 1;
	}
	(*fname == '|' ? pclose : fclose)(stream);
    } else {
	file_open_error(fname, errno, "write");
    }
}

/* Save a clist as a html table definition */
void
export_as_html(
    const char	*fname, 
    GtkCList	*clist)
{
    char	*value;
    FILE	*stream;
    int		ncol;
    int		nrow;
    char	*tablename = gtk_object_get_data(GTK_OBJECT(clist), "tablename");
    char	*owner	   = gtk_object_get_data(GTK_OBJECT(clist), "owner");
    
    assert(fname && *fname);

    if (*fname == '|')
	stream = popen(fname+1, "w");
    else
	stream=fopen(fname,"w");

    if (stream) {
	if (tablename && owner)
	    fprintf(stream,"<HTML>\n<HEAD>\n<TITLE>%s.%s</TITLE>\n",
		    owner, tablename);
	else
	    fprintf(stream,"<HTML>\n<HEAD>\n<TITLE>Query results</TITLE>\n");

	fprintf(stream,"</HEAD>\n<BODY BGCOLOR=\"#ffffff\">\n\n");

	fprintf(stream,"<table BORDER=\"1\">\n<tr BGCOLOR=\"#C0C0C0\">");
	for (ncol = 0; ncol < clist->columns; ncol++) {
	    char *s = html_quote_string(gtk_clist_get_column_title (clist, ncol));
	    fprintf(stream,"<th>%s</th>", s);
	    g_free(s);
	}
	
	for(nrow = 0;nrow < clist->rows; nrow++){
	    fprintf(stream,"</tr>\n<tr>");
	    for(ncol = 0; ncol < clist->columns; ncol++){
		char *s;

		gtk_clist_get_text(clist, nrow, ncol, &value);
		s = html_quote_string(value);
		fprintf(stream,"<td>%s</td>\n", *s ? s : "&nbsp");
		g_free(s);
	    }
	    fprintf(stream,"</tr>\n");
	}
	fprintf(stream,"</table>\n");
	fprintf(stream,"</BODY>\n");

	(*fname == '|' ? pclose : fclose)(stream);
    } else {
	file_open_error(fname, errno, "write");
    }
}

void
export_as_text(
    const char *fname, 
    GtkCList *clist)
{
    char	 *buf1;
    FILE	 *stream;
    int		 ncol;
    int		 nrow;
    int		 *max_len;

    assert(fname && *fname);

    if (*fname == '|')
	stream = popen(fname+1, "w");
    else
	stream=fopen(fname,"w");

    if (stream) {
	max_len = g_malloc(sizeof(int) * clist->columns);

	for (ncol = 0; ncol < clist->columns; ncol++) {
	    max_len[ncol] = 
		strlen(gtk_clist_get_column_title (clist, ncol));
	}
	for (ncol = 0; ncol < clist->columns; ncol++) {
	    for(nrow = 0;nrow < clist->rows; nrow++){
		int l;
		gtk_clist_get_text(clist, nrow, ncol, &buf1);
		l = buf1 ? strlen(buf1) : 0;
		if (l > max_len[ncol])
		    max_len[ncol] = l;
	    }
	}

	for (ncol = 0; ncol < clist->columns; ncol++) {
	    char *str = g_malloc(max_len[ncol]+1);
	    memset(str, '-', max_len[ncol]);
	    str[max_len[ncol]] = '\0';
	    fprintf(stream,"%s-+", str);
	    g_free(str);
	}
	fprintf(stream,"\n|");

	for (ncol = 0; ncol < clist->columns; ncol++) {
	    fprintf(stream,"%*s |",
		    -max_len[ncol],
		    gtk_clist_get_column_title (clist, ncol));
	}

	fprintf(stream,"\n+");
	for (ncol = 0; ncol < clist->columns; ncol++) {
	    char *str = g_malloc(max_len[ncol]+1);
	    memset(str, '-', max_len[ncol]);
	    str[max_len[ncol]] = '\0';
	    fprintf(stream,"%s-+", str);
	    g_free(str);
	}

	fprintf(stream, "\n");
	
	for(nrow = 0;nrow < clist->rows; nrow++){
	    fprintf(stream,"|");
	    for(ncol = 0; ncol < clist->columns; ncol++){
		gtk_clist_get_text(clist, nrow, ncol, &buf1);
		fprintf(stream,"%*s |", max_len[ncol], buf1);
	    }
	    fprintf(stream,"\n");
	}
	fprintf(stream,"+");
	for (ncol = 0; ncol < clist->columns; ncol++) {
	    char *str = g_malloc(max_len[ncol]+1);
	    memset(str, '-', max_len[ncol]);
	    str[max_len[ncol]] = '\0';
	    fprintf(stream,"%s-+", str);
	    g_free(str);
	}
	
	(*fname == '|' ? pclose : fclose)(stream);
    } else {
	file_open_error(fname, errno, "write");
    }
}

/* Save out the contents of a clist as a set of insert statements */
/* FIXME - check error code from writes */
void
export_as_sql(
    const char *fname, 
    GtkCList *clist)
{
    char      *value;
    FILE      *stream;
    int	      ncol=0;
    int	      nrow=0;
    char      *tablename = gtk_object_get_data(GTK_OBJECT(clist), "tablename");
    char      *owner	 = gtk_object_get_data(GTK_OBJECT(clist), "owner");
    int       do_create	 = (int)gtk_object_get_data(GTK_OBJECT(clist), "create");
    char      *sep;
    int	      coltype;
    int	      colsize;
    time_t    tnow = time(0);

    assert(fname && *fname);
    assert(tablename);

    if (*fname == '|')
	stream = popen(fname+1, "w");
    else
	stream=fopen(fname,"w");

    if (stream) {
	fprintf(stream, "/* %s%s%s data exported by %s on %s */\n\n",
		owner ? owner : "", owner ? "." : "",
		tablename, g_get_user_name(), ctime(&tnow));
	if (do_create) {
	    char *sep = "";
	    if (owner)
		fprintf(stream, "create table %s.%s (\n", owner, tablename);
	    else
		fprintf(stream, "create table %s (\n", tablename);

	    for(ncol = 0; ncol < clist->columns; ncol++){
		fprintf(stream, "%s\t%s", sep, gtk_clist_get_column_title(clist, ncol));
		coltype = 
		    (int) gtk_object_get_data(GTK_OBJECT(gtk_clist_get_column_widget (clist, ncol)), 
					      "coltype");
		colsize = 
		    (int) gtk_object_get_data(GTK_OBJECT(gtk_clist_get_column_widget (clist, ncol)), 
					      "colsize");
		switch (coltype) {
		case COLTYPE_CHAR:
		    fprintf(stream, "\t CHAR(%d)", colsize); break;
		case COLTYPE_VARCHAR:
		    fprintf(stream, "\t VARCHAR(%d)", colsize); break;
		case COLTYPE_NCHAR:
		    fprintf(stream, "\t NCHAR(%d)", colsize); break;
		case COLTYPE_NVARCHAR:
		    fprintf(stream, "\t NVARCHAR(%d)", colsize); break;
		case COLTYPE_INT:
		case COLTYPE_UINT:
		    fprintf(stream, "\t INT"); break;
		case COLTYPE_FLOAT:
		    fprintf(stream, "\t FLOAT"); break;
		case COLTYPE_DECIMAL:
		    fprintf(stream, "\t DECIMAL"); break;
		case COLTYPE_BLOB:
		    fprintf(stream, "\t BLOB"); break;
		case COLTYPE_TIMESTAMP:
		    fprintf(stream, "\t TIMESTAMP"); break;
		case COLTYPE_DATE:
		    fprintf(stream, "\t DATE"); break;
		case COLTYPE_DATETIME:
		    fprintf(stream, "\t DATETIME"); break;
		case COLTYPE_TIME:
		    fprintf(stream, "\t TIME"); break;
		case COLTYPE_ROWID:
		    fprintf(stream, "\t ROWID"); break;
		}
		sep = ",\n";
	    }
	    fprintf(stream, ");\n\n");
	}
	while(nrow < clist->rows){
	    if (owner)
		fprintf(stream, "insert into %s.%s values (", owner, tablename);
	    else
		fprintf(stream, "insert into %s values (", tablename);
	    sep = "";
	    for(ncol = 0; ncol < clist->columns; ncol++){
		gtk_clist_get_text(clist, nrow, ncol, &value);
		coltype = 
		    (int) gtk_object_get_data(GTK_OBJECT(gtk_clist_get_column_widget (clist, ncol)), 
					      "coltype");
		/* I hope no-one actually sets a text column to
		   '<NULL>' because they're going to get a surprise if
		   they do.  Unfortunately we can't attach meta data
		   to the individual cells in a clist so we don't have
		   anywhere to say whether the value was NULL or not */
		if (strcmp(value, "<NULL>") == 0) {
		    fprintf(stream,"%sNULL", sep);
		} else {
		    switch (coltype) {
		    case COLTYPE_CHAR: 
		    case COLTYPE_VARCHAR:
		    {
			char *s = g_malloc(strlen(value)*2+3);
			char *p = s;
			int c;

			*p++ = '\'';
			while ((c = *value++) != '\0') {
			    if (c == '\'') {
				*p++ = '\'';
			    }
			    *p++ = c;
			}
			*p++ = '\'';
			*p++ = '\0';
			fprintf(stream,"%s%s", sep, s);
			g_free(s);
			break;
		    }
		    default:
			fprintf(stream,"%s%s", sep, value);
		    }
		}
		sep = ", ";
	    }
	    fprintf(stream,");\n");
	    nrow += 1;
	}

	(*fname == '|' ? pclose : fclose)(stream);
    } else {
	file_open_error(fname, errno, "write");
    }
}

void
export_as_postscript(
    const char	 *fname, 
    GtkCList	 *clist)
{
    char	 *value;
    FILE	 *stream;
    int		 ncol;
    int		 nrow;
    time_t	 tnow	  = time(0);
    int          pages = clist->rows/66;
    int		 ll;

    assert(fname && *fname);

    // This is a temporary measure while the code below is being developed
    show_warning_msg("postscript output not available yet");
    return;

    if (*fname == '|')
	stream = popen(fname+1, "w");
    else
	stream=fopen(fname,"w");

    if (stream) {
	fprintf(stream,"%%!PS-Adobe-2.0\n");
	fprintf(stream,"%%%%DocumentFonts: Courier Times-Bold\n");
	fprintf(stream,"%%%%Title: SQLWork query output\n");
	fprintf(stream,"%%%%Creator: %s\n", sqlwork_version());
	fprintf(stream,"%%%%CreationDate: %s\n", ctime(&tnow));
	fprintf(stream,"%%%%Orientation: Portrait\n");
	fprintf(stream,"%%%%DocumentMedia: Letter 612 792\n");
	fprintf(stream,"%%%%BoundingBox: 18 18 594 774\n");
	fprintf(stream,"%%%%Pages: %d\n", pages);
	fprintf(stream,"%%%%EndComments\n");

	fprintf(stream,"%%%%Page 1\n");

	ll = 3;
	fprintf(stream,"[ [");
	for (ncol = 0; ncol < clist->columns; ncol++) {
	    char *s = ps_quote_string(gtk_clist_get_column_title (clist, ncol));
	    int l = strlen(s)+2;

	    ll += l;
	    if (ll > 80) {
		fprintf(stream,"\n");
		ll = l;
	    }
	    fprintf(stream,"(%s)", s);
	    g_free(s);
	}
	
	fprintf(stream,"]");
	
	for(nrow = 0;nrow < clist->rows; nrow++){
	    fprintf(stream,"\n");
	    ll = 1;
	    fprintf(stream,"[");
	    for(ncol = 0; ncol < clist->columns; ncol++){
		char *s;
		int l;

		gtk_clist_get_text(clist, nrow, ncol, &value);
		s = ps_quote_string(value);
		ll += (l = s ? strlen(s)+2 : 2);
		if (ll > 80) {
		    fprintf(stream,"\n");
		    ll = l;
		}
		fprintf(stream,"(%s)", *s ? s : "");
		g_free(s);
	    }
	    fprintf(stream,"]\n");
	    ll = 0;
	}
	fprintf(stream,"]\n");

	(*fname == '|' ? pclose : fclose)(stream);
    } else {
	file_open_error(fname, errno, "write");
    }
}
