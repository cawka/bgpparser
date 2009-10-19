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
#include "AttributeTypeClusterList.h"

extern "C" {
    #include "xmlinternal.h"
}

AttributeTypeClusterListDumper::AttributeTypeClusterListDumper(AttributeType* attr)
: AttributeTypeDumper(attr)
{}

AttributeTypeClusterListDumper::~AttributeTypeClusterListDumper()
{}

xmlNodePtr AttributeTypeClusterListDumper::genXml()
{
    AttributeTypeClusterList *attr = (AttributeTypeClusterList *)attr_type;
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST (char *)type_str.c_str()); /* CLUSTER_LIST */

    static char rid_str[INET6_ADDRSTRLEN];
    list<uint32_t>* clist = attr->getClusterList();
	if (clist != NULL)
	{	
		list<uint32_t>::iterator it;
		for (it = clist->begin(); it != clist->end(); it++)
		{
            rid_str[0] = '\0';
            inet_ntop(AF_INET, &(*it), rid_str, INET_ADDRSTRLEN);
            xmlNewChildString(node, "ID", rid_str);
        }
    }
    return node;
}

string AttributeTypeClusterListDumper::genAscii()
{
    string node = "";
    /*
    // Not supported in ascii output
    AttributeTypeClusterList *attr = (AttributeTypeClusterList *)attr_type;
    */
    return node;
}

// vim: sw=4 ts=4 sts=4 expandtab
