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
#include "BGPAttribute.h"
#include "BGPAttributeDumper.h"
#include "AttributeTypeDumper.h"

extern "C" {
    #include "xmlinternal.h"
}

BGPAttributeDumper::BGPAttributeDumper( const BGPAttributePtr &attr )
{
    bgp_attr = attr;
}

BGPAttributeDumper::~BGPAttributeDumper()
{}

BGPAttributeDumperPtr BGPAttributeDumper::newDumper( const BGPAttributePtr &attr )
{
    return BGPAttributeDumperPtr( new BGPAttributeDumper(attr) );
}

xmlNodePtr BGPAttributeDumper::genXml()
{
    xmlNodePtr node      = xmlNewNode(NULL, BAD_CAST "ATTRIBUTE");

    /* Flag */
    xmlNodePtr flag_node = xmlNewNode(NULL, BAD_CAST "FLAG");
    xmlAddChild(node, flag_node);
    if ( bgp_attr->isOptional()       ) xmlAddChild(flag_node, xmlNewNode(NULL, BAD_CAST "OPTIONAL")  );
    if ( bgp_attr->isTransitive()     ) xmlAddChild(flag_node, xmlNewNode(NULL, BAD_CAST "TRANSITIVE"));
    if ( bgp_attr->isPartial()        ) xmlAddChild(flag_node, xmlNewNode(NULL, BAD_CAST "PARTIAL")   );
    if ( bgp_attr->isExtendedLength() ) xmlAddChild(flag_node, xmlNewNode(NULL, BAD_CAST "EXTENDED")  );

    /* Length */
    xmlNewChildInt(node, "LENGTH", bgp_attr->getAttributeLength());

    /* Type */
    string type_str = bgp_attr->getAttributeTypeStr();
    xmlNodePtr type_node = xmlNewChildString( node, "TYPE", type_str.c_str() );
    xmlNewPropInt(type_node, "value", bgp_attr->getAttributeTypeCode());

    /* Attribute xml node */
    AttributeTypeDumperPtr attr_dumper =
    		AttributeTypeDumper::newDumper( bgp_attr->getAttributeTypeCode(), bgp_attr->getAttributeTypeStr(), bgp_attr->getAttributeValue() );

    xmlAddChild( node, attr_dumper->genXml() );

    return node;
}

string BGPAttributeDumper::genAscii()
{
    string node = "";

    /* Attribute xml node */
    AttributeTypeDumperPtr attr_dumper =
    		AttributeTypeDumper::newDumper(bgp_attr->getAttributeTypeCode(), bgp_attr->getAttributeTypeStr(), bgp_attr->getAttributeValue());
    node += attr_dumper->genAscii();

    return node;
}


// vim: sw=4 ts=4 sts=4 expandtab
