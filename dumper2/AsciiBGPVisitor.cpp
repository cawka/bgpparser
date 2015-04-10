
#include <bgpparser.h>
#include "AsciiBGPVisitor.h"

#include "Params.h"

void
AsciiBGPVisitor::visit(MRTTblDump& n, boost::any param)
{
  InfoPtr info = boost::any_cast<InfoPtr>(param);
  info->afi = n.getSubType();

  info->parseNLRI = false;

  BOOST_FOREACH (const BGPAttributePtr& attr, n.getAttributes()) {
    attr->accept(*this, param);
  }

  if (!info->parseNLRI)
    NLRIReachable(n.getPrefixLength(), n.getPrefix()).accept(*this, param);
}

void
AsciiBGPVisitor::visit(MRTTblDumpV2RibHeader& n, boost::any param)
{
  InfoPtr info = boost::any_cast<InfoPtr>(param);
  info->afi = n.getAFI();

  BOOST_FOREACH (const TblDumpV2RibEntryPtr& entry, n.getRibEntries()) {
    const MRTTblDumpV2PeerIndexTblPeerEntryPtr peer = n.getPeer(*entry);

    info->peer_addr = peer->peerIP;
    info->peer_as = peer->peerAS;
    info->peer_afi = peer->IPType;

    InfoPtr prefixInfo = InfoPtr(new Info(*info));
    entry->accept(*this, prefixInfo);
    if (!prefixInfo->parseNLRI)
      NLRIReachable(n.getPrefixLength(), n.getPrefix()).accept(*this, prefixInfo);
  }
}

void
AsciiBGPVisitor::visit(BGPOpen& n, boost::any param)
{
  // prevent visiting optional headers
}

void
AsciiBGPVisitor::visit(BGPUpdate& n, boost::any param)
{
  BOOST_FOREACH (const BGPAttributePtr& attr, n.getPathAttributes()) {
    attr->accept(*this, param);
  }

  //	////////////////////////////////////////////////////////////////////

  boost::any_cast<InfoPtr>(param)->afi = AFI_IPv4;

  BOOST_FOREACH (const RoutePtr& route, n.getWithdrawnRoutes()) {
    route->accept(*this, param);
  }

  BOOST_FOREACH (const RoutePtr& route, n.getNlriRoutes()) {
    route->accept(*this, param);
  }
}

void
AsciiBGPVisitor::visit(AttributeTypeMPReachNLRI& n, boost::any param)
{
  InfoPtr info = boost::any_cast<InfoPtr>(param);

  if (n.getAFI() == 0) {
    info->next_hop = FORMAT_IP_ADDRESS(n.getNextHopAddress(), info->afi);
    return;
  }

  info->afi = n.getAFI(); // should be set everywhere except TABLE_DUMP_V2
  info->next_hop = FORMAT_IP_ADDRESS(n.getNextHopAddress(), info->afi);

  BOOST_FOREACH (const NLRIReachablePtr& route, n.getNLRI()) {
    route->accept(*this, param);
  }

  BOOST_FOREACH (const NLRIReachablePtr& route, n.getSNPA()) {
    route->accept(*this, param);
  }
}

void
AsciiBGPVisitor::visit(AttributeTypeMPUnreachNLRI& n, boost::any param)
{
  if (n.getAFI() == 0)
    return;

  boost::any_cast<InfoPtr>(param)->afi = n.getAFI();

  BOOST_FOREACH (const NLRIUnReachablePtr& route, n.getNLRI()) {
    route->accept(*this, param);
  }
}

void
AsciiBGPVisitor::visit(NLRIReachable& n, boost::any param)
{
  InfoPtr info = boost::any_cast<InfoPtr>(param);

  std::cout << info->mrt_type << "|" << info->timestamp << "|" << info->dump_type << "|" // A or I
            << FORMAT_IP_ADDRESS(info->peer_addr, info->peer_afi) << "|" << info->peer_as << "|"
            << FORMAT_IP_ADDRESS(n.getPrefix(), info->afi) << "/" << (int)n.getLength() << "|"
            << info->as_path << "|" << info->origin << "|" << info->next_hop << "|" << info->lpref
            << "|" << info->med << "|" << info->communities << "|" << info->atomic_agg << "|"
            << info->agg << "|"
            << "\n";
  info->parseNLRI = true;
}

void
AsciiBGPVisitor::visit(NLRIUnReachable& n, boost::any param)
{
  InfoPtr info = boost::any_cast<InfoPtr>(param);

  std::cout << info->mrt_type << "|" << info->timestamp << "|"
            << "W"
            << "|" << FORMAT_IP_ADDRESS(info->peer_addr, info->peer_afi) << "|" << info->peer_as
            << "|" << FORMAT_IP_ADDRESS(n.getPrefix(), info->afi) << "/" << (int)n.getLength()
            << "\n";
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////// ORIGIN //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void
AsciiBGPVisitor::visit(AttributeTypeOrigin& n, boost::any param)
{
  InfoPtr info = boost::any_cast<InfoPtr>(param);

  switch (n.getOrigin()) {
  case AttributeTypeOrigin::IGP:
    info->origin = "IGP";
    break;
  case AttributeTypeOrigin::EGP:
    info->origin = "EGP";
    break;
  case AttributeTypeOrigin::INCOMPLETE:
  default:
    info->origin = "INCOMPLETE";
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////// AS PATH /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void
AsciiBGPVisitor::visit(AttributeTypeASPath& n, boost::any param)
{
  std::ostringstream os;

  bool isFirst = true;
  BOOST_FOREACH (const AttributeTypeASPathSegmentPtr& segment,
                 /**/ n.getPathSegmentsComplete() /**/) {
    if (isFirst)
      isFirst = false;
    else
      os << " ";

    segment->accept(_asSegmentVisitor, static_cast<std::ostream*>(&os));
  }

  boost::any_cast<InfoPtr>(param)->as_path = os.str();
}

void
AsciiBGPVisitor::visit(AttributeTypeAS4Path& n, boost::any param)
{
  std::ostringstream os;

  bool isFirst = true;
  BOOST_FOREACH (const AttributeTypeASPathSegmentPtr& segment, /**/ n.getPathSegments() /**/) {
    if (isFirst)
      isFirst = false;
    else
      os << " ";

    segment->accept(_asSegmentVisitor, static_cast<std::ostream*>(&os));
  }

  boost::any_cast<InfoPtr>(param)->as_path = os.str();
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////// NEXT HOP ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void
AsciiBGPVisitor::visit(AttributeTypeNextHop& n, boost::any param)
{
  boost::any_cast<InfoPtr>(param)->next_hop =
    FORMAT_IP_ADDRESS(n.getNextHopIPAddress(), n.getNextHopAFI());
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// MultiExitDisc /////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void
AsciiBGPVisitor::visit(AttributeTypeMultiExitDisc& n, boost::any param)
{
  boost::any_cast<InfoPtr>(param)->med = n.getMultiExitDiscValue();
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// LOCAL PREF ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void
AsciiBGPVisitor::visit(AttributeTypeLocalPref& n, boost::any param)
{
  boost::any_cast<InfoPtr>(param)->lpref = n.getLocalPrefValue();
}

//////////////////////////////////////////////////////////////////////////////
/////////////////////////// ATOMIC AGGREGATE ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void
AsciiBGPVisitor::visit(AttributeTypeAtomicAggregate& n, boost::any param)
{
  boost::any_cast<InfoPtr>(param)->atomic_agg = "AG";
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// AGGREGATOR ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void
AsciiBGPVisitor::visit(AttributeTypeAggregator& n, boost::any param)
{
  std::ostringstream os;
  os << (int)n.getAggregatorLastASComplete() << " " << n.getAggregatorBGPSpeakerIPAddress().ipv4;
  boost::any_cast<InfoPtr>(param)->agg = os.str();
}

void
AsciiBGPVisitor::visit(AttributeTypeAS4Aggregator& n, boost::any param)
{
  std::ostringstream os;
  os << (int)n.getAggregatorLastAS() << " " << n.getAggregatorBGPSpeakerIPAddress().ipv4;
  boost::any_cast<InfoPtr>(param)->agg = os.str();
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// COMMUNITIES ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void
AsciiBGPVisitor::visit(AttributeTypeCommunities& n, boost::any param)
{
  std::ostringstream os;
  std::string sep = "";

  BOOST_FOREACH (const CommunityValue& value, n.getCommunityValues()) {
    if (value.ASnum == 0xFFFF && value.info == 0xFF01) {
      os << sep << "no-export";
    }
    else {
      os << sep << (int)value.ASnum << ":" << (int)value.info;
    }
    sep = " ";
  }

  boost::any_cast<InfoPtr>(param)->communities = os.str();
}
