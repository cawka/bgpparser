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

#include <string>
#include <libxml/tree.h>
#include "AttributeTypeDumper.h"
#include "AttributeTypeMPReachNLRI.h"

extern "C" {
    #include "xmlinternal.h"
}

AttributeTypeMPReachNLRIDumper::AttributeTypeMPReachNLRIDumper(AttributeType* attr)
: AttributeTypeDumper(attr)
{}

AttributeTypeMPReachNLRIDumper::~AttributeTypeMPReachNLRIDumper()
{}

xmlNodePtr AttributeTypeMPReachNLRIDumper::genPrefixNode(Route rt, int afi, int safi)
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "PREFIX");

    //[TODO] afi / safi

    /* generate prefix node */
    string tt = rt.toString(afi);
    node = xmlNewNodeString((char *)"PREFIX", (char *)tt.c_str());

    return node;
}

xmlNodePtr AttributeTypeMPReachNLRIDumper::genXml()
{
    AttributeTypeMPReachNLRI *attr = (AttributeTypeMPReachNLRI *)attr_type;
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST (char *)type_str.c_str()); /* AS_PATH or AS4_PATH */

    /* afi / safi */
    xmlAddChild(node, xmlNewNodeAFI((char *)"AFI",   attr->getAFI()));
    xmlAddChild(node, xmlNewNodeSAFI((char *)"SAFI", attr->getSAFI()));

    /* Next hop */
    string nh_str = "";
    static char str[INET6_ADDRSTRLEN];

	IPAddress addr = attr->getNextHopAddress();
	switch(attr->getAFI())
	{
		case AFI_IPv4: 
			inet_ntop(AF_INET, &(addr.ipv4), str, INET_ADDRSTRLEN);
			break;
		case AFI_IPv6: 
			inet_ntop(AF_INET6, &(addr.ipv6), str, INET6_ADDRSTRLEN);
			break;
	}
    nh_str += str;
    xmlAddChild(node, xmlNewNodeString((char *)"NEXT_HOP", (char *)nh_str.c_str()));

    /* NLRI */
	list<NLRIReachable> *nlri_reachable = attr->getNLRI();
    xmlNodePtr nlri_node = xmlNewNode(NULL, BAD_CAST "NLRI");
    xmlNewPropInt(nlri_node, (char *)"count", nlri_reachable->size());
    xmlAddChild(node, nlri_node);

	list<NLRIReachable>::iterator routeIter;
    for (routeIter = nlri_reachable->begin(); routeIter != nlri_reachable->end(); routeIter++)
    {
        Route rt = *routeIter;
        xmlAddChild(nlri_node, genPrefixNode(rt, attr->getAFI(), attr->getSAFI()));
    }

    return node;
}

// vim: sw=4 ts=4 sts=4 expandtab
