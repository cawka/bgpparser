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

#include <sstream>
#include <libxml/tree.h>
#include "BGPStateChangeDumper.h"
#include "MRTBgp4MPStateChange.h"

extern "C" {
#include "xmlinternal.h"
}

BGPStateChangeDumper::BGPStateChangeDumper()
{
}

BGPStateChangeDumper::~BGPStateChangeDumper()
{
}

xmlNodePtr
BGPStateChangeDumper::genXml()
{
  xmlNodePtr bgpmsg_node = xmlNewNode(NULL, BAD_CAST "BGP_MESSAGE");
  xmlNewPropString(bgpmsg_node, "version", _XFB_VERSION);

  /* | TIME node | --------------------------------------------------------------------- */
  xmlNodePtr time_node = xmlNewNode(NULL, BAD_CAST "TIME");
  xmlAddChild(bgpmsg_node, time_node);
  /* TIMESTAMP      */ xmlNewChildUnsignedInt(time_node, "TIMESTAMP", timestamp);
  /* DATETIME       */ xmlNewChildGmtTime(time_node, "DATETIME", timestamp);
  ///* PRECISION_TIME */    xmlNewChildUnsignedInt(time_node,  "PRECISION_TIME",
  ///bmf->precisiontime);

  /* | Peering node | ------------------------------------------------------------------ */
  xmlNodePtr peering_node = xmlNewNode(NULL, BAD_CAST "PEERING");
  xmlAddChild(bgpmsg_node, peering_node);

  xmlNodePtr src_addr_node, dst_addr_node;
  static char src_addr[INET6_ADDRSTRLEN];
  src_addr[0] = '\0';
  static char dst_addr[INET6_ADDRSTRLEN];
  dst_addr[0] = '\0';
  switch (afi) {
  case AFI_IPv4:
    inet_ntop(AF_INET, &(peer_addr.ipv4), dst_addr, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(local_addr.ipv4), src_addr, INET_ADDRSTRLEN);
    break;
  case AFI_IPv6:
    inet_ntop(AF_INET6, &(peer_addr.ipv6), dst_addr, INET6_ADDRSTRLEN);
    inet_ntop(AF_INET6, &(local_addr.ipv6), src_addr, INET6_ADDRSTRLEN);
    break;
  }

  /* SRC_ADDR */ src_addr_node = xmlNewChildString(peering_node, "SRC_ADDR", src_addr);
  /* SRC_PORT */ xmlNewChildString(peering_node, "SRC_PORT", "");
  /* SRC_AS   */ xmlNewChildInt(peering_node, "SRC_AS", local_as);
  /* DST_ADDR */ dst_addr_node = xmlNewChildString(peering_node, "DST_ADDR", dst_addr);
  /* DST_PORT */ xmlNewChildString(peering_node, "DST_PORT", "");
  /* DST_AS   */ xmlNewChildInt(peering_node, "DST_AS", peer_as);
  /* BGPID    */ // xmlNewChildBGPID(peering_node,  "BGPID",    sp->configInUse.remoteBGPID);
  xmlNewPropAFI(src_addr_node, afi);
  xmlNewPropAFI(dst_addr_node, afi);
  // xmlNewPropInt(src_addr_node, "if_index", 0);
  // xmlNewPropInt(dst_addr_node, "if_index", 0);

  /* | STATUS node         | ----------------------------------------------------------- */
  xmlNodePtr status_node = xmlNewNode(NULL, BAD_CAST "STATUS_MSG");
  xmlNodePtr session_status_node =
    xmlAddChild(status_node, xmlNewNode(NULL, BAD_CAST "SESSION_STATUS"));
  xmlNodePtr session_node = xmlAddChild(session_status_node, xmlNewNode(NULL, BAD_CAST "SESSION"));
  xmlNewChildString(session_node, "ADDR", dst_addr);
  xmlNewChildString(session_node, "PORT", "");
  // xmlNewChildInt(session_node,    "AS",   peer_as);
  xmlNodePtr state_change_node =
    xmlAddChild(session_node, xmlNewNode(NULL, BAD_CAST "STATE_CHANGE"));
  xmlNewChildInt(state_change_node, "OLD_STATE", oldState);
  xmlNewChildInt(state_change_node, "NEW_STATE", newState);
  xmlNewChildString(state_change_node, "REASON", "");
  xmlAddChild(bgpmsg_node, status_node);

  return bgpmsg_node;
}

string
BGPStateChangeDumper::genAscii()
{
  string bgpmsg_node = "";

  /* | Peering node | ------------------------------------------------------------------ */
  static char src_addr[INET6_ADDRSTRLEN];
  src_addr[0] = '\0';
  static char dst_addr[INET6_ADDRSTRLEN];
  dst_addr[0] = '\0';
  switch (afi) {
  case AFI_IPv4:
    inet_ntop(AF_INET, &(peer_addr.ipv4), dst_addr, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(local_addr.ipv4), src_addr, INET_ADDRSTRLEN);
    break;
  case AFI_IPv6:
    inet_ntop(AF_INET6, &(peer_addr.ipv6), dst_addr, INET6_ADDRSTRLEN);
    inet_ntop(AF_INET6, &(local_addr.ipv6), src_addr, INET6_ADDRSTRLEN);
    break;
  }
  string peer_addr_str = dst_addr;
  static char buffer[256];
  buffer[0] = '\0';
  sprintf(buffer, "%u", peer_as);
  string peer_as_str = buffer;
  buffer[0] = '\0';
  sprintf(buffer, "%d", (int)timestamp);
  string ts_str = buffer;
  buffer[0] = '\0';
  sprintf(buffer, "%d", oldState);
  string old_str = buffer;
  buffer[0] = '\0';
  sprintf(buffer, "%d", newState);
  string new_str = buffer;

  bgpmsg_node = bgpmsg_node + "BGP4MP" + "|" + ts_str + "|" + "STATE" + "|" + peer_addr_str + "|"
                + peer_as_str + "|" + old_str + "|" + new_str + "\n";

  return bgpmsg_node;
}

// vim: sw=4 ts=4 sts=4 expandtab
