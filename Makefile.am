## Process this file with automake to produce Makefile.in

SUBDIRS = intl po src

install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/xpm; then \
	  $(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/pixmaps; \
	  for pixmap in $(srcdir)/xpm/*; do \
	    if test -f $$pixmap; then \
	      $(INSTALL_DATA) $$pixmap $(DESTDIR)$(pkgdatadir)/pixmaps; \
	    fi \
	  done \
	fi

dist-hook:
	if test -d xpm; then \
	  mkdir $(distdir)/xpm; \
	  for pixmap in xpm/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(distdir)/xpm; \
	    fi \
	  done \
	fi

