
#ifndef _GJVISITOR_H_
#define _GJVISITOR_H_

#include "ForwardDeclarations.h"

class GJVisitor
{
public:
	virtual boost::any visit( MRTCommonHeader& 				, boost::any& )=0;

	virtual boost::any visit( MRTBgp4MPMessage& 				, boost::any& )=0;
	virtual boost::any visit( MRTBgp4MPMessageAS4& 			, boost::any& )=0;
	virtual boost::any visit( MRTBgp4MPStateChange& 			, boost::any& )=0;
	virtual boost::any visit( MRTBgp4MPStateChangeAS4& 		, boost::any& )=0;

	virtual boost::any visit( AttributeType& 					, boost::any& )=0;
	virtual boost::any visit( AttributeTypeAS4Aggregator& 		, boost::any& )=0;
	virtual boost::any visit( AttributeTypeAS4Path& 			, boost::any& )=0;
	virtual boost::any visit( AttributeTypeAS4PathSegment& 	, boost::any& )=0;
	virtual boost::any visit( AttributeTypeASPath& 			, boost::any& )=0;
	virtual boost::any visit( AttributeTypeASPathSegment& 		, boost::any& )=0;
	virtual boost::any visit( AttributeTypeAggregator& 		, boost::any& )=0;
	virtual boost::any visit( AttributeTypeAtomicAggregate&	, boost::any& )=0;
	virtual boost::any visit( AttributeTypeClusterList& 		, boost::any& )=0;
	virtual boost::any visit( AttributeTypeCommunities& 		, boost::any& )=0;
	virtual boost::any visit( AttributeTypeExtCommunities& 	, boost::any& )=0;
	virtual boost::any visit( AttributeTypeLocalPref& 			, boost::any& )=0;
	virtual boost::any visit( AttributeTypeMPReachNLRI& 		, boost::any& )=0;
	virtual boost::any visit( AttributeTypeMPUnreachNLRI& 		, boost::any& )=0;
	virtual boost::any visit( AttributeTypeMultiExitDisc& 		, boost::any& )=0;
	virtual boost::any visit( AttributeTypeNextHop& 			, boost::any& )=0;
	virtual boost::any visit( AttributeTypeOrigin& 			, boost::any& )=0;
	virtual boost::any visit( AttributeTypeOriginatorID& 		, boost::any& )=0;

	virtual boost::any visit( BGPCommonHeader& 				, boost::any& )=0;
	virtual boost::any visit( BGPKeepAlive& 					, boost::any& )=0;
	virtual boost::any visit( BGPNotification& 				, boost::any& )=0;
	virtual boost::any visit( BGPOpen& 						, boost::any& )=0;
	virtual boost::any visit( BGPRouteRefresh&					, boost::any& )=0;
	virtual boost::any visit( BGPUpdate& 						, boost::any& )=0;
	virtual boost::any visit( BGPAttribute& 					, boost::any& )=0;

	virtual boost::any visit( MRTTblDump& 						, boost::any& )=0;
	virtual boost::any visit( MRTTblDumpV2PeerIndexTbl& 		, boost::any& )=0;
	virtual boost::any visit( MRTTblDumpV2RibGeneric& 			, boost::any& )=0;
	virtual boost::any visit( MRTTblDumpV2RibHeader& 			, boost::any& )=0;
	virtual boost::any visit( MRTTblDumpV2RibIPv4Multicast& 	, boost::any& )=0;
	virtual boost::any visit( MRTTblDumpV2RibIPv4Unicast& 		, boost::any& )=0;
	virtual boost::any visit( MRTTblDumpV2RibIPv6Multicast& 	, boost::any& )=0;
	virtual boost::any visit( MRTTblDumpV2RibIPv6Unicast& 		, boost::any& )=0;
	virtual boost::any visit( OptionalParameter& 				, boost::any& )=0;
	virtual boost::any visit( OptionalParameterCapabilities& 	, boost::any& )=0;
	virtual boost::any visit( TblDumpV2RibEntry& 				, boost::any& )=0;

	virtual boost::any visit( Route& 							, boost::any& )=0;
};

#endif
