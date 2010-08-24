AM_CPPFLAGS=-I$(top_srcdir)/include

bin_PROGRAMS = bgpparser

bgpparser_SOURCES = \
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
xmlinternal.c \
cfile_tools.c	cfile_tools.h

bgpparser_LDADD   = ../../src/libbgpparser.la
bgpparser_LDFLAGS = $(XML_LIBS) -lbz2 -lz $(log4cxx_LIBS) $(BOOST_SYSTEM_LIB) $(BOOST_FILESYSTEM_LIB)
bgpparser_CPPFLAGS= -I$(top_srcdir)/include $(XML_CFLAGS)  $(log4cxx_CFLAGS)

