#AM_CPPFLAGS = $(XML_CFLAGS)

noinst_LTLIBRARIES = libhelper.la
noinst_HEADERS = helper.h

libhelper_la_LIBADD = $(XML_LIBS) $(PLIST_LIBS)
libhelper_la_CPPFLAGS = $(XML_CFLAGS) $(PLIST_CFLAGS)

libhelper_la_SOURCES = 	\
	xmlutil.cpp	\
	stringutil.cpp	\
	pathutil.cpp	\
	sdkutil.cpp
