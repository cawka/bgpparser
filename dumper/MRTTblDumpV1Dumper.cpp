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

#include "MRTTblDumpV1Dumper.h"
#include "FakeBGPUpdate.h"
#include "BGPMessageDumper.h"
#include "BGPCommonHeader.h"
#include "BGPStructure.h"
#include "AttributeType.h"
#include "AttributeTypeMPReachNLRI.h"
#include "AttributeTypeMPUnreachNLRI.h"

#include <libxml/tree.h>
#include <list>
#include <boost/foreach.hpp>

extern "C" {
    #include "xmlinternal.h"
}

MRTTblDumpV1Dumper::MRTTblDumpV1Dumper()
{
}

MRTTblDumpV1Dumper::~MRTTblDumpV1Dumper()
{
}

xmlNodePtr MRTTblDumpV1Dumper::genXml()
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "BGP_MESSAGES");

    // Prefix / Route
    NLRIReachablePtr   route(   new NLRIReachable(  tblDumpMsg->getPrefixLength(), tblDumpMsg->getPrefix()) );
    NLRIUnReachablePtr unroute( new NLRIUnReachable(tblDumpMsg->getPrefixLength(), tblDumpMsg->getPrefix()) );

    shared_ptr<FakeBGPUpdate> update( new FakeBGPUpdate() );

    /* Fill in the update members */
    BOOST_FOREACH( const BGPAttributePtr &attr, tblDumpMsg->getAttributes() )
    {
        /* Multiple Protocol */
        if( attr->getAttributeTypeCode() == AttributeType::MP_REACH_NLRI )
        { 
            AttributeTypeMPReachNLRIPtr mp_attr = dynamic_pointer_cast<AttributeTypeMPReachNLRI>( attr->getAttributeValueMutable() );
            //mp_attr->setAFI(this->tblDumpMsg->getAFI());
            //mp_attr->setSAFI(this->tblDumpMsg->getSAFI());
            mp_attr->addNLRI( route );
        }
        if (attr->getAttributeTypeCode() == AttributeType::MP_UNREACH_NLRI)
        { 
            AttributeTypeMPUnreachNLRIPtr mp_attr = dynamic_pointer_cast<AttributeTypeMPUnreachNLRI>( attr->getAttributeValueMutable() );
            //mp_attr->setAFI(this->tblDumpMsg->getAFI());
            //mp_attr->setSAFI(this->tblDumpMsg->getSAFI());
            mp_attr->addNLRI( unroute );
        }
        update->addPathAttribute( attr );
    }

    /* Add IPV4/UNICAST NLRI */
    update->addAnnouncedRoute( static_pointer_cast<Route>(route) );

    /* Message Dumper */
    BGPMessageDumperPtr bgpmsg_dumper ( new BGPMessageDumper(update) );

    /* Collect infomation */
    bgpmsg_dumper->setTimestamp(tblDumpMsg->getTimestamp());
    bgpmsg_dumper->setPeering(
                                tblDumpMsg->getPeerIP(), // LocalIP
                                tblDumpMsg->getPeerIP(), // PeerIP
                                tblDumpMsg->getPeerAS(), // LocalAS
                                tblDumpMsg->getPeerAS(), // LocalAS
                                0,                       // Interface Index
                                AFI_IPv4                 // IPV4
                             );
    xmlAddChild(node, bgpmsg_dumper->genXml());

    return node;
}

string MRTTblDumpV1Dumper::genAscii()
{
	// Prefix / Route
	NLRIReachablePtr   route(   new NLRIReachable(  tblDumpMsg->getPrefixLength(), tblDumpMsg->getPrefix()) );
	NLRIUnReachablePtr unroute( new NLRIUnReachable(tblDumpMsg->getPrefixLength(), tblDumpMsg->getPrefix()) );

	shared_ptr<FakeBGPUpdate> update( new FakeBGPUpdate() );

	/* Fill in the update members */
	BOOST_FOREACH( const BGPAttributePtr &attr, tblDumpMsg->getAttributes() )
	{
	   /* Multiple Protocol */
		if( attr->getAttributeTypeCode() == AttributeType::MP_REACH_NLRI )
		{
			AttributeTypeMPReachNLRIPtr mp_attr = dynamic_pointer_cast<AttributeTypeMPReachNLRI>( attr->getAttributeValueMutable() );
			//mp_attr->setAFI(this->tblDumpMsg->getAFI());
			//mp_attr->setSAFI(this->tblDumpMsg->getSAFI());
			mp_attr->addNLRI( route );
		}
		if (attr->getAttributeTypeCode() == AttributeType::MP_UNREACH_NLRI)
		{
			AttributeTypeMPUnreachNLRIPtr mp_attr = dynamic_pointer_cast<AttributeTypeMPUnreachNLRI>( attr->getAttributeValueMutable() );
			//mp_attr->setAFI(this->tblDumpMsg->getAFI());
			//mp_attr->setSAFI(this->tblDumpMsg->getSAFI());
			mp_attr->addNLRI( unroute );
		}
		update->addPathAttribute( attr );
	}

	/* Add IPV4/UNICAST NLRI */
	update->addAnnouncedRoute( static_pointer_cast<Route>(route) );

    /* Message Dumper */
	BGPMessageDumperPtr bgpmsg_dumper ( new BGPMessageDumper(update) );

    /* Collect infomation */
	bgpmsg_dumper->setTimestamp(tblDumpMsg->getTimestamp());
	bgpmsg_dumper->setPeering(
								tblDumpMsg->getPeerIP(), // LocalIP
								tblDumpMsg->getPeerIP(), // PeerIP
								tblDumpMsg->getPeerAS(), // LocalAS
								tblDumpMsg->getPeerAS(), // LocalAS
								0,                       // Interface Index
								AFI_IPv4                 // IPV4
							 );
    bgpmsg_dumper->isTableDump(true);

    string node = bgpmsg_dumper->genAscii();

    return node;
}

// vim: sw=4 ts=4 sts=4 expandtab
