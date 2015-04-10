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
using namespace boost;

#include "MRTTblDumpV1Dumper.h"
#include "BGPMessageDumper.h"
#include "BGPCommonHeader.h"
#include "BGPStructure.h"
#include "AttributeType.h"

#include "FakeBGPUpdate.h"
#include "FakeBGPAttribute.h"
#include "FakeAttributeTypeMPReachNLRI.h"
//#include "FakeAttributeTypeMPUnreachNLRI.h"

#include <libxml/tree.h>
#include <list>
#include <boost/foreach.hpp>

extern "C" {
#include "xmlinternal.h"
}

log4cxx::LoggerPtr MRTTblDumpV1Dumper::Logger =
  log4cxx::Logger::getLogger("bgpparser.MRTTblDumpV1Dumper");

MRTTblDumpV1Dumper::MRTTblDumpV1Dumper()
{
}

MRTTblDumpV1Dumper::~MRTTblDumpV1Dumper()
{
}

xmlNodePtr
MRTTblDumpV1Dumper::genXml()
{
  xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "BGP_MESSAGES");

  BGPMessageDumperPtr bgpmsg_dumper = genMsgDumper();

  // Gen xml
  xmlAddChild(node, bgpmsg_dumper->genXml());

  return node;
}

string
MRTTblDumpV1Dumper::genAscii()
{
  string node = "";

  BGPMessageDumperPtr bgpmsg_dumper = genMsgDumper();

  // Gen ascii
  node = bgpmsg_dumper->genAscii();

  return node;
}

BGPMessageDumperPtr
MRTTblDumpV1Dumper::genMsgDumper()
{
  // Prefix / Route
  NLRIReachablePtr route(new NLRIReachable(tblDumpMsg->getPrefixLength(), tblDumpMsg->getPrefix()));
  //	NLRIUnReachablePtr unroute( new NLRIUnReachable(tblDumpMsg->getPrefixLength(),
  //tblDumpMsg->getPrefix()) );

  shared_ptr<FakeBGPUpdate> update(new FakeBGPUpdate());

  /* Fill in the update members */
  std::list<BGPAttributePtr>::const_iterator mp_attr =
    find_if(tblDumpMsg->getAttributes().begin(), tblDumpMsg->getAttributes().end(),
            findByType(AttributeType::MP_REACH_NLRI));

  //	std::list<BGPAttributePtr>::const_iterator nh_attr=
  //		find_if( tblDumpMsg->getAttributes().begin(),
  //				 tblDumpMsg->getAttributes().end(),
  //				 findByType(AttributeType::NEXT_HOP) );

  BOOST_FOREACH (const BGPAttributePtr& attr, tblDumpMsg->getAttributes()) {
    update->addPathAttribute(attr);
  }

  /* Add NLRI */
  switch (tblDumpMsg->getSubType()) {
  case AFI_IPv4: {
    /* Add IPV4/UNICAST NLRI */
    update->addAnnouncedRoute(static_pointer_cast<Route>(route));
    break;
  }

  case AFI_IPv6: {
    /* Add IPV6 NLRI */
    if (mp_attr == tblDumpMsg->getAttributes().end()) {
      LOG4CXX_INFO(Logger, "IPv6 is specified, but message contains no NLRI attributes");

      // Create fake MP_REACH_NLRI attribute
      AttributeTypeMPReachNLRIPtr mp_attr(new FakeAttributeTypeMPReachNLRI(AFI_IPv6, 1));
      mp_attr->addNLRI(route);

      // mp_attr->setNextHopAddressLength(0);
      // mp_attr->setNextHopAddress(&nh_attr->getNextHopIPAddress());

      // TODO
      BGPAttributePtr attr(new FakeBGPAttribute(0x80, 14, mp_attr));

      update->addPathAttribute(attr);
    }
    else {
      dynamic_pointer_cast<AttributeTypeMPReachNLRI>(*mp_attr)->addNLRI(route);
    }
    break;
  }
  }

  /* Message Dumper */
  BGPMessageDumperPtr bgpmsg_dumper(new BGPMessageDumper(update));

  /* Collect infomation */
  bgpmsg_dumper->setTimestamp(tblDumpMsg->getTimestamp());
  bgpmsg_dumper->setPeering(tblDumpMsg->getPeerIP(), // LocalIP
                            tblDumpMsg->getPeerIP(), // PeerIP
                            tblDumpMsg->getPeerAS(), // LocalAS
                            tblDumpMsg->getPeerAS(), // LocalAS
                            0,                       // Interface Index
                            tblDumpMsg->getSubType() // IPV4
                            );
  bgpmsg_dumper->isTableDump(true);

  return bgpmsg_dumper;
}

// vim: sw=4 ts=4 sts=4 expandtab
