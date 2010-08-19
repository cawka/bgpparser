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
#include "AttributeTypeOriginatorID.h"

extern "C" {
    #include "xmlinternal.h"
}

AttributeTypeOriginatorIDDumper::AttributeTypeOriginatorIDDumper(AttributeType* attr)
: AttributeTypeDumper(attr)
{}

AttributeTypeOriginatorIDDumper::~AttributeTypeOriginatorIDDumper()
{}

xmlNodePtr AttributeTypeOriginatorIDDumper::genXml()
{
    AttributeTypeOriginatorID *attr = (AttributeTypeOriginatorID *)attr_type;

    static char oid_str[INET6_ADDRSTRLEN];
    oid_str[0] = '\0';

    uint32_t router_id = attr->getOrigin();
    inet_ntop(AF_INET, &router_id, oid_str, INET_ADDRSTRLEN);

    xmlNodePtr node = xmlNewNodeString("ORIGINATOR_ID",  oid_str);
    return node;
}

string AttributeTypeOriginatorIDDumper::genAscii()
{
    string node = "";
    /*
    // Not supported in ascii output
    AttributeTypeOriginatorID *attr = (AttributeTypeOriginatorID *)attr_type;
    */
    return node;
}

// vim: sw=4 ts=4 sts=4 expandtab
