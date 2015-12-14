bin_PROGRAMS = ldid
ldid_SOURCES = \
	       ldid.cpp	\
	       lookup2.c

ldid_CPPFLAGS = -std=c++0x -I$(top_srcdir)/libhelper
ldid_LDADD = $(top_builddir)/libhelper/libhelper.la -lcrypto

EXTRA_DIST =	\
	sha1.h
