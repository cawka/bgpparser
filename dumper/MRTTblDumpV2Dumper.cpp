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
#include <list>
#include "MRTTblDumpV2Dumper.h"
#include "MRTTblDumpV2RibIPv4Unicast.h"
#include "MRTTblDumpV2RibIPv4Multicast.h"
#include "MRTTblDumpV2RibIPv6Unicast.h"
#include "MRTTblDumpV2RibIPv6Multicast.h"
#include "BGPUpdate.h"
#include "BGPMessageDumper.h"
#include "BGPCommonHeader.h"
#include "BGPStructure.h"
#include "AttributeType.h"

extern "C" {
    #include "xmlinternal.h"
}

MRTTblDumpV2Dumper::MRTTblDumpV2Dumper()
{
    peerIndexTbl = NULL;
    tblDumpMsg   = NULL;
}

MRTTblDumpV2Dumper::~MRTTblDumpV2Dumper()
{}

xmlNodePtr MRTTblDumpV2Dumper::genXml()
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "BGP_MESSAGES");

    /* [TODO] Check pointer validity */
    // Prefix
    /*
    //cerr << "Subtype1:" <<  this->peerIndexTbl->getSubType() << endl;
    //cerr << "Subtype2:" <<  this->tblDumpMsg->getSubType() << endl;
    PRINT_IP_ADDR(tblDumpMsg->getPrefix().ipv4);
	cout << "/" << (int)(tblDumpMsg->getPrefixLength() & BITMASK_8) << endl;
    tblDumpMsg->printMeCompact();
    cout << endl;
    */

    // Prefix / Route
    NLRIReachable   route(tblDumpMsg->getPrefixLength(), tblDumpMsg->getPrefix());
    NLRIUnReachable unroute(tblDumpMsg->getPrefixLength(), tblDumpMsg->getPrefix());

	int i = 0;
	list<TblDumpV2RibEntry>::iterator iter;
	list<MRTTblDumpV2PeerIndexTblPeerEntry>::iterator peerIterator;
	list<MRTTblDumpV2PeerIndexTblPeerEntry> *peerEntries = peerIndexTbl->getPeerEntries();
	list<TblDumpV2RibEntry>                 *ribs        = tblDumpMsg->getRibEntries();

	for (iter = ribs->begin(); iter != ribs->end(); iter++)
	{
        /* Peer */
        /* [TODO] Modify for better performance */
        for (
             i = 0, peerIterator = peerEntries->begin(); 
             (i < iter->getPeerIndex()) && (peerIterator != peerEntries->end()); 
             peerIterator++, i++
            )
        {
        /* find proper location into table */
        }

		//if (peerIterator->IPType == AFI_IPv4) { PRINT_IP_ADDR(peerIterator->peerIP.ipv4);   }
		//else                                  { PRINT_IPv6_ADDR(peerIterator->peerIP.ipv6); }

        /* Each Route Entry */
        BGPUpdate *update = new BGPUpdate(peerIterator->isAS4);


        /* Fill in the update members */
        list<BGPAttribute>* attrs      = iter->getAttributes();
        list<BGPAttribute>* path_attrs = update->getPathAttributes();
        list<BGPAttribute>::iterator attrIter;
        for (attrIter = attrs->begin(); attrIter != attrs->end(); attrIter++)
        {
            /* Multiple Protocol */
            if (attrIter->getAttributeTypeCode() == AttributeType::MP_REACH_NLRI)
            { 
                AttributeTypeMPReachNLRI* mp_attr = (AttributeTypeMPReachNLRI*)attrIter->getAttributeValueMutable();
                mp_attr->setAFI(this->tblDumpMsg->getAFI());
                mp_attr->setSAFI(this->tblDumpMsg->getSAFI());
                mp_attr->addNLRI(route);
            }
            if (attrIter->getAttributeTypeCode() == AttributeType::MP_UNREACH_NLRI)
            { 
                AttributeTypeMPUnreachNLRI* mp_attr = (AttributeTypeMPUnreachNLRI*)attrIter->getAttributeValueMutable();
                mp_attr->setAFI(this->tblDumpMsg->getAFI());
                mp_attr->setSAFI(this->tblDumpMsg->getSAFI());
                mp_attr->addNLRI(unroute);
            }
            path_attrs->push_back(*attrIter);
        }

        /* Add IPV4/UNICAST NLRI */
        if ( this->tblDumpMsg->getAFI() == AFI_IPv4 && this->tblDumpMsg->getSAFI() == SAFI_UNICAST)
        {
            list<Route>* nlri = update->getNlriRoutes();
            nlri->push_back(route);
        }

        /* Message Dumper */
        BGPMessageDumper *bgpmsg_dumper = new BGPMessageDumper();
        /* Collect infomation */
        bgpmsg_dumper->setTimestamp(tblDumpMsg->getTimestamp());
        bgpmsg_dumper->setPeering(
                                    peerIterator->peerIP, // LocalIP
                                    peerIterator->peerIP, // PeerIP
                                    peerIterator->peerAS, // LocalAS
                                    peerIterator->peerAS, // LocalAS
                                    0,                    // Interface Index
                                    peerIterator->IPType  // AFI
                                 );
        bgpmsg_dumper->setBGPMessage((BGPMessage*)update);
        xmlAddChild(node, bgpmsg_dumper->genXml());

        delete bgpmsg_dumper;
        delete update;
    }
    xmlNewPropInt(node, (char *)"count", ribs->size());

    return node;
}

// vim: sw=4 ts=4 sts=4 expandtab
