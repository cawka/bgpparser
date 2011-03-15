AM_CPPFLAGS=-I$(top_srcdir)/include \
	$(log4cxx_CFLAGS) \
	$(BOOST_CPPFLAGS)

bgpparser2_LDADD   = ../parser/libbgpparser.la

AM_LDFLAGS = \
	$(log4cxx_LIBS) \
	$(BOOST_LDFLAGS) \
		$(BOOST_SYSTEM_LIB) \
		$(BOOST_FILESYSTEM_LIB) \
		$(BOOST_PROGRAM_OPTIONS_LIB) \
		$(BOOST_REGEX_LIB) \
		$(BOOST_IOSTREAMS_LIB)

#	-static-libtool-libs 

bin_PROGRAMS = bgpparser2

bgpparser2_SOURCES = \
	AsciiVisitor.cpp AsciiVisitor.h \
	AsciiBGPVisitor.cpp AsciiBGPVisitor.h \
	ASSegmentVisitor.cpp ASSegmentVisitor.h \
	IPTypeDiscoveryVisitor.cpp IPTypeDiscoveryVisitor.h \
	Params.h \
	main.cpp

