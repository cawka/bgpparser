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

#include <string>
#include <sstream>
#include <libxml/tree.h>
#include "AttributeTypeDumper.h"
#include "AttributeTypeCommunities.h"
#include <boost/foreach.hpp>

extern "C" {
    #include "xmlinternal.h"
}

AttributeTypeCommunitiesDumper::AttributeTypeCommunitiesDumper( const AttributeTypePtr &attr )
: AttributeTypeDumper(attr)
{}

AttributeTypeCommunitiesDumper::~AttributeTypeCommunitiesDumper()
{}

xmlNodePtr AttributeTypeCommunitiesDumper::genXml()
{
    AttributeTypeCommunitiesPtr attr = dynamic_pointer_cast<AttributeTypeCommunities>( attr_type );
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "COMMUNITIES");

    const char *ctag = "COMMUNITY";
    const char *rtag = "RESERVED_COMMUNITY";
    const char *atag = "AS";
    const char *vtag = "VALUE";

	BOOST_FOREACH( const CommunityValue &value, attr->getCommunityValues() )
    {
        if ( value.ASnum == 0xFFFF && value.info ==  0xFF01 )
        {
            xmlAddChild(node, xmlNewNode(NULL, BAD_CAST "NO_EXPORT"));
        }
        else if ( value.ASnum == 0xFFFF && value.info == 0xFF02 )
        {
            xmlAddChild(node, xmlNewNode(NULL, BAD_CAST "NO_ADVERTISE"));
        }
        else if ( value.ASnum == 0xFFFF && value.info == 0xFF03 )
        {
            xmlAddChild(node, xmlNewNode(NULL, BAD_CAST "NO_EXPORT_SUBCONFED"));
        }
        else if ( value.ASnum == 0x0000 || value.ASnum == 0xFFFF )
        {
            xmlNodePtr child_node = xmlNewNode(NULL, BAD_CAST rtag);
            xmlNewChildInt(child_node, atag, value.ASnum);
            xmlNewChildInt(child_node, vtag, value.info);
            xmlAddChild(node, child_node);
        }
        else
        {
            xmlNodePtr child_node = xmlNewNode(NULL, BAD_CAST ctag);
            xmlNewChildInt(child_node, atag, value.ASnum);
            xmlNewChildInt(child_node, vtag, value.info);
            xmlAddChild(node, child_node);
        }
    }
    return node;
}

string AttributeTypeCommunitiesDumper::genAscii()
{
    AttributeTypeCommunitiesPtr attr = dynamic_pointer_cast<AttributeTypeCommunities>( attr_type );
    ostringstream node;
    string sep  = "";

    BOOST_FOREACH( const CommunityValue &value, attr->getCommunityValues() )
    {
        if ( value.ASnum == 0xFFFF && value.info == 0xFF01 )
        {
            node << sep << "no-export";
        }
        else
        {
        	node << sep << (int)value.ASnum << ":" << (int)value.info;
        }
        sep = " ";
    }
    return node.str();
}

// vim: sw=4 ts=4 sts=4 expandtab
