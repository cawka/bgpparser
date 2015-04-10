/*
 * Copyright (c) 2008,2009, University of California, Los Angeles All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of NLnetLabs nor the names of its
 *     contributors may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

// Author: Jason Ryder, Paul Wang
// Modified: Jonathan Park (jpark@cs.ucla.edu)
#include <bgpparser.h>

#include "AttributeType.h"

#include "AttributeTypeAggregator.h"
#include "AttributeTypeAS4Aggregator.h"
#include "AttributeTypeASPath.h"
#include "AttributeTypeAS4Path.h"
#include "AttributeTypeAtomicAggregate.h"
#include "AttributeTypeCommunities.h"
#include "AttributeTypeOriginatorID.h"
#include "AttributeTypeClusterList.h"
#include "AttributeTypeExtCommunities.h"
#include "AttributeTypeLocalPref.h"
#include "AttributeTypeMultiExitDisc.h"
#include "AttributeTypeNextHop.h"
#include "AttributeTypeOrigin.h"
#include "AttributeTypeMPReachNLRI.h"
#include "AttributeTypeMPUnreachNLRI.h"

namespace io = boost::iostreams;

using namespace std;

log4cxx::LoggerPtr AttributeType::Logger = log4cxx::Logger::getLogger("bgpparser.AttributeType");

// uint8_t *AttributeType::endMsg = NULL;

AttributeType::AttributeType(uint16_t len, istream& input, bool isAS4)
{
  LOG4CXX_TRACE(Logger, "length = " << (int)len);

  length = len;
  this->isAS4 = isAS4;

  if (length == 0)
    return;

  data = std::shared_ptr<char>(new char[length]);

  int read = io::read(input, data.get(), length);

  if (read == -1 || read != length) {
    LOG4CXX_ERROR(Logger, length << " bytes was requested, read only " << read << " bytes");
    throw BGPError();
  }
}

AttributeType::~AttributeType(void)
{
}

AttributeTypePtr
AttributeType::newAttribute(uint8_t attrType, uint16_t len, istream& input, bool isAS4)
{
  AttributeTypePtr attr;
  AttributeType header(len, input, isAS4);

  io::stream<io::array_source> in(header.data.get(), header.length);

  switch (attrType) {
  case ORIGIN: {
    attr = AttributeTypePtr(new AttributeTypeOrigin(header, in));
    break;
  }
  case AS_PATH: {
    attr = AttributeTypePtr(new AttributeTypeASPath(header, in));
    break;
  }
  case NEXT_HOP: {
    attr = AttributeTypePtr(new AttributeTypeNextHop(header, in));
    break;
  }
  case MULTI_EXIT_DISC: {
    attr = AttributeTypePtr(new AttributeTypeMultiExitDisc(header, in));
    break;
  }
  case LOCAL_PREF: {
    attr = AttributeTypePtr(new AttributeTypeLocalPref(header, in));
    break;
  }
  case ATOMIC_AGGREGATE: {
    attr = AttributeTypePtr(new AttributeTypeAtomicAggregate(header, in));
    break;
  }
  case AGGREGATOR: {
    attr = AttributeTypePtr(new AttributeTypeAggregator(header, in));
    break;
  }
  case COMMUNITIES: {
    attr = AttributeTypePtr(new AttributeTypeCommunities(header, in));
    break;
  }
  case ORIGINATOR_ID: {
    attr = AttributeTypePtr(new AttributeTypeOriginatorID(header, in));
    break;
  }
  case CLUSTER_LIST: {
    attr = AttributeTypePtr(new AttributeTypeClusterList(header, in));
    break;
  }
  case EXT_COMMUNITIES: {
    attr = AttributeTypePtr(new AttributeTypeExtCommunities(header, in));
    break;
  }
  case MP_REACH_NLRI: {
    attr = AttributeTypePtr(new AttributeTypeMPReachNLRI(header, in));
    break;
  }
  case MP_UNREACH_NLRI: {
    attr = AttributeTypePtr(new AttributeTypeMPUnreachNLRI(header, in));
    break;
  }
  case NEW_AS_PATH: {
    attr = AttributeTypePtr(new AttributeTypeAS4Path(header, in));
    break;
  }
  case NEW_AGGREGATOR: {
    attr = AttributeTypePtr(new AttributeTypeAS4Aggregator(header, in));
    break;
  }
  // ADD MORE ATTRIBUTES HERE

  default: {
    //			LOG4CXX_ERROR(Logger,"Unknown attribute type code ["<<(int)attrType<<"]");
    attr = AttributeTypePtr(new AttributeType(header));
    break;
  }
  }
  return attr;
}

std::string
AttributeType::getTypeStr(uint8_t attrType)
{
  switch (attrType) {
  case ORIGIN: {
    return "ORIGIN";
    break;
  }
  case AS_PATH: {
    return "AS_PATH";
    break;
  }
  case NEXT_HOP: {
    return "NEXT_HOP";
    break;
  }
  case MULTI_EXIT_DISC: {
    return "MULTI_EXIT_DISC";
    break;
  }
  case LOCAL_PREF: {
    return "LOCAL_PREF";
    break;
  }
  case ATOMIC_AGGREGATE: {
    return "ATOMIC_AGGREGATE";
    break;
  }
  case AGGREGATOR: {
    return "AGGREGATOR";
    break;
  }
  case COMMUNITIES: {
    return "COMMUNITIES";
    break;
  }
  case ORIGINATOR_ID: {
    return "ORIGINATOR_ID";
    break;
  }
  case CLUSTER_LIST: {
    return "CLUSTER_LIST";
    break;
  }
  case EXT_COMMUNITIES: {
    return "EXT_COMMUNITIES";
    break;
  }
  case MP_REACH_NLRI: {
    return "MP_REACH_NLRI";
    break;
  }
  case MP_UNREACH_NLRI: {
    return "MP_UNREACH_NLRI";
    break;
  }
  case NEW_AS_PATH: {
    return "AS4_PATH";
    break;
  }
  case NEW_AGGREGATOR: {
    return "AS4_AGGREGATOR";
    break;
  }
  // ADD MORE ATTRIBUTES HERE

  default: {
    return "OTHER";
  }
  }
  return "";
}
