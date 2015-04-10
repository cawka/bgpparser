
#include <bgpparser.h>

#include "IPTypeDiscoveryVisitor.h"

boost::any
IPTypeDiscoveryVisitor::visit(MRTCommonHeader& n)
{
  return TYPE_UNKNOWN;
}

boost::any
IPTypeDiscoveryVisitor::visit(MRTBgp4MPMessage& n)
{
  switch (n.getAddressFamily()) {
  case AFI_IPv6:
    return TYPE_IPv6;
    break;
  case AFI_IPv4:
    return TYPE_IPv4;
    break;
  }
  return TYPE_UNKNOWN;
}

boost::any
IPTypeDiscoveryVisitor::visit(MRTBgp4MPStateChange& n)
{
  switch (n.getAddressFamily()) {
  case AFI_IPv6:
    return TYPE_IPv6;
    break;
  case AFI_IPv4:
    return TYPE_IPv4;
    break;
  }

  return TYPE_UNKNOWN;
}

boost::any
IPTypeDiscoveryVisitor::visit(MRTTblDump& n)
{
  switch (n.getSubType()) {
  case AFI_IPv6:
    return TYPE_IPv6;
    break;
  case AFI_IPv4:
    return TYPE_IPv4;
    break;
  }

  return TYPE_UNKNOWN;
}

boost::any
IPTypeDiscoveryVisitor::visit(MRTTblDumpV2PeerIndexTbl& n)
{
  return TYPE_UNKNOWN;
}

boost::any
IPTypeDiscoveryVisitor::visit(MRTTblDumpV2RibHeader& n)
{
  switch (n.getAFI()) {
  case AFI_IPv6:
    return TYPE_IPv6;
    break;
  case AFI_IPv4:
    return TYPE_IPv4;
    break;
  }

  return TYPE_UNKNOWN;
}
