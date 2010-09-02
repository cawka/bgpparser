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
#include "BGPAttribute.h"

#include "BGPAttributeDumper.h"
#include "AttributeTypeDumper.h"

extern "C" {
    #include "xmlinternal.h"
}

AttributeTypeDumper::AttributeTypeDumper(AttributeType* attr)
{
    attr_type = attr;
}

AttributeTypeDumper::~AttributeTypeDumper()
{}

AttributeTypeDumper* AttributeTypeDumper::newDumper(uint8_t attrType, string attrTypeStr, AttributeType* attr)
{
	AttributeTypeDumper* attr_dumper = NULL;

	switch(attrType)
	{
		case AttributeType::ORIGIN:           { attr_dumper = new AttributeTypeOriginDumper(attr);          break; }
		case AttributeType::AS_PATH:          { attr_dumper = new AttributeTypeASPathDumper(attr);          break; }
		case AttributeType::NEXT_HOP:         { attr_dumper = new AttributeTypeNextHopDumper(attr);         break; }
		case AttributeType::MULTI_EXIT_DISC:  { attr_dumper = new AttributeTypeMultiExitDiscDumper(attr);   break; }
		case AttributeType::LOCAL_PREF:       { attr_dumper = new AttributeTypeLocalPrefDumper(attr);       break; }
		case AttributeType::ATOMIC_AGGREGATE: { attr_dumper = new AttributeTypeAtomicAggregateDumper(attr); break; }
		case AttributeType::AGGREGATOR:       { attr_dumper = new AttributeTypeAggregatorDumper(attr);      break; }
		case AttributeType::COMMUNITIES:      { attr_dumper = new AttributeTypeCommunitiesDumper(attr);     break; }
        case AttributeType::ORIGINATOR_ID:    { attr_dumper = new AttributeTypeOriginatorIDDumper(attr);    break; }
        case AttributeType::CLUSTER_LIST:     { attr_dumper = new AttributeTypeClusterListDumper(attr);     break; }
		case AttributeType::MP_REACH_NLRI:    { attr_dumper = new AttributeTypeMPReachNLRIDumper(attr);     break; }
		case AttributeType::MP_UNREACH_NLRI:  { attr_dumper = new AttributeTypeMPUnreachNLRIDumper(attr);   break; }
		case AttributeType::NEW_AS_PATH:      { attr_dumper = new AttributeTypeASPathDumper(attr);          break; }
		case AttributeType::NEW_AGGREGATOR:   { attr_dumper = new AttributeTypeAggregatorDumper(attr);      break; }
		default:
		{
			PRINT_INFO("  Unhandled attribute type code");
			attr_dumper = new AttributeTypeDumper(attr);
			break;
		}
	}
    attr_dumper->setTypeStr(attrTypeStr);
    attr_dumper->setTypeCode(attrType);

	return attr_dumper;
}

xmlNodePtr AttributeTypeDumper::genXml()
{
    xmlNodePtr node, octets_node;
    node        = xmlNewNode(NULL, BAD_CAST (char *)type_str.c_str());
    octets_node = xmlAddChild(node, xmlNewNodeOctets("OCTETS", attr_type->getValue(), attr_type->getLength()));
    return node;
}

string AttributeTypeDumper::genAscii()
{
    string node = "";
    return node;
}

// vim: sw=4 ts=4 sts=4 expandtab
