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
#include <libxml/parser.h>
#include "BGPUpdate.h"
#include "BGPAttribute.h"
#include "BGPDumper.h"
#include "BGPAttributeDumper.h"

extern "C" {
    #include "xmlinternal.h"
}

BGPUpdateDumper::BGPUpdateDumper(BGPMessage* msg) : BGPDumper(msg)
{}

BGPUpdateDumper::~BGPUpdateDumper()
{}

xmlNodePtr BGPUpdateDumper::genPrefixNode(Route rt, uint32_t afi, uint32_t safi)
{
    xmlNodePtr node = NULL;

    //[TODO] afi / safi property

    /* generate prefix node */
    string tt = rt.toString();
    node = xmlNewNodeString("PREFIX", (char *)tt.c_str());

    return node;
}

xmlNodePtr BGPUpdateDumper::genUpdateWithdrawnNode(list<Route>* withdrawnRoutes)
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "WITHDRAWN");
    xmlNewPropInt(node, "count", withdrawnRoutes->size());

	list<Route>::iterator routeIter;
    for (routeIter = withdrawnRoutes->begin(); routeIter != withdrawnRoutes->end(); routeIter++)
    {
        //Route rt = *routeIter;
        xmlAddChild(node, genPrefixNode(*routeIter, 1, 1));
    }
    return node;
}

xmlNodePtr BGPUpdateDumper::genUpdateNlriNode(list<Route>* routes)
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "NLRI");
    xmlNewPropInt(node, "count", routes->size());

	list<Route>::iterator routeIter;
    for (routeIter = routes->begin(); routeIter != routes->end(); routeIter++)
    {
        //Route rt = *routeIter;
        xmlAddChild(node, genPrefixNode(*routeIter, 1, 1));
    }
    return node;
}

xmlNodePtr BGPUpdateDumper::genUpdateAttributesNode(list<BGPAttribute>* pathAttributes)
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "PATH_ATTRIBUTES");
    xmlNewPropInt(node, "count", pathAttributes->size());

	list<BGPAttribute>::iterator attrIter;
    for (attrIter = pathAttributes->begin(); attrIter != pathAttributes->end(); attrIter++)
    {
		BGPAttribute *attr = &(*attrIter);
        BGPAttributeDumper *attr_dumper = BGPAttributeDumper::newDumper(attr);
        xmlAddChild(node, attr_dumper->genXml()); 
        delete attr_dumper;
    }
    return node;
}

xmlNodePtr BGPUpdateDumper::genXmlAscii()
{
    BGPUpdate *bgp_update = (BGPUpdate *)bgp_msg;

    /* BGP Update node */
    xmlNodePtr bgp_node = xmlNewNode(NULL, BAD_CAST "UPDATE");
    
    /* Child nodes */
    /* WITHDRAWN_LEN  */ xmlAddChild(bgp_node, xmlNewNodeInt("WITHDRAWN_LEN",  bgp_update->getWithdrawnRoutesLength()));
    /* WITHDRAWN      */ xmlAddChild(bgp_node, genUpdateWithdrawnNode(bgp_update->getWithdrawnRoutes()));
    /* ATTRIBUTES_LEN */ xmlAddChild(bgp_node, xmlNewNodeInt("PATH_ATTRIBUTES_LEN", bgp_update->getPathAttributesLength()));
    /* ATTRIBUTES     */ xmlAddChild(bgp_node, genUpdateAttributesNode(bgp_update->getPathAttributes()));
    /* NLRI           */ xmlAddChild(bgp_node, genUpdateNlriNode(bgp_update->getNlriRoutes()));

    return bgp_node;
}

// vim: sw=4 ts=4 sts=4 expandtab
