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

#include <libxml/tree.h>
#include "BGPDumper.h"
#include "BGPMessageDumper.h"
#include "MRTBgp4MPMessage.h"
#include <list>

extern "C" {
    #include "xmlinternal.h"
}

BGPMessageDumper::BGPMessageDumper()
{
    is_tabledump = false;
}

BGPMessageDumper::~BGPMessageDumper()
{}

xmlNodePtr BGPMessageDumper::genXml()
{
    xmlNodePtr bgpmsg_node = xmlNewNode(NULL, BAD_CAST "BGP_MESSAGE");
    xmlNewPropString(bgpmsg_node, "version", _XFB_VERSION);


    /* | TIME node | --------------------------------------------------------------------- */
    xmlNodePtr time_node = xmlNewNode(NULL, BAD_CAST "TIME");
    xmlAddChild(bgpmsg_node, time_node);
    /* TIMESTAMP      */      xmlNewChildUnsignedInt(time_node,  "TIMESTAMP",      timestamp);
    /* DATETIME       */      xmlNewChildGmtTime(time_node,      "DATETIME",       timestamp);
    ///* PRECISION_TIME */    xmlNewChildUnsignedInt(time_node,  "PRECISION_TIME", bmf->precisiontime);
    
    /* | Peering node | ------------------------------------------------------------------ */
    xmlNodePtr peering_node = xmlNewNode(NULL, BAD_CAST "PEERING");
    xmlAddChild(bgpmsg_node, peering_node);

    xmlNodePtr src_addr_node, dst_addr_node;
    static char src_addr[INET6_ADDRSTRLEN]; src_addr[0] = '\0';
    static char dst_addr[INET6_ADDRSTRLEN]; dst_addr[0] = '\0';
	switch(afi)
	{
		case AFI_IPv4: 
			inet_ntop(AF_INET,  &(peer_addr.ipv4),  dst_addr, INET_ADDRSTRLEN);
			inet_ntop(AF_INET,  &(local_addr.ipv4), src_addr, INET_ADDRSTRLEN);
			break;
		case AFI_IPv6: 
			inet_ntop(AF_INET6, &(peer_addr.ipv6),  dst_addr, INET6_ADDRSTRLEN);
			inet_ntop(AF_INET6, &(local_addr.ipv6), src_addr, INET6_ADDRSTRLEN);
			break;
	}

    /* SRC_ADDR */ src_addr_node = xmlNewChildString(peering_node, "SRC_ADDR", src_addr);
    /* SRC_PORT */ xmlNewChildString(peering_node,    "SRC_PORT", "");
    /* SRC_AS   */ xmlNewChildInt(peering_node,       "SRC_AS",   local_as);
    /* DST_ADDR */ dst_addr_node = xmlNewChildString(peering_node, "DST_ADDR", dst_addr);
    /* DST_PORT */ xmlNewChildString(peering_node,    "DST_PORT", "");
    /* DST_AS   */ xmlNewChildInt(peering_node,       "DST_AS",   peer_as);
    /* BGPID    */ //xmlNewChildBGPID(peering_node,  "BGPID",    sp->configInUse.remoteBGPID);
    xmlNewPropAFI(src_addr_node, afi);
    xmlNewPropAFI(dst_addr_node, afi);
    //xmlNewPropInt(src_addr_node, "if_index", 0);
    //xmlNewPropInt(dst_addr_node, "if_index", 0);

    /* | Ascii / Octets node | ----------------------------------------------------------- */
    BGPDumper *bgp_dumper = BGPDumper::newDumper(bgp_msg);
    xmlAddChild(bgpmsg_node, bgp_dumper->genXmlAsciiMsg());
    //xmlAddChild(bgpmsg_node, bgp_dumper->genXmlOctetMsg());
    delete bgp_dumper;

    //[TODO]
    //printNode(bgpmsg_node);
    return bgpmsg_node;
}

string BGPMessageDumper::genAscii()
{
    string bgpmsg_node = "";
    
    /* | Peering node | ------------------------------------------------------------------ */
    static char src_addr[INET6_ADDRSTRLEN]; src_addr[0] = '\0';
    static char dst_addr[INET6_ADDRSTRLEN]; dst_addr[0] = '\0';
	switch(afi)
	{
		case AFI_IPv4: 
			inet_ntop(AF_INET,  &(peer_addr.ipv4),  dst_addr, INET_ADDRSTRLEN);
			inet_ntop(AF_INET,  &(local_addr.ipv4), src_addr, INET_ADDRSTRLEN);
			break;
		case AFI_IPv6: 
			inet_ntop(AF_INET6, &(peer_addr.ipv6),  dst_addr, INET6_ADDRSTRLEN);
			inet_ntop(AF_INET6, &(local_addr.ipv6), src_addr, INET6_ADDRSTRLEN);
			break;
	}

    static char buffer[256]; 
    buffer[0] = '\0';
    sprintf(buffer, "%u", peer_as);
    string   peer_addr_str = dst_addr;
    string   peer_as_str   = buffer;

    BGPDumper *bgp_dumper = BGPDumper::newDumper(bgp_msg);
    list<string> bgp_str_list = bgp_dumper->genAsciiMsg(peer_addr_str, peer_as_str, is_tabledump);
    delete bgp_dumper;

    list<string>::iterator i;
    string msg_type = ( is_tabledump ) ? "TABLE_DUMP" : "BGP4MP";
    buffer[0] = '\0';
    for(i=bgp_str_list.begin(); i != bgp_str_list.end(); ++i)
    {
        buffer[0] = '\0';
        sprintf (buffer, "%s|%d|", 
                         msg_type.c_str(),
                         (int)timestamp
                );
        bgpmsg_node += buffer + *i;
        bgpmsg_node += "\n";
    }

    //printNode(bgpmsg_node);
    return bgpmsg_node;
}

// vim: sw=4 ts=4 sts=4 expandtab
