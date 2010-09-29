AM_CPPFLAGS=-I$(top_srcdir)/include \
	$(log4cxx_CFLAGS) \
	$(BOOST_CPPFLAGS)

bgpparser_LDADD   = ../parser/libbgpparser.la

AM_LDFLAGS = \
	$(log4cxx_LIBS) \
	$(BOOST_LDFLAGS) \
		$(BOOST_SYSTEM_LIB) \
		$(BOOST_FILESYSTEM_LIB) \
		$(BOOST_PROGRAM_OPTIONS_LIB) \
		$(BOOST_REGEX_LIB) \
		$(BOOST_IOSTREAMS_LIB)

bin_PROGRAMS = bgpparser

bgpparser_SOURCES = \
AsciiVisitor.cpp AsciiBGPVisitor.cpp AsciiAttrsVisitor.cpp \
main.cpp

