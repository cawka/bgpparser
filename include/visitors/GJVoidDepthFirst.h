
#ifndef _GJVOIDDEPTHFIRST_H_
#define _GJVOIDDEPTHFIRST_H_

#include "GJVoidVisitor.h"

#include "parsers/MRTCommonHeader.h"

#include "parsers/MRTBgp4MPStateChange.h"
#include "parsers/MRTBgp4MPStateChangeAS4.h"
#include "parsers/MRTBgp4MPMessage.h"
#include "parsers/MRTBgp4MPMessageAS4.h"

#include "parsers/MRTTblDump.h"
#include "parsers/MRTTblDumpV2PeerIndexTbl.h"
#include "parsers/MRTTblDumpV2RibHeader.h"
#include "parsers/MRTTblDumpV2RibIPv4Unicast.h"
#include "parsers/MRTTblDumpV2RibIPv4Multicast.h"
#include "parsers/MRTTblDumpV2RibIPv6Unicast.h"
#include "parsers/MRTTblDumpV2RibIPv6Multicast.h"
#include "parsers/MRTTblDumpV2RibGeneric.h"

#include "parsers/BGPCommonHeader.h"

#include "parsers/BGPOpen.h"
#include "parsers/BGPUpdate.h"
#include "parsers/BGPKeepAlive.h"
#include "parsers/BGPNotification.h"
#include "parsers/BGPRouteRefresh.h"

#include "parsers/AttributeType.h"

#include "parsers/AttributeTypeAggregator.h"
#include "parsers/AttributeTypeAS4Aggregator.h"
#include "parsers/AttributeTypeASPath.h"
#include "parsers/AttributeTypeAS4Path.h"
#include "parsers/AttributeTypeAtomicAggregate.h"
#include "parsers/AttributeTypeCommunities.h"
#include "parsers/AttributeTypeOriginatorID.h"
#include "parsers/AttributeTypeClusterList.h"
#include "parsers/AttributeTypeExtCommunities.h"
#include "parsers/AttributeTypeLocalPref.h"
#include "parsers/AttributeTypeMultiExitDisc.h"
#include "parsers/AttributeTypeNextHop.h"
#include "parsers/AttributeTypeOrigin.h"
#include "parsers/AttributeTypeMPReachNLRI.h"
#include "parsers/AttributeTypeMPUnreachNLRI.h"

class GJVoidDepthFirst : public GJVoidVisitor
{
public:
	virtual void visit( MRTCommonHeader &n, boost::any param )
	{
//		n.getTimestamp( )
//		n.getType( )
//		n.getSubType( )
//		n.getLength( )
	}

	virtual void visit( MRTBgp4MPMessage &n, boost::any param )
	{
//		n.getTimestamp( )
//		n.getType( )
//		n.getSubType( )
//		n.getLength( )

//		n.getPeerAS()
//		n.getLocalAS()
//		n.getInterfaceIndex()
//		n.getAddressFamily();
//		n.getPeerIP()
//		n.getLocalIP()

		n.getPayload()->accept( *this, param );
	}

	virtual void visit( MRTBgp4MPMessageAS4 &n, boost::any param )
	{
		static_cast<MRTBgp4MPMessage>( n ).accept( *this, param );
	}

	virtual void visit( MRTBgp4MPStateChange &n, boost::any param )
	{
//		n.getTimestamp( )
//		n.getType( )
//		n.getSubType( )
//		n.getLength( )

//		n.getPeerAS( )
//		n.getLocalAS( )
//		n.getInterfaceIndex( )
//		n.getAddressFamily( )
//		n.getPeerIP( )
//		n.getLocalIP( )
//		n.getOldState( )
//		n.getNewState( )
	}

	virtual void visit( MRTBgp4MPStateChangeAS4 &n, boost::any param )
	{
		static_cast<MRTBgp4MPStateChange>( n ).accept( *this, param );
	}

	virtual void visit( MRTTblDump &n, boost::any param )
	{
//		n.getTimestamp( )
//		n.getType( )
//		n.getSubType( )
//		n.getLength( )

//		n.getViewNumber( )
//		n.getSequenceNumber( )
//		n.getPrefix( )
//		n.getPrefixLength( )
//		n.getStatus( )
//		n.getOriginatedTime( )
//		n.getPeerIP( )
//		n.getPeerAS( )
//		n.getAttributeLength( )

		BOOST_FOREACH( const BGPAttributePtr &attr, n.getAttributes() )
		{
			attr->accept( *this, param );
		}
	}

	virtual void visit( MRTTblDumpV2PeerIndexTbl &n, boost::any param )
	{
//		n.getTimestamp( )
//		n.getType( )
//		n.getSubType( )
//		n.getLength( )

//		n.getCollectorBGPId( )
//		n.getViewNameLength( )
//		n.getPeerCount( )
//		n.getPeerEntries( )
	}

	virtual void visit( MRTTblDumpV2RibHeader &n, boost::any param )
	{
//		n.getTimestamp( )
//		n.getType( )
//		n.getSubType( )
//		n.getLength( )

//		n.getSequenceNumber( )
//		n.getPrefixLength( )
//		n.getPrefix( )
//		n.getEntryCount( )
//		n.getAFI( )
//		n.getSAFI( )

//		n.getPeerIndexTbl()

		BOOST_FOREACH( const TblDumpV2RibEntryPtr &entry, n.getRibEntries() )
		{
			entry->accept( *this, param );
		}
	}

	virtual void visit( MRTTblDumpV2RibGeneric &n, boost::any param )
	{
		static_cast<MRTTblDumpV2RibHeader>( n ).accept( *this, param );
	}
	virtual void visit( MRTTblDumpV2RibIPv4Multicast &n, boost::any param )
	{
		static_cast<MRTTblDumpV2RibHeader>( n ).accept( *this, param );
	}
	virtual void visit( MRTTblDumpV2RibIPv4Unicast &n, boost::any param )
	{
		static_cast<MRTTblDumpV2RibHeader>( n ).accept( *this, param );
	}
	virtual void visit( MRTTblDumpV2RibIPv6Multicast &n, boost::any param )
	{
		static_cast<MRTTblDumpV2RibHeader>( n ).accept( *this, param );
	}
	virtual void visit( MRTTblDumpV2RibIPv6Unicast &n, boost::any param )
	{
		static_cast<MRTTblDumpV2RibHeader>( n ).accept( *this, param );
	}

	virtual void visit( TblDumpV2RibEntry &n, boost::any param )
	{
//		n.getPeerIndex( )
//		n.getOriginatedTime( )
//		n.getAttributeLength( )

		BOOST_FOREACH( const BGPAttributePtr &attr, n.getAttributes() )
		{
			attr->accept( *this, param );
		}
	}

	virtual void visit( BGPCommonHeader &n, boost::any param )
	{
//		n.getMarker()
//		n.getLength()
//		n.getType()
	}

	virtual void visit( BGPKeepAlive &n, boost::any param )
	{
//		n.getMarker()
//		n.getLength()
//		n.getType()
	}

	virtual void visit( BGPNotification &n, boost::any param )
	{
//		n.getMarker()
//		n.getLength()
//		n.getType()

//		n.getErrorCode( )
//		n.getSubErrorCode( )
//		n.getData( )
	}

	virtual void visit( BGPOpen &n, boost::any param )
	{
//		n.getMarker()
//		n.getLength()
//		n.getType()

//		n.getVersion( )
//		n.getMyAS( )
//		n.getHoldTime( )
//		n.getBgpId( )
//		n.getOptParmLen( )
//		n.isAS4Supported( )

		BOOST_FOREACH( const OptionalParameterPtr &optparam, n.getOptParams( ) )
		{
			optparam->accept( *this, param );
		}
	}
	virtual void visit( BGPRouteRefresh &n, boost::any param )
	{
//		n.getMarker()
//		n.getLength()
//		n.getType()

//		n.getAfi( )
//		n.getRes( )
//		n.getSafi( )
	}
	virtual void visit( BGPUpdate &n, boost::any param )
	{
//		n.getMarker()
//		n.getLength()
//		n.getType()

//		n.getWithdrawnRoutesLength()
//		n.getPathAttributesLength()
//		n.getNlriLength()

		BOOST_FOREACH( const BGPAttributePtr &attr, n.getPathAttributes() )
		{
			attr->accept( *this, param );
		}

		BOOST_FOREACH( const RoutePtr &route, n.getWithdrawnRoutes() )
		{
			route->accept( *this, param );
		}

		BOOST_FOREACH( const RoutePtr &route, n.getNlriRoutes() )
		{
			route->accept( *this, param );
		}
	}

	virtual void visit( BGPAttribute &n, boost::any param )
	{
//		n.getAttributeFlags( )
//		n.getAttributeTypeCode( )
//		n.getAttributeLength( )
//		+ other helpers available, see BGPAttribute

		n.getAttributeValue()->accept( *this, param );
	}

	virtual void visit( AttributeType &n, boost::any param )
	{
//		n.getLength( )
//		n.getData( )
//		n.getAS4( )
	}
	virtual void visit( AttributeTypeAS4Aggregator &n, boost::any param )
	{
//		n.getLength( )
//		n.getData( )
//		n.getAS4( )

//		n.getAggregatorLastAS( )
//		n.getAggregatorBGPSpeakerIPAddress( )
	}
	virtual void visit( AttributeTypeASPath &n, boost::any param )
	{
//		n.getLength( )
//		n.getData( )
//		n.getAS4( )

//		+ n.getPathSegmentsComplete() is also available
		BOOST_FOREACH( const AttributeTypeASPathSegmentPtr &segment, n.getPathSegments() )
		{
			segment->accept( *this, param );
		}
	}
	virtual void visit( AttributeTypeASPathSegment &n, boost::any param )
	{
//		n.getPathSegmentType( )
//		n.getPathSegmentLength( )
//		n.getPathSegmentValue( )
	}
	virtual void visit( AttributeTypeAS4Path &n, boost::any param )
	{
//		n.getLength( )
//		n.getData( )
//		n.getAS4( )

		BOOST_FOREACH( const AttributeTypeASPathSegmentPtr &segment, n.getPathSegments() )
		{
			segment->accept( *this, param );
		}
	}
	virtual void visit( AttributeTypeAS4PathSegment &n, boost::any param )
	{
//		n.getPathSegmentType( )
//		n.getPathSegmentLength( )
//		n.getPathSegmentValue( )
	}
	virtual void visit( AttributeTypeAggregator &n, boost::any param )
	{
//		n.getLength( )
//		n.getData( )
//		n.getAS4( )

//		n.getAggregatorLastAS( )
//		n.getAggregatorLastASComplete( )
//		n.getAggregatorBGPSpeakerIPAddress( )
	}
	virtual void visit( AttributeTypeAtomicAggregate &n, boost::any param )
	{
//		n.getLength( )
//		n.getData( )
//		n.getAS4( )
	}
	virtual void visit( AttributeTypeClusterList &n, boost::any param )
	{
//		n.getLength( )
//		n.getData( )
//		n.getAS4( )

//		n.getClusterList( )
	}
	virtual void visit( AttributeTypeCommunities &n, boost::any param )
	{
//		n.getLength( )
//		n.getData( )
//		n.getAS4( )

//		n.getCommunityValues( )
	}
	virtual void visit( AttributeTypeExtCommunities &n, boost::any param )
	{
//		n.getLength( )
//		n.getData( )
//		n.getAS4( )

//		n.getExtCommunityValues( )
	}
	virtual void visit( AttributeTypeLocalPref &n, boost::any param )
	{
//		n.getLength( )
//		n.getData( )
//		n.getAS4( )

//		n.getLocalPrefValue( )
	}
	virtual void visit( AttributeTypeMPReachNLRI &n, boost::any param )
	{
//		n.getLength( )
//		n.getData( )
//		n.getAS4( )

//		n.getAFI( )
//		n.getSAFI( )
//		n.getNextHopAddressLength( )
//		n.getNextHopAddress( )
//		n.getNextHopAddressLocal( )

		BOOST_FOREACH( const NLRIReachablePtr& route, n.getNLRI() )
		{
			route->accept( *this, param );
		}

		BOOST_FOREACH( const NLRIReachablePtr& route, n.getSNPA() )
		{
			route->accept( *this, param );
		}
	}
	virtual void visit( AttributeTypeMPUnreachNLRI &n, boost::any param )
	{
//		n.getLength( )
//		n.getData( )
//		n.getAS4( )

//		n.getAFI( )
//		n.getSAFI( )

		BOOST_FOREACH( const NLRIUnReachablePtr &route, n.getNLRI() )
		{
			route->accept( *this, param );
		}
	}
	virtual void visit( AttributeTypeMultiExitDisc &n, boost::any param )
	{
//		n.getLength( )
//		n.getData( )
//		n.getAS4( )

//		n.getMultiExitDiscValue( )
	}
	virtual void visit( AttributeTypeNextHop &n, boost::any param )
	{
//		n.getLength( )
//		n.getData( )
//		n.getAS4( )

//		n.getNextHopIPAddress( )
//		n.getNextHopAFI( )
	}
	virtual void visit( AttributeTypeOrigin &n, boost::any param )
	{
//		n.getLength( )
//		n.getData( )
//		n.getAS4( )

//		n.getOrigin( )
	}
	virtual void visit( AttributeTypeOriginatorID &n, boost::any param )
	{
//		n.getLength( )
//		n.getData( )
//		n.getAS4( )

//		n.getOrigin( )
	}

	virtual void visit( OptionalParameter &n, boost::any param )
	{
//		n.getType( )
//		n.getLength( )
	}
	virtual void visit( OptionalParameterCapabilities &n, boost::any param )
	{
//		n.getType( )
//		n.getLength( )

//		n.getCapCode( )
//		n.getCapLength( )
//		n.getCapValue( )
	}

	virtual void visit( Route &n, boost::any param )
	{
//		n.getLength( )
//		n.getPrefix( )
//		n.getNumOctets( )
//		+ helper methods available, see Route
	}

	virtual void visit( NLRIReachable &n, boost::any param )
	{
//		n.getLength( )
//		n.getPrefix( )
//		n.getNumOctets( )
//		+ helper methods available, see Route
	}
	virtual void visit( NLRIUnReachable &n, boost::any param )
	{
//		n.getLength( )
//		n.getPrefix( )
//		n.getNumOctets( )
//		+ helper methods available, see Route
	}
};

#endif
