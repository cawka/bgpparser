AM_CPPFLAGS=-I$(top_srcdir)/include \
	$(log4cxx_CFLAGS) \
	$(BOOST_CPPFLAGS)

bgpparser2_LDADD   = ../parser/libbgpparser.la

AM_LDFLAGS = \
	-static-libtool-libs \
	$(log4cxx_LIBS) \
	$(BOOST_LDFLAGS) \
		$(BOOST_SYSTEM_LIB) \
		$(BOOST_FILESYSTEM_LIB) \
		$(BOOST_PROGRAM_OPTIONS_LIB) \
		$(BOOST_REGEX_LIB) \
		$(BOOST_IOSTREAMS_LIB)

bin_PROGRAMS = bgpparser2

bgpparser2_SOURCES = \
AsciiVisitor.cpp AsciiBGPVisitor.cpp ASSegmentVisitor.cpp \
main.cpp

