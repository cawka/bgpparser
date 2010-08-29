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
#include <libxml/tree.h>
#include "AttributeTypeDumper.h"
#include "AttributeTypeAggregator.h"
#include "AttributeTypeAS4Aggregator.h"

extern "C" {
    #include "xmlinternal.h"
}

AttributeTypeAggregatorDumper::AttributeTypeAggregatorDumper( const AttributeTypePtr &attr )
: AttributeTypeDumper(attr)
{
}

AttributeTypeAggregatorDumper::~AttributeTypeAggregatorDumper()
{}

xmlNodePtr AttributeTypeAggregatorDumper::genXml()
{
    AttributeTypeAggregatorPtr attr = dynamic_pointer_cast<AttributeTypeAggregator>( attr_type );
    //xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "AGGREGATOR");
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST type_str.c_str()); /* AGGREGATOR or AS4_AGGREGATOR */

    uint32_t as =
			(type_code==AttributeType::AGGREGATOR) ?
					dynamic_pointer_cast<AttributeTypeAggregator>(attr_type)->getAggregatorLastASComplete()
			: /*type_code==AttributeType::NEW_AGGREGATOR*/
					dynamic_pointer_cast<AttributeTypeAS4Aggregator>(attr_type)->getAggregatorLastAS();

    xmlNewChildInt(   node, "AS",   as);
    xmlNewChildString(node, "ADDR", FORMAT_IPv4_ADDRESS( attr->getAggregatorBGPSpeakerIPAddress().ipv4 ).c_str());
    return node;
}

string AttributeTypeAggregatorDumper::genAscii()
{
    AttributeTypeAggregatorPtr attr = dynamic_pointer_cast<AttributeTypeAggregator>( attr_type );
    //xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "AGGREGATOR");
    ostringstream node;

    uint32_t as =
			(type_code==AttributeType::AGGREGATOR) ?
					dynamic_pointer_cast<AttributeTypeAggregator>(attr_type)->getAggregatorLastASComplete()
			: /*type_code==AttributeType::NEW_AGGREGATOR*/
					dynamic_pointer_cast<AttributeTypeAS4Aggregator>(attr_type)->getAggregatorLastAS();

    node << (int)as << " " << FORMAT_IPv4_ADDRESS( attr->getAggregatorBGPSpeakerIPAddress().ipv4 );
    return node.str( );
}

// vim: sw=4 ts=4 sts=4 expandtab
