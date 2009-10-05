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

#include "BGPDumper.h"
#include "BGPOpen.h"
#include "BGPUpdate.h"
#include <libxml/tree.h>

extern "C" {
    #include "xmlinternal.h"
}

BGPDumper::BGPDumper(BGPMessage* msg)
{
    bgp_msg = msg;
}

BGPDumper::~BGPDumper()
{}

BGPDumper* BGPDumper::newDumper(BGPMessage* msg)
{
	BGPDumper* bgp_dumper = NULL;

	switch(msg->getType())
	{
        case BGPCommonHeader::OPEN:
            bgp_dumper = new BGPOpenDumper(msg);
            break;

        case BGPCommonHeader::UPDATE:
            PRINT_DBG("  case BGPCommonHeader::UPDATE");
            bgp_dumper = new BGPUpdateDumper(msg);
            break;
            
        case BGPCommonHeader::NOTIFICATION:
            bgp_dumper = new BGPNotificationDumper(msg);
            break;

        case BGPCommonHeader::KEEPALIVE:
            bgp_dumper = new BGPKeepAliveDumper(msg);
            break;

        case BGPCommonHeader::ROUTE_REFRESH:
            bgp_dumper = new BGPRouteRefreshDumper(msg);
            break;

        default:
            PRINT_DBG("  [BGPDumper] unsupported case");
            bgp_dumper = new BGPDumper(msg);
            break;
	}
	
	return bgp_dumper;
}

xmlNodePtr BGPDumper::genXml()
{
    return genXmlAsciiMsg();  // Default: XML ascii message
    //return genXmlOctetMsg();
}

xmlNodePtr BGPDumper::genXmlAsciiMsg()
{

    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "ASCII_MSG");

    xmlNodePtr static_marker_node = xmlNewNodeOctets("MARKER", bgp_msg->getMarker(), 16);
    string type = bgp_msg->TypeStr();

    //cout << "************************************************" << endl;
    //cout << type << endl;
    //cout << bgp_msg->TypeStr() << endl;
    //cout << bgp_msg->Type() << endl;
    //cout << "************************************************" << endl;
    xmlNodePtr marker_node, len_node, type_node, ascii_node;
    marker_node = xmlAddChild(node, static_marker_node);
    len_node    = xmlAddChild(node, xmlNewNodeInt("LENGTH" , bgp_msg->getLength()));
    type_node   = xmlAddChild(node, xmlNewNodeString("TYPE", (char *)type.c_str()));
    ascii_node  = xmlAddChild(node, genXmlAsciiNode());

    //printNode(node);
    return node;
}

xmlNodePtr BGPDumper::genXmlOctetMsg()
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "OCTET_MSG");

    xmlNodePtr static_marker_node = xmlNewNodeOctets("MARKER", bgp_msg->getMarker(), 16);
    string type = bgp_msg->TypeStr();
    
    xmlNodePtr marker_node, len_node, type_node, octets_node;
    marker_node = xmlAddChild(node, static_marker_node);
    len_node    = xmlAddChild(node, xmlNewNodeInt("LENGTH" , bgp_msg->getLength()));
    type_node   = xmlAddChild(node, xmlNewNodeString("TYPE", (char *)type.c_str()));
    octets_node = xmlAddChild(node, xmlNewNodeOctets("OCTETS", bgp_msg->getOctets(), 19 + bgp_msg->getLength()));

    //printNode(node);
    return node;
}

xmlNodePtr BGPDumper::genXmlAsciiNode()
{
    // Virtual function, should be overriwritten by derived classes.
    xmlNodePtr node, octets_node;
    string type = bgp_msg->TypeStr();
	int len = 16+2+1+bgp_msg->getLength();
    node        = xmlNewNode(NULL, BAD_CAST (char *)type.c_str());
    octets_node = xmlAddChild(node, xmlNewNodeOctets("OCTETS", bgp_msg->getOctets(), len));
    return node;
}

string BGPDumper::genAscii()
{
    // Virtual function, should be overriwritten by derived classes.
    string ascii_msg = "";
    return ascii_msg;
}

/*
list<string> BGPDumper::genAsciiMsg()
{
    // Virtual function, should be overriwritten by derived classes.
    list<string> bgp_msg_list;
    return bgp_msg_list;
}
*/

list<string> BGPDumper::genAsciiMsg(string peer_addr, string peer_as, bool is_tabledump=false)
{
    // Virtual function, should be overriwritten by derived classes.
    list<string> bgp_msg_list;
    return bgp_msg_list;
}


// vim: sw=4 ts=4 sts=4 expandtab
