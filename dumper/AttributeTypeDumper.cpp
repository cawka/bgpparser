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

#include <bgpparser.h>
using namespace std;

#include "AttributeTypeDumper.h"
#include "BGPAttributeDumper.h"
#include "BGPAttribute.h"

#include <sstream>
#include <libxml/tree.h>

extern "C" {
#include "xmlinternal.h"
}

log4cxx::LoggerPtr AttributeTypeDumper::Logger =
  log4cxx::Logger::getLogger("bgpdump.AttributeTypeDumper");

AttributeTypeDumper::AttributeTypeDumper(const AttributeTypePtr& attr)
{
  attr_type = attr;
}

AttributeTypeDumper::~AttributeTypeDumper()
{
}

AttributeTypeDumperPtr
AttributeTypeDumper::newDumper(uint8_t attrType, string attrTypeStr, const AttributeTypePtr& attr)
{
  AttributeTypeDumperPtr attr_dumper;

  switch (attrType) {
  case AttributeType::ORIGIN: {
    attr_dumper = AttributeTypeDumperPtr(new AttributeTypeOriginDumper(attr));
    break;
  }
  case AttributeType::AS_PATH: {
    attr_dumper = AttributeTypeDumperPtr(new AttributeTypeASPathDumper(attr));
    break;
  }
  case AttributeType::NEXT_HOP: {
    attr_dumper = AttributeTypeDumperPtr(new AttributeTypeNextHopDumper(attr));
    break;
  }
  case AttributeType::MULTI_EXIT_DISC: {
    attr_dumper = AttributeTypeDumperPtr(new AttributeTypeMultiExitDiscDumper(attr));
    break;
  }
  case AttributeType::LOCAL_PREF: {
    attr_dumper = AttributeTypeDumperPtr(new AttributeTypeLocalPrefDumper(attr));
    break;
  }
  case AttributeType::ATOMIC_AGGREGATE: {
    attr_dumper = AttributeTypeDumperPtr(new AttributeTypeAtomicAggregateDumper(attr));
    break;
  }
  case AttributeType::AGGREGATOR: {
    attr_dumper = AttributeTypeDumperPtr(new AttributeTypeAggregatorDumper(attr));
    break;
  }
  case AttributeType::COMMUNITIES: {
    attr_dumper = AttributeTypeDumperPtr(new AttributeTypeCommunitiesDumper(attr));
    break;
  }
  case AttributeType::ORIGINATOR_ID: {
    attr_dumper = AttributeTypeDumperPtr(new AttributeTypeOriginatorIDDumper(attr));
    break;
  }
  case AttributeType::CLUSTER_LIST: {
    attr_dumper = AttributeTypeDumperPtr(new AttributeTypeClusterListDumper(attr));
    break;
  }
  case AttributeType::MP_REACH_NLRI: {
    attr_dumper = AttributeTypeDumperPtr(new AttributeTypeMPReachNLRIDumper(attr));
    break;
  }
  case AttributeType::MP_UNREACH_NLRI: {
    attr_dumper = AttributeTypeDumperPtr(new AttributeTypeMPUnreachNLRIDumper(attr));
    break;
  }
  case AttributeType::NEW_AS_PATH: {
    attr_dumper = AttributeTypeDumperPtr(new AttributeTypeASPathDumper(attr));
    break;
  }
  case AttributeType::NEW_AGGREGATOR: {
    attr_dumper = AttributeTypeDumperPtr(new AttributeTypeAggregatorDumper(attr));
    break;
  }
  default: {
    LOG4CXX_INFO(Logger, "Unhandled attribute type code [" << (int)attrType << "]");
    attr_dumper = AttributeTypeDumperPtr(new AttributeTypeDumper(attr));
    break;
  }
  }
  attr_dumper->setTypeStr(attrTypeStr);
  attr_dumper->setTypeCode(attrType);

  return attr_dumper;
}

xmlNodePtr
AttributeTypeDumper::genXml()
{
  xmlNodePtr node = xmlNewNode(NULL, BAD_CAST(char*)type_str.c_str());
  xmlAddChild(node, xmlNewNodeOctets("OCTETS", (u_char*)attr_type->getData().get(),
                                     attr_type->getLength()));
  return node;
}

string
AttributeTypeDumper::genAscii()
{
  string node = "";
  return node;
}

// vim: sw=4 ts=4 sts=4 expandtab
