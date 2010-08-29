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
#include "AttributeTypeClusterList.h"
#include <boost/foreach.hpp>

extern "C" {
    #include "xmlinternal.h"
}

AttributeTypeClusterListDumper::AttributeTypeClusterListDumper( const AttributeTypePtr &attr )
: AttributeTypeDumper(attr)
{}

AttributeTypeClusterListDumper::~AttributeTypeClusterListDumper()
{}

xmlNodePtr AttributeTypeClusterListDumper::genXml()
{
    AttributeTypeClusterListPtr attr = dynamic_pointer_cast<AttributeTypeClusterList>( attr_type );
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST type_str.c_str()); /* CLUSTER_LIST */

    BOOST_FOREACH( uint32_t cluster, attr->getClusterList() )
    {
    	in_addr addr;
    	addr.s_addr=cluster;
		xmlNewChildString(node, "ID", FORMAT_IPv4_ADDRESS(addr).c_str() );
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
