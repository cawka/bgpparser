
#ifndef _GJNOARGUVISITOR_H_
#define _GJNOARGUVISITOR_H_

#include "ForwardDeclarations.h"

class GJNoArguVisitor
{
public:
	virtual boost::any visit( MRTCommonHeader& 			)=0;

	virtual boost::any visit( MRTBgp4MPMessage& 		)=0;
	virtual boost::any visit( MRTBgp4MPMessageAS4& 		)=0;
	virtual boost::any visit( MRTBgp4MPStateChange& 	)=0;
	virtual boost::any visit( MRTBgp4MPStateChangeAS4& 	)=0;

	virtual boost::any visit( AttributeType& 			)=0;
	virtual boost::any visit( AttributeTypeAS4Aggregator& 	)=0;
	virtual boost::any visit( AttributeTypeAS4Path& 	)=0;
	virtual boost::any visit( AttributeTypeAS4PathSegment& )=0;
	virtual boost::any visit( AttributeTypeASPath& 		)=0;
	virtual boost::any visit( AttributeTypeASPathSegment& )=0;
	virtual boost::any visit( AttributeTypeAggregator& 	)=0;
	virtual boost::any visit( AttributeTypeAtomicAggregate& )=0;
	virtual boost::any visit( AttributeTypeClusterList& )=0;
	virtual boost::any visit( AttributeTypeCommunities& )=0;
	virtual boost::any visit( AttributeTypeExtCommunities& )=0;
	virtual boost::any visit( AttributeTypeLocalPref& 	)=0;
	virtual boost::any visit( AttributeTypeMPReachNLRI& )=0;
	virtual boost::any visit( AttributeTypeMPUnreachNLRI& )=0;
	virtual boost::any visit( AttributeTypeMultiExitDisc& )=0;
	virtual boost::any visit( AttributeTypeNextHop& 	)=0;
	virtual boost::any visit( AttributeTypeOrigin& 		)=0;
	virtual boost::any visit( AttributeTypeOriginatorID& )=0;

	virtual boost::any visit( BGPCommonHeader& 			)=0;
	virtual boost::any visit( BGPKeepAlive& 			)=0;
	virtual boost::any visit( BGPNotification& 			)=0;
	virtual boost::any visit( BGPOpen& 					)=0;
	virtual boost::any visit( BGPRouteRefresh&			)=0;
	virtual boost::any visit( BGPUpdate& 				)=0;
	virtual boost::any visit( BGPAttribute& 			)=0;

	virtual boost::any visit( MRTTblDump& 				)=0;
	virtual boost::any visit( MRTTblDumpV2PeerIndexTbl& )=0;
	virtual boost::any visit( MRTTblDumpV2RibGeneric& 	)=0;
	virtual boost::any visit( MRTTblDumpV2RibHeader& 	)=0;
	virtual boost::any visit( MRTTblDumpV2RibIPv4Multicast& )=0;
	virtual boost::any visit( MRTTblDumpV2RibIPv4Unicast& )=0;
	virtual boost::any visit( MRTTblDumpV2RibIPv6Multicast& )=0;
	virtual boost::any visit( MRTTblDumpV2RibIPv6Unicast& )=0;
	virtual boost::any visit( OptionalParameter& 		)=0;
	virtual boost::any visit( OptionalParameterCapabilities& )=0;
	virtual boost::any visit( TblDumpV2RibEntry& 		)=0;

	virtual boost::any visit( Route& )=0;
	virtual boost::any visit( NLRIReachable&	 		)=0;
	virtual boost::any visit( NLRIUnReachable& 			)=0;
};

#endif
