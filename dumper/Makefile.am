AM_CPPFLAGS=-I$(top_srcdir)/include \
	$(XML_CFLAGS) \
	$(log4cxx_CFLAGS) \
	$(BOOST_CPPFLAGS)

bgpparser_LDADD   = ../parser/libbgpparser.la

AM_LDFLAGS = \
	$(XML_LIBS) \
	$(log4cxx_LIBS) \
	$(BOOST_LDFLAGS) \
		$(BOOST_SYSTEM_LIB) \
		$(BOOST_FILESYSTEM_LIB) \
		$(BOOST_PROGRAM_OPTIONS_LIB) \
		$(BOOST_REGEX_LIB) \
		$(BOOST_IOSTREAMS_LIB)

bin_PROGRAMS = bgpparser

bgpparser_SOURCES = \
FakeBGPUpdate.cpp 					FakeBGPUpdate.h \
FakeBGPAttribute.cpp 				FakeBGPAttribute.h \
FakeAttributeTypeMPReachNLRI.cpp 	FakeAttributeTypeMPReachNLRI.h \
AsciiVisitor.cpp XMLVisitor.cpp \
xmlinternal.c \
main.cpp

