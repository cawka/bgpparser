AM_CPPFLAGS = -I$(top_srcdir)/include $(log4cxx_CFLAGS) $(BOOST_CPPFLAGS)

noinst_PROGRAMS = bgpparser-simple

bgpparser_simple_SOURCES = bgpparser.cpp SimplePrinter.h SimplePrinter.cpp \
						   ../dumper2/ASSegmentVisitor.h ../dumper2/ASSegmentVisitor.cpp

bgpparser_simple_LDADD   = ../parser/libbgpparser.la
bgpparser_simple_LDFLAGS = \
	$(log4cxx_LIBS) \
	$(BOOST_LDFLAGS) \
		$(BOOST_SYSTEM_LIB) \
		$(BOOST_FILESYSTEM_LIB) \
		$(BOOST_PROGRAM_OPTIONS_LIB) \
		$(BOOST_REGEX_LIB) \
		$(BOOST_IOSTREAMS_LIB)
#	-static-libtool-libs 

