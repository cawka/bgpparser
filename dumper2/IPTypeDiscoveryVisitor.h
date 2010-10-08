
#ifndef _IPTYPEDISCOVERYVISITOR_H_
#define _IPTYPEDISCOVERYVISITOR_H_

#include <visitors/GJNoArguDepthFirst.h>

enum mrt_type { TYPE_UNKNOWN, TYPE_IPv6, TYPE_IPv4 };

class IPTypeDiscoveryVisitor : public GJNoArguDepthFirst
{
public:
	virtual boost::any visit( MRTCommonHeader &n );
	virtual boost::any visit( MRTBgp4MPMessage &n );
	virtual boost::any visit( MRTBgp4MPStateChange &n );
	virtual boost::any visit( MRTTblDump &n );
	virtual boost::any visit( MRTTblDumpV2PeerIndexTbl &n );
	virtual boost::any visit( MRTTblDumpV2RibHeader &n );
};

#endif
