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

#include "BGPDumper.h"
#include "BGPMessageDumper.h"
#include "MRTBgp4MPMessage.h"

#include <libxml/tree.h>
#include <sstream>
#include <list>
#include <boost/foreach.hpp>

extern "C" {
    #include "xmlinternal.h"
}

BGPMessageDumper::BGPMessageDumper( const BGPMessagePtr &msg )
{
    is_tabledump = false;
    this->bgp_msg = msg;
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

    string src_addr, dst_addr;
	switch(afi)
	{
		case AFI_IPv4: 
			src_addr=FORMAT_IPv4_ADDRESS( local_addr.ipv4 );
			dst_addr=FORMAT_IPv4_ADDRESS( peer_addr.ipv4 );
			break;
		case AFI_IPv6: 
			src_addr=FORMAT_IPv6_ADDRESS( local_addr.ipv6 );
			dst_addr=FORMAT_IPv6_ADDRESS( peer_addr.ipv6 );
			break;
	}

    /* SRC_ADDR */ src_addr_node = xmlNewChildString(peering_node, "SRC_ADDR", src_addr.c_str());
    /* SRC_PORT */ xmlNewChildString(peering_node,    "SRC_PORT", "");
    /* SRC_AS   */ xmlNewChildInt(peering_node,       "SRC_AS",   local_as);
    /* DST_ADDR */ dst_addr_node = xmlNewChildString(peering_node, "DST_ADDR", dst_addr.c_str());
    /* DST_PORT */ xmlNewChildString(peering_node,    "DST_PORT", "");
    /* DST_AS   */ xmlNewChildInt(peering_node,       "DST_AS",   peer_as);
    /* BGPID    */ //xmlNewChildBGPID(peering_node,  "BGPID",    sp->configInUse.remoteBGPID);
    xmlNewPropAFI(src_addr_node, afi);
    xmlNewPropAFI(dst_addr_node, afi);
    //xmlNewPropInt(src_addr_node, "if_index", 0);
    //xmlNewPropInt(dst_addr_node, "if_index", 0);

    /* | Ascii / Octets node | ----------------------------------------------------------- */
    BGPDumperPtr bgp_dumper = BGPDumper::newDumper(bgp_msg);
    xmlAddChild(bgpmsg_node, bgp_dumper->genXmlAsciiMsg());
    //xmlAddChild(bgpmsg_node, bgp_dumper->genXmlOctetMsg());

    //[TODO]
    //printNode(bgpmsg_node);
    return bgpmsg_node;
}

string BGPMessageDumper::genAscii()
{
    ostringstream bgpmsg_node;
    
    /* | Peering node | ------------------------------------------------------------------ */
    string src_addr, dst_addr;
	switch(afi)
	{
		case AFI_IPv4: 
			src_addr=FORMAT_IPv4_ADDRESS( local_addr.ipv4 );
			dst_addr=FORMAT_IPv4_ADDRESS( peer_addr.ipv4 );
			break;
		case AFI_IPv6: 
			src_addr=FORMAT_IPv6_ADDRESS( local_addr.ipv6 );
			dst_addr=FORMAT_IPv6_ADDRESS( peer_addr.ipv6 );
			break;
	}

    ostringstream peer_buffer;
    peer_buffer << (int)peer_as;

    string   peer_addr_str = dst_addr;
    string   peer_as_str   = peer_buffer.str();

    BGPDumperPtr bgp_dumper = BGPDumper::newDumper(bgp_msg);
    list<string> bgp_str_list = bgp_dumper->genAsciiMsg(peer_addr_str, peer_as_str, is_tabledump);

    string msg_type = ( is_tabledump ) ? "TABLE_DUMP" : "BGP4MP";
    BOOST_FOREACH( const string &msg, bgp_str_list )
    {
        ostringstream buffer;
        buffer << msg_type << "|" << (int)timestamp << "|";

        bgpmsg_node << buffer.str() << msg
        			<< endl;
    }

    //printNode(bgpmsg_node);
    return bgpmsg_node.str();
}

// vim: sw=4 ts=4 sts=4 expandtab
