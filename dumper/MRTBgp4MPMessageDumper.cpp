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

#include <libxml/tree.h>
#include "MRTBgp4MPMessage.h"
#include "MRTBgp4MPMessageDumper.h"
#include "BGPMessageDumper.h"

MRTBgp4MPMessageDumper::MRTBgp4MPMessageDumper()
{
}

MRTBgp4MPMessageDumper::~MRTBgp4MPMessageDumper()
{
}

MRTBgp4MPMessageDumperPtr
MRTBgp4MPMessageDumper::newDumper(const MRTBgp4MPMessagePtr& bgp4mp_msg)
{
  MRTBgp4MPMessageDumperPtr dumper = MRTBgp4MPMessageDumperPtr(new MRTBgp4MPMessageDumper());
  dumper->bgp4mp_msg = bgp4mp_msg;

  return dumper;
}

xmlNodePtr
MRTBgp4MPMessageDumper::genXml()
{
  xmlNodePtr node = NULL;

  BGPMessageDumperPtr bgpmsg_dumper(new BGPMessageDumper(bgp4mp_msg->getPayload()));
  /* Collect infomation */
  bgpmsg_dumper->setTimestamp(bgp4mp_msg->getTimestamp());

  bgpmsg_dumper->setPeering(bgp4mp_msg->getPeerIP(), bgp4mp_msg->getLocalIP(),
                            bgp4mp_msg->getPeerAS(), bgp4mp_msg->getLocalAS(),
                            bgp4mp_msg->getInterfaceIndex(), bgp4mp_msg->getAddressFamily());
  node = bgpmsg_dumper->genXml();

  return node;
}

string
MRTBgp4MPMessageDumper::genAscii()
{
  string node = "";

  BGPMessageDumperPtr bgpmsg_dumper(new BGPMessageDumper(bgp4mp_msg->getPayload()));
  /* Collect infomation */
  bgpmsg_dumper->setTimestamp(bgp4mp_msg->getTimestamp());
  bgpmsg_dumper->setPeering(bgp4mp_msg->getPeerIP(), bgp4mp_msg->getLocalIP(),
                            bgp4mp_msg->getPeerAS(), bgp4mp_msg->getLocalAS(),
                            bgp4mp_msg->getInterfaceIndex(), bgp4mp_msg->getAddressFamily());
  node = bgpmsg_dumper->genAscii();

  return node;
}

// vim: sw=4 ts=4 sts=4 expandtab
