
#ifndef _SIMPLEPRINTER_H_
#define _SIMPLEPRINTER_H_

#include <visitors/DepthFirstVisitor.h>
#include <visitors/GJVoidDepthFirst.h>
#include "../dumper2/ASSegmentVisitor.h"

class SimplePrefixPrinter : public GJVoidDepthFirst
{
public:
	virtual void visit( Route &n,			boost::any param );
	virtual void visit( NLRIReachable &n, 	boost::any param );
	virtual void visit( NLRIUnReachable &n, boost::any param );
};

class SimplePrinter : public DepthFirstVisitor
{
public:
	virtual void visit( MRTBgp4MPMessage &n 				);
	virtual void visit( MRTBgp4MPStateChange &n 			);
	virtual void visit( MRTBgp4MPMessageAS4 &n 				);
	virtual void visit( MRTBgp4MPStateChangeAS4 &n 			);

	virtual void visit( MRTTblDump &n 						);

	virtual void visit( MRTTblDumpV2RibHeader &n 			);
	virtual void visit( MRTTblDumpV2PeerIndexTbl &n 		);
	virtual void visit( MRTTblDumpV2RibGeneric &n 			);
	virtual void visit( MRTTblDumpV2RibIPv4Multicast &n 	);
	virtual void visit( MRTTblDumpV2RibIPv4Unicast &n 		);
	virtual void visit( MRTTblDumpV2RibIPv6Multicast &n 	);
	virtual void visit( MRTTblDumpV2RibIPv6Unicast &n 		);

	virtual void visit( BGPKeepAlive &n 					);

	virtual void visit( BGPNotification &n 					);
	virtual void visit( BGPOpen &n 							);
	virtual void visit( BGPRouteRefresh &n					);
	virtual void visit( BGPUpdate &n 						);

	virtual void visit( AttributeTypeMPReachNLRI &n			);
	virtual void visit( AttributeTypeMPUnreachNLRI &n		);

	//////////////////////////////////////////////////////////////////////
	virtual void visit( AttributeType &n					);
	virtual void visit( AttributeTypeOrigin &n				);
	virtual void visit( AttributeTypeASPath &n				);
	virtual void visit( AttributeTypeAS4Path &n				);
	virtual void visit( AttributeTypeNextHop &n				);
	virtual void visit( AttributeTypeMultiExitDisc &n		);
	virtual void visit( AttributeTypeLocalPref &n			);
	virtual void visit( AttributeTypeAtomicAggregate &n		);
	virtual void visit( AttributeTypeAS4Aggregator &n		);
	virtual void visit( AttributeTypeAggregator &n			);
	virtual void visit( AttributeTypeCommunities &n			);

	virtual void visit( AttributeTypeClusterList &n			);
	virtual void visit( AttributeTypeExtCommunities &n		);
	virtual void visit( AttributeTypeOriginatorID &n		);

private:
	void printTimestamp( const MRTMessage &mrt );

private:
	SimplePrefixPrinter _prefixPrinter;
	ASSegmentVisitor _asSegmentVisitor;
};

#endif //_SIMPLEPRINTER_H_
