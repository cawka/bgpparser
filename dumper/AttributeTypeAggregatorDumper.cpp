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
#include "AttributeTypeAggregator.h"

extern "C" {
    #include "xmlinternal.h"
}

AttributeTypeAggregatorDumper::AttributeTypeAggregatorDumper(AttributeType* attr)
: AttributeTypeDumper(attr)
{}

AttributeTypeAggregatorDumper::~AttributeTypeAggregatorDumper()
{}

xmlNodePtr AttributeTypeAggregatorDumper::genXml()
{
    AttributeTypeAggregator *attr = (AttributeTypeAggregator *)attr_type;
    //xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "AGGREGATOR");
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST (char *)type_str.c_str()); /* AGGREGATOR or AS4_AGGREGATOR */

	IPAddress addr = attr->getAggregatorBGPSpeakerIPAddress();
    static char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.ipv4), str, INET_ADDRSTRLEN);

    xmlNewChildInt(node,    "AS",   attr->getAggregatorLastAS());
    xmlNewChildString(node, "ADDR", str);
    return node;
}

string AttributeTypeAggregatorDumper::genAscii()
{
    AttributeTypeAggregator *attr = (AttributeTypeAggregator *)attr_type;
    //xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "AGGREGATOR");
    string node = "";

	IPAddress addr = attr->getAggregatorBGPSpeakerIPAddress();
    static char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.ipv4), str, INET_ADDRSTRLEN);

    char buffer[64];
    buffer[0] = '\0';
    sprintf(buffer, "%d %s", attr->getAggregatorLastAS(), str);
    node += buffer;
    return node;
}

// vim: sw=4 ts=4 sts=4 expandtab
