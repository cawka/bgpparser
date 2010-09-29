
#ifndef _GJVOIDVISITOR_H_
#define _GJVOIDVISITOR_H_

#include "ForwardDeclarations.h"

class GJVoidVisitor
{
public:
	virtual void visit( MRTCommonHeader& 				, boost::any& )=0;

	virtual void visit( MRTBgp4MPMessage& 				, boost::any& )=0;
	virtual void visit( MRTBgp4MPMessageAS4& 			, boost::any& )=0;
	virtual void visit( MRTBgp4MPStateChange& 			, boost::any& )=0;
	virtual void visit( MRTBgp4MPStateChangeAS4& 		, boost::any& )=0;

	virtual void visit( AttributeType& 					, boost::any& )=0;
	virtual void visit( AttributeTypeAS4Aggregator& 	, boost::any& )=0;
	virtual void visit( AttributeTypeAS4Path& 			, boost::any& )=0;
	virtual void visit( AttributeTypeAS4PathSegment& 	, boost::any& )=0;
	virtual void visit( AttributeTypeASPath& 			, boost::any& )=0;
	virtual void visit( AttributeTypeASPathSegment& 	, boost::any& )=0;
	virtual void visit( AttributeTypeAggregator& 		, boost::any& )=0;
	virtual void visit( AttributeTypeAtomicAggregate&	, boost::any& )=0;
	virtual void visit( AttributeTypeClusterList& 		, boost::any& )=0;
	virtual void visit( AttributeTypeCommunities& 		, boost::any& )=0;
	virtual void visit( AttributeTypeExtCommunities& 	, boost::any& )=0;
	virtual void visit( AttributeTypeLocalPref& 		, boost::any& )=0;
	virtual void visit( AttributeTypeMPReachNLRI& 		, boost::any& )=0;
	virtual void visit( AttributeTypeMPUnreachNLRI& 	, boost::any& )=0;
	virtual void visit( AttributeTypeMultiExitDisc& 	, boost::any& )=0;
	virtual void visit( AttributeTypeNextHop& 			, boost::any& )=0;
	virtual void visit( AttributeTypeOrigin& 			, boost::any& )=0;
	virtual void visit( AttributeTypeOriginatorID& 		, boost::any& )=0;

	virtual void visit( BGPCommonHeader& 				, boost::any& )=0;
	virtual void visit( BGPKeepAlive& 					, boost::any& )=0;
	virtual void visit( BGPNotification& 				, boost::any& )=0;
	virtual void visit( BGPOpen& 						, boost::any& )=0;
	virtual void visit( BGPRouteRefresh&				, boost::any& )=0;
	virtual void visit( BGPUpdate& 						, boost::any& )=0;
	virtual void visit( BGPAttribute& 					, boost::any& )=0;

	virtual void visit( MRTTblDump& 					, boost::any& )=0;
	virtual void visit( MRTTblDumpV2PeerIndexTbl& 		, boost::any& )=0;
	virtual void visit( MRTTblDumpV2RibGeneric& 		, boost::any& )=0;
	virtual void visit( MRTTblDumpV2RibHeader& 			, boost::any& )=0;
	virtual void visit( MRTTblDumpV2RibIPv4Multicast& 	, boost::any& )=0;
	virtual void visit( MRTTblDumpV2RibIPv4Unicast& 	, boost::any& )=0;
	virtual void visit( MRTTblDumpV2RibIPv6Multicast& 	, boost::any& )=0;
	virtual void visit( MRTTblDumpV2RibIPv6Unicast& 	, boost::any& )=0;
	virtual void visit( OptionalParameter& 				, boost::any& )=0;
	virtual void visit( OptionalParameterCapabilities& 	, boost::any& )=0;
	virtual void visit( TblDumpV2RibEntry& 				, boost::any& )=0;

	virtual void visit( Route& 							, boost::any& )=0;
};

#endif
