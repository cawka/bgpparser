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

#include "BGPUpdate.h"
#include "BGPAttribute.h"
#include "BGPDumper.h"
#include "BGPAttributeDumper.h"
#include "AttributeTypeMPReachNLRI.h"
#include "AttributeTypeMPUnreachNLRI.h"

#include <sstream>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <boost/foreach.hpp>

extern "C" {
    #include "xmlinternal.h"
}

BGPUpdateDumper::BGPUpdateDumper( const BGPMessagePtr &msg ) : BGPDumper(msg)
{}

BGPUpdateDumper::~BGPUpdateDumper()
{}

xmlNodePtr BGPUpdateDumper::genPrefixNode( const RoutePtr &rt, uint32_t afi, uint32_t safi)
{
    xmlNodePtr node = NULL;

    //[TODO] afi / safi property

    /* generate prefix node */
    node = xmlNewNodeString("PREFIX", rt->toString().c_str());

    return node;
}

xmlNodePtr BGPUpdateDumper::genUpdateWithdrawnNode( const list<RoutePtr>& withdrawnRoutes)
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "WITHDRAWN");
    xmlNewPropInt(node, "count", withdrawnRoutes.size());

    BOOST_FOREACH( const RoutePtr &route, withdrawnRoutes )
    {
        xmlAddChild(node, genPrefixNode(route, 1, 1));
    }
    return node;
}

xmlNodePtr BGPUpdateDumper::genUpdateNlriNode( const list<RoutePtr>& routes)
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "NLRI");
    xmlNewPropInt(node, "count", routes.size());

	list<Route>::iterator routeIter;
    BOOST_FOREACH( const RoutePtr &route, routes )
    {
        //Route rt = *routeIter;
        xmlAddChild(node, genPrefixNode(route, 1, 1));
    }
    return node;
}

xmlNodePtr BGPUpdateDumper::genUpdateAttributesNode( const list<BGPAttributePtr>& pathAttributes )
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "PATH_ATTRIBUTES");
    xmlNewPropInt(node, "count", pathAttributes.size());

	list<BGPAttribute>::iterator attrIter;
    BOOST_FOREACH( const BGPAttributePtr &attr, pathAttributes )
    {
        BGPAttributeDumperPtr attr_dumper = BGPAttributeDumper::newDumper( attr );
        xmlAddChild(node, attr_dumper->genXml()); 
    }
    return node;
}

xmlNodePtr BGPUpdateDumper::genXmlAsciiNode()
{
    BGPUpdatePtr bgp_update = dynamic_pointer_cast<BGPUpdate>( bgp_msg );

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

list<string> BGPUpdateDumper::genAsciiMsg(string peer_addr, string peer_as, bool is_tabledump=false)
{
    BGPUpdatePtr bgp_update = dynamic_pointer_cast<BGPUpdate>( bgp_msg );
    list<string> bgp_msg_list;

    /* BGP Update node */
    // Attributes
    string as_path     = "";
    string origin      = "";
    string next_hop    = "";
    string med         = "0";
    string lpref       = "0";
    string agg         = "NAG";
    string aggregator  = "";
    string communities = "";

    BGPAttributePtr mp_reach;
    BGPAttributePtr mp_unreach;

    BOOST_FOREACH( const BGPAttributePtr &attr, bgp_update->getPathAttributes() )
    {
        BGPAttributeDumperPtr attr_dumper = BGPAttributeDumper::newDumper( attr );

        switch (attr->getAttributeTypeCode())
        {
            case 1: origin      = attr_dumper->genAscii(); break;
            case 2: as_path     = attr_dumper->genAscii(); break;
            case 3: next_hop    = attr_dumper->genAscii(); break;
            case 4: med         = attr_dumper->genAscii(); break;
            case 5: lpref       = attr_dumper->genAscii(); break;
            case 6: agg         = attr_dumper->genAscii(); break;
            case 7: aggregator  = attr_dumper->genAscii(); break;
            case 8: communities = attr_dumper->genAscii(); break;

            case 14: 
                    mp_reach   = attr;
                    break;
            case 15: 
                    mp_unreach = attr;
                    break;

            default:
                break;
        }
    }

    if( mp_reach.get()!=NULL )
    {
        AttributeTypeMPReachNLRIPtr attr = dynamic_pointer_cast<AttributeTypeMPReachNLRI>( mp_reach->getAttributeValue() );
        switch(attr->getAFI())
        {
            case AFI_IPv4: 
            	next_hop=FORMAT_IPv4_ADDRESS( attr->getNextHopAddress().ipv4 );
                break;
            case AFI_IPv6: 
                next_hop=FORMAT_IPv6_ADDRESS( attr->getNextHopAddress().ipv6 );
                break;
        }
    }

    ostringstream attr_str;
    attr_str << "|" << as_path      <<
                "|" << origin       <<
                "|" << next_hop     <<
                "|" << lpref        <<
                "|" << med          <<
                "|" << communities  <<
                "|" << agg          <<
                "|" << aggregator   << "|"
               ;

    /* WITHDRAWN */
    BOOST_FOREACH( const RoutePtr &route, bgp_update->getWithdrawnRoutes() )
    {
        string with = "";
        string prefix = route->toString();
        with = with + "W" + "|" + peer_addr + "|" + peer_as + "|" + prefix;
        bgp_msg_list.push_back(with);
    }

    if( mp_unreach.get() != NULL )
    {
        AttributeTypeMPUnreachNLRIPtr attr = dynamic_pointer_cast<AttributeTypeMPUnreachNLRI>( mp_unreach->getAttributeValue() );
        BOOST_FOREACH( const NLRIUnReachablePtr &route, attr->getNLRI() )
        {
            string with = "";
            string prefix = route->toString( attr->getAFI() );
            with = with + "W" + "|" + peer_addr + "|" + peer_as + "|" + prefix;
            bgp_msg_list.push_back(with);
        }
    }

    /* NLRI */
    string anno_type = ( is_tabledump ) ? "B" : "A";
    BOOST_FOREACH( const RoutePtr &route, bgp_update->getNlriRoutes() )
    {
        string anno = "";
        string prefix = route->toString( );
        anno = anno + anno_type + "|" + peer_addr + "|" + peer_as + "|" + prefix + attr_str.str();
        //anno = anno + "A" + "|\%s|\%d|" + prefix;
        bgp_msg_list.push_back(anno);
    }

    if( mp_reach.get()!=NULL )
    {
        AttributeTypeMPReachNLRIPtr attr = dynamic_pointer_cast<AttributeTypeMPReachNLRI>( mp_reach->getAttributeValue() );
        BOOST_FOREACH( const NLRIReachablePtr &route, attr->getNLRI() )
        {
            string anno = "";
            string prefix = route->toString(attr->getAFI());
            anno = anno + anno_type + "|" + peer_addr + "|" + peer_as + "|" + prefix + attr_str.str();
            bgp_msg_list.push_back(anno);
        }
    }

    return bgp_msg_list;
}

// vim: sw=4 ts=4 sts=4 expandtab
