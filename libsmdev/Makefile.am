if HAVE_LOCAL_LIBSMDEV
AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@

noinst_LTLIBRARIES = libsmdev.la

libsmdev_la_SOURCES = \
	libsmdev_ata.c libsmdev_ata.h \
	libsmdev_codepage.h \
	libsmdev_debug.c libsmdev_debug.h \
	libsmdev_definitions.h \
	libsmdev_error.c libsmdev_error.h \
	libsmdev_extern.h \
	libsmdev_handle.c libsmdev_handle.h \
	libsmdev_libcdata.h \
	libsmdev_libcerror.h \
	libsmdev_libcfile.h \
	libsmdev_libclocale.h \
	libsmdev_libcnotify.h \
	libsmdev_libcstring.h \
	libsmdev_libuna.h \
	libsmdev_metadata.c libsmdev_metadata.h \
	libsmdev_notify.c libsmdev_notify.h \
	libsmdev_optical_disc.c libsmdev_optical_disc.h \
	libsmdev_scsi.c libsmdev_scsi.h \
	libsmdev_sector_range.c libsmdev_sector_range.h \
	libsmdev_string.c libsmdev_string.h \
	libsmdev_support.c libsmdev_support.h \
	libsmdev_track_value.c libsmdev_track_value.h \
	libsmdev_types.h \
	libsmdev_unused.h \
	libsmdev_usb.c libsmdev_usb.h

libsmdev_la_LIBADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@
endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libsmdev ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libsmdev_la_SOURCES)

