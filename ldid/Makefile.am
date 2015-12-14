bin_PROGRAMS = ldid
ldid_SOURCES = ldid.cpp

ldid_CPPFLAGS = -std=c++0x -I$(top_srcdir)/libhelper $(CRYPTO_CFLAGS)

ldid_LDADD          = liblookup2.la $(top_builddir)/libhelper/libhelper.la $(CRYPTO_LIBS)

noinst_LTLIBRARIES  = liblookup2.la  
liblookup2_la_SOURCES  = lookup2.c
liblookup2_la_CFLAGS = -I$(top_srcdir)/libhelper $(CRYPTO_CFLAGS) 

EXTRA_DIST = sha1.h ldid.hpp
