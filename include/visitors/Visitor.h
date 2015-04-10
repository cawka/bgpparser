
#ifndef _VISITOR_H_
#define _VISITOR_H_

#include "ForwardDeclarations.h"

class Visitor {
public:
  virtual void
  visit(MRTCommonHeader&) = 0;

  virtual void
  visit(MRTBgp4MPMessage&) = 0;
  virtual void
  visit(MRTBgp4MPMessageAS4&) = 0;
  virtual void
  visit(MRTBgp4MPStateChange&) = 0;
  virtual void
  visit(MRTBgp4MPStateChangeAS4&) = 0;

  virtual void
  visit(AttributeType&) = 0;
  virtual void
  visit(AttributeTypeAS4Aggregator&) = 0;
  virtual void
  visit(AttributeTypeAS4Path&) = 0;
  virtual void
  visit(AttributeTypeAS4PathSegment&) = 0;
  virtual void
  visit(AttributeTypeASPath&) = 0;
  virtual void
  visit(AttributeTypeASPathSegment&) = 0;
  virtual void
  visit(AttributeTypeAggregator&) = 0;
  virtual void
  visit(AttributeTypeAtomicAggregate&) = 0;
  virtual void
  visit(AttributeTypeClusterList&) = 0;
  virtual void
  visit(AttributeTypeCommunities&) = 0;
  virtual void
  visit(AttributeTypeExtCommunities&) = 0;
  virtual void
  visit(AttributeTypeLocalPref&) = 0;
  virtual void
  visit(AttributeTypeMPReachNLRI&) = 0;
  virtual void
  visit(AttributeTypeMPUnreachNLRI&) = 0;
  virtual void
  visit(AttributeTypeMultiExitDisc&) = 0;
  virtual void
  visit(AttributeTypeNextHop&) = 0;
  virtual void
  visit(AttributeTypeOrigin&) = 0;
  virtual void
  visit(AttributeTypeOriginatorID&) = 0;

  virtual void
  visit(BGPCommonHeader&) = 0;
  virtual void
  visit(BGPKeepAlive&) = 0;
  virtual void
  visit(BGPNotification&) = 0;
  virtual void
  visit(BGPOpen&) = 0;
  virtual void
  visit(BGPRouteRefresh&) = 0;
  virtual void
  visit(BGPUpdate&) = 0;
  virtual void
  visit(BGPAttribute&) = 0;

  virtual void
  visit(MRTTblDump&) = 0;
  virtual void
  visit(MRTTblDumpV2PeerIndexTbl&) = 0;
  virtual void
  visit(MRTTblDumpV2RibGeneric&) = 0;
  virtual void
  visit(MRTTblDumpV2RibHeader&) = 0;
  virtual void
  visit(MRTTblDumpV2RibIPv4Multicast&) = 0;
  virtual void
  visit(MRTTblDumpV2RibIPv4Unicast&) = 0;
  virtual void
  visit(MRTTblDumpV2RibIPv6Multicast&) = 0;
  virtual void
  visit(MRTTblDumpV2RibIPv6Unicast&) = 0;
  virtual void
  visit(OptionalParameter&) = 0;
  virtual void
  visit(OptionalParameterCapabilities&) = 0;
  virtual void
  visit(TblDumpV2RibEntry&) = 0;

  virtual void
  visit(Route&) = 0;
  virtual void
  visit(NLRIReachable&) = 0;
  virtual void
  visit(NLRIUnReachable&) = 0;
};

#endif
