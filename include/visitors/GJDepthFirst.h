
#ifndef _GJDEPTHFIRST_H_
#define _GJDEPTHFIRST_H_

#include "GJVisitor.h"

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

class GJDepthFirst : public GJVisitor {
  virtual boost::any
  visit(MRTCommonHeader& n, boost::any param)
  {
    //		n.getTimestamp( )
    //		n.getType( )
    //		n.getSubType( )
    //		n.getLength( )
    return boost::any();
  }

  virtual boost::any
  visit(MRTBgp4MPMessage& n, boost::any param)
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

    return n.getPayload()->accept(*this, param);
  }

  virtual boost::any
  visit(MRTBgp4MPMessageAS4& n, boost::any param)
  {
    return static_cast<MRTBgp4MPMessage>(n).accept(*this, param);
  }

  virtual boost::any
  visit(MRTBgp4MPStateChange& n, boost::any param)
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
    return boost::any();
  }

  virtual boost::any
  visit(MRTBgp4MPStateChangeAS4& n, boost::any param)
  {
    return static_cast<MRTBgp4MPStateChange>(n).accept(*this, param);
  }

  virtual boost::any
  visit(MRTTblDump& n, boost::any param)
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

    BOOST_FOREACH (const BGPAttributePtr& attr, n.getAttributes()) {
      attr->accept(*this, param);
    }

    return boost::any();
  }

  virtual boost::any
  visit(MRTTblDumpV2PeerIndexTbl& n, boost::any param)
  {
    //		n.getTimestamp( )
    //		n.getType( )
    //		n.getSubType( )
    //		n.getLength( )

    //		n.getCollectorBGPId( )
    //		n.getViewNameLength( )
    //		n.getPeerCount( )
    //		n.getPeerEntries( )
    return boost::any();
  }

  virtual boost::any
  visit(MRTTblDumpV2RibHeader& n, boost::any param)
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

    BOOST_FOREACH (const TblDumpV2RibEntryPtr& entry, n.getRibEntries()) {
      entry->accept(*this, param);
    }
    return boost::any();
  }

  virtual boost::any
  visit(MRTTblDumpV2RibGeneric& n, boost::any param)
  {
    return static_cast<MRTTblDumpV2RibHeader>(n).accept(*this, param);
  }
  virtual boost::any
  visit(MRTTblDumpV2RibIPv4Multicast& n, boost::any param)
  {
    return static_cast<MRTTblDumpV2RibHeader>(n).accept(*this, param);
  }
  virtual boost::any
  visit(MRTTblDumpV2RibIPv4Unicast& n, boost::any param)
  {
    return static_cast<MRTTblDumpV2RibHeader>(n).accept(*this, param);
  }
  virtual boost::any
  visit(MRTTblDumpV2RibIPv6Multicast& n, boost::any param)
  {
    return static_cast<MRTTblDumpV2RibHeader>(n).accept(*this, param);
  }
  virtual boost::any
  visit(MRTTblDumpV2RibIPv6Unicast& n, boost::any param)
  {
    return static_cast<MRTTblDumpV2RibHeader>(n).accept(*this, param);
  }

  virtual boost::any
  visit(TblDumpV2RibEntry& n, boost::any param)
  {
    //		n.getPeerIndex( )
    //		n.getOriginatedTime( )
    //		n.getAttributeLength( )

    BOOST_FOREACH (const BGPAttributePtr& attr, n.getAttributes()) {
      attr->accept(*this, param);
    }

    return boost::any();
  }

  virtual boost::any
  visit(BGPCommonHeader& n, boost::any param)
  {
    //		n.getMarker()
    //		n.getLength()
    //		n.getType()
    return boost::any();
  }

  virtual boost::any
  visit(BGPKeepAlive& n, boost::any param)
  {
    //		n.getMarker()
    //		n.getLength()
    //		n.getType()
    return boost::any();
  }

  virtual boost::any
  visit(BGPNotification& n, boost::any param)
  {
    //		n.getMarker()
    //		n.getLength()
    //		n.getType()

    //		n.getErrorCode( )
    //		n.getSubErrorCode( )
    //		n.getData( )
    return boost::any();
  }

  virtual boost::any
  visit(BGPOpen& n, boost::any param)
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

    BOOST_FOREACH (const OptionalParameterPtr& optparam, n.getOptParams()) {
      optparam->accept(*this, param);
    }
    return boost::any();
  }
  virtual boost::any
  visit(BGPRouteRefresh& n, boost::any param)
  {
    //		n.getMarker()
    //		n.getLength()
    //		n.getType()

    //		n.getAfi( )
    //		n.getRes( )
    //		n.getSafi( )
    return boost::any();
  }
  virtual boost::any
  visit(BGPUpdate& n, boost::any param)
  {
    //		n.getMarker()
    //		n.getLength()
    //		n.getType()

    //		n.getWithdrawnRoutesLength()
    //		n.getPathAttributesLength()
    //		n.getNlriLength()

    BOOST_FOREACH (const BGPAttributePtr& attr, n.getPathAttributes()) {
      attr->accept(*this, param);
    }

    BOOST_FOREACH (const RoutePtr& route, n.getWithdrawnRoutes()) {
      route->accept(*this, param);
    }

    BOOST_FOREACH (const RoutePtr& route, n.getNlriRoutes()) {
      route->accept(*this, param);
    }
    return boost::any();
  }

  virtual boost::any
  visit(BGPAttribute& n, boost::any param)
  {
    //		n.getAttributeFlags( )
    //		n.getAttributeTypeCode( )
    //		n.getAttributeLength( )
    //		+ other helpers available, see BGPAttribute

    n.getAttributeValue()->accept(*this, param);
    return boost::any();
  }

  virtual boost::any
  visit(AttributeType& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getData( )
    //		n.getAS4( )
    return boost::any();
  }
  virtual boost::any
  visit(AttributeTypeAS4Aggregator& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getData( )
    //		n.getAS4( )

    //		n.getAggregatorLastAS( )
    //		n.getAggregatorBGPSpeakerIPAddress( )
    return boost::any();
  }
  virtual boost::any
  visit(AttributeTypeASPath& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getData( )
    //		n.getAS4( )

    //		+ n.getPathSegmentsComplete() is also available
    BOOST_FOREACH (const AttributeTypeASPathSegmentPtr& segment, n.getPathSegments()) {
      segment->accept(*this, param);
    }
    return boost::any();
  }

  virtual boost::any
  visit(AttributeTypeASPathSegment& n, boost::any param)
  {
    //		n.getPathSegmentType( )
    //		n.getPathSegmentLength( )
    //		n.getPathSegmentValue( )
    return boost::any();
  }
  virtual boost::any
  visit(AttributeTypeAS4PathSegment& n, boost::any param)
  {
    return static_cast<AttributeTypeASPathSegment>(n).accept(*this, param);
  }

  virtual boost::any
  visit(AttributeTypeAS4Path& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getData( )
    //		n.getAS4( )

    BOOST_FOREACH (const AttributeTypeASPathSegmentPtr& segment, n.getPathSegments()) {
      segment->accept(*this, param);
    }
    return boost::any();
  }

  virtual boost::any
  visit(AttributeTypeAggregator& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getData( )
    //		n.getAS4( )

    //		n.getAggregatorLastAS( )
    //		n.getAggregatorLastASComplete( )
    //		n.getAggregatorBGPSpeakerIPAddress( )
    return boost::any();
  }
  virtual boost::any
  visit(AttributeTypeAtomicAggregate& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getData( )
    //		n.getAS4( )
    return boost::any();
  }
  virtual boost::any
  visit(AttributeTypeClusterList& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getData( )
    //		n.getAS4( )

    //		n.getClusterList( )
    return boost::any();
  }
  virtual boost::any
  visit(AttributeTypeCommunities& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getData( )
    //		n.getAS4( )

    //		n.getCommunityValues( )
    return boost::any();
  }
  virtual boost::any
  visit(AttributeTypeExtCommunities& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getData( )
    //		n.getAS4( )

    //		n.getExtCommunityValues( )
    return boost::any();
  }
  virtual boost::any
  visit(AttributeTypeLocalPref& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getData( )
    //		n.getAS4( )

    //		n.getLocalPrefValue( )
    return boost::any();
  }
  virtual boost::any
  visit(AttributeTypeMPReachNLRI& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getData( )
    //		n.getAS4( )

    //		n.getAFI( )
    //		n.getSAFI( )
    //		n.getNextHopAddressLength( )
    //		n.getNextHopAddress( )
    //		n.getNextHopAddressLocal( )

    BOOST_FOREACH (const NLRIReachablePtr& route, n.getNLRI()) {
      route->accept(*this, param);
    }

    BOOST_FOREACH (const NLRIReachablePtr& route, n.getSNPA()) {
      route->accept(*this, param);
    }
    return boost::any();
  }
  virtual boost::any
  visit(AttributeTypeMPUnreachNLRI& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getData( )
    //		n.getAS4( )

    //		n.getAFI( )
    //		n.getSAFI( )

    BOOST_FOREACH (const NLRIUnReachablePtr& route, n.getNLRI()) {
      route->accept(*this, param);
    }
    return boost::any();
  }
  virtual boost::any
  visit(AttributeTypeMultiExitDisc& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getData( )
    //		n.getAS4( )

    //		n.getMultiExitDiscValue( )
    return boost::any();
  }
  virtual boost::any
  visit(AttributeTypeNextHop& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getData( )
    //		n.getAS4( )

    //		n.getNextHopIPAddress( )
    //		n.getNextHopAFI( )
    return boost::any();
  }
  virtual boost::any
  visit(AttributeTypeOrigin& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getData( )
    //		n.getAS4( )

    //		n.getOrigin( )
    return boost::any();
  }
  virtual boost::any
  visit(AttributeTypeOriginatorID& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getData( )
    //		n.getAS4( )

    //		n.getOrigin( )
    return boost::any();
  }

  virtual boost::any
  visit(OptionalParameter& n, boost::any param)
  {
    //		n.getType( )
    //		n.getLength( )
    return boost::any();
  }
  virtual boost::any
  visit(OptionalParameterCapabilities& n, boost::any param)
  {
    //		n.getType( )
    //		n.getLength( )

    //		n.getCapCode( )
    //		n.getCapLength( )
    //		n.getCapValue( )
    return boost::any();
  }

  virtual boost::any
  visit(Route& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getPrefix( )
    //		n.getNumOctets( )
    //		+ helper methods available, see Route
    return boost::any();
  }

  virtual boost::any
  visit(NLRIReachable& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getPrefix( )
    //		n.getNumOctets( )
    //		+ helper methods available, see Route
    return boost::any();
  }
  virtual boost::any
  visit(NLRIUnReachable& n, boost::any param)
  {
    //		n.getLength( )
    //		n.getPrefix( )
    //		n.getNumOctets( )
    //		+ helper methods available, see Route
    return boost::any();
  }
};

#endif
