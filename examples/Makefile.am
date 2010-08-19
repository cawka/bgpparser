AM_CPPFLAGS = -I$(top_srcdir)/include $(log4cxx_CFLAGS) $(BOOST_CPPFLAGS)

bin_PROGRAMS = bgpparser-simple

bgpparser_simple_SOURCES = bgpparser.cpp
bgpparser_simple_LDADD   = ../parser/libbgpparser.la
bgpparser_simple_LDFLAGS = \
	$(log4cxx_LIBS) \
	$(BOOST_LDFLAGS) \
		$(BOOST_SYSTEM_LIB) \
		$(BOOST_FILESYSTEM_LIB) \
		$(BOOST_PROGRAM_OPTIONS_LIB) \
		$(BOOST_REGEX_LIB) \
		$(BOOST_IOSTREAMS_LIB)

