if HAVE_LOCAL_LIBHMAC
AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@

noinst_LTLIBRARIES = libhmac.la

libhmac_la_SOURCES = \
	libhmac_byte_stream.h \
	libhmac_definitions.h \
	libhmac_extern.h \
	libhmac_error.c libhmac_error.h \
	libhmac_libcerror.h \
	libhmac_md5.c libhmac_md5.h \
	libhmac_sha1.c libhmac_sha1.h \
	libhmac_sha256.c libhmac_sha256.h \
	libhmac_support.c libhmac_support.h \
	libhmac_types.h \
	libhmac_unused.h
endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libhmac ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libhmac_la_SOURCES)

