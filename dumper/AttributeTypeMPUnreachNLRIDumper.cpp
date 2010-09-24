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

#include "AttributeTypeMPUnreachNLRI.h"
#include "AttributeTypeDumper.h"
#include <string>
#include <sstream>
#include <libxml/tree.h>
#include <boost/foreach.hpp>

extern "C" {
    #include "xmlinternal.h"
}

AttributeTypeMPUnreachNLRIDumper::AttributeTypeMPUnreachNLRIDumper( const AttributeTypePtr &attr )
: AttributeTypeDumper(attr)
{}

AttributeTypeMPUnreachNLRIDumper::~AttributeTypeMPUnreachNLRIDumper()
{}

xmlNodePtr AttributeTypeMPUnreachNLRIDumper::genPrefixNode(const NLRIUnReachablePtr &rt, int afi, int safi)
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "PREFIX");

    //[TODO] afi / safi

    /* generate prefix node */
    node = xmlNewNodeString((char *)"PREFIX", rt->toString(afi).c_str());

    return node;
}

xmlNodePtr AttributeTypeMPUnreachNLRIDumper::genXml()
{
	AttributeTypeMPUnreachNLRIPtr attr = dynamic_pointer_cast<AttributeTypeMPUnreachNLRI>( attr_type );

	xmlNodePtr node = xmlNewNode(NULL, BAD_CAST (char *)type_str.c_str()); /* AS_PATH or AS4_PATH */

    /* afi / safi */
    xmlAddChild(node, xmlNewNodeAFI((char *)"AFI",   attr->getAFI()));
    xmlAddChild(node, xmlNewNodeSAFI((char *)"SAFI", attr->getSAFI()));

    /* withdrawn */
    xmlNodePtr with_node = xmlNewNode(NULL, BAD_CAST "WITHDRAWN");
    xmlNewPropInt(with_node, "count", attr->getNLRI().size());
    xmlAddChild(node, with_node);

	list<NLRIUnReachable>::iterator routeIter;
    BOOST_FOREACH(const NLRIUnReachablePtr &rt, attr->getNLRI() )
    {
        xmlAddChild(with_node, genPrefixNode(rt, attr->getAFI(), attr->getSAFI()));
    }
    return node;
}

// vim: sw=4 ts=4 sts=4 expandtab
