AM_CPPFLAGS=-I$(top_srcdir)/include -I$(top_srcdir)/include/parsers -Iinclude \
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
AttributeTypeASPathDumper.cpp \
AttributeTypeAggregatorDumper.cpp \
AttributeTypeAtomicAggregateDumper.cpp \
AttributeTypeClusterListDumper.cpp \
AttributeTypeCommunitiesDumper.cpp \
AttributeTypeDumper.cpp \
AttributeTypeLocalPrefDumper.cpp \
AttributeTypeMPReachNLRIDumper.cpp \
AttributeTypeMPUnreachNLRIDumper.cpp \
AttributeTypeMultiExitDiscDumper.cpp \
AttributeTypeNextHopDumper.cpp \
AttributeTypeOriginDumper.cpp \
AttributeTypeOriginatorIDDumper.cpp \
BGPAttributeDumper.cpp \
BGPDumper.cpp \
BGPKeepAliveDumper.cpp \
BGPMessageDumper.cpp \
BGPNotificationDumper.cpp \
BGPOpenDumper.cpp \
BGPRouteRefreshDumper.cpp \
BGPStateChangeDumper.cpp \
BGPUpdateDumper.cpp \
Dumper.cpp \
MRTBgp4MPMessageDumper.cpp \
MRTBgp4MPStateChangeDumper.cpp \
MRTTblDumpV1Dumper.cpp \
MRTTblDumpV2Dumper.cpp \
mrt2xml.cpp \
xmlinternal.c

