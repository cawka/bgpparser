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

// Author: Paul Wang, Jason Ryder
// Modified: Jonathan Park (jpark@cs.ucla.edu)
#include "MRTTblDumpV2RibHeader.h"
#include "MRTTblDumpV2PeerIndexTbl.h"

MRTTblDumpV2PeerIndexTbl* MRTTblDumpV2RibHeader::_peerIndexTbl = NULL;

MRTTblDumpV2RibHeader::MRTTblDumpV2RibHeader(void) {
	/* nothing */
}

MRTTblDumpV2RibHeader::MRTTblDumpV2RibHeader(const uint8_t **ptr) : MRTCommonHeader(ptr) {
	ribs = new list<TblDumpV2RibEntry>();
}

MRTTblDumpV2RibHeader::~MRTTblDumpV2RibHeader(void) {
	// This frees each object in the list as well as the list object
	delete ribs;
	ribs = NULL;
}

uint32_t MRTTblDumpV2RibHeader::getSequenceNumber(void) const {
	return sequenceNumber;
}

uint8_t MRTTblDumpV2RibHeader::getPrefixLength(void) const {
	return prefixLength;
}

IPAddress MRTTblDumpV2RibHeader::getPrefix(void) const {
	return prefix;
}

uint16_t MRTTblDumpV2RibHeader::getEntryCount(void) const {
	return entryCount;
}

uint16_t MRTTblDumpV2RibHeader::getAFI(void) const {
	return afi;
}

uint16_t MRTTblDumpV2RibHeader::getSAFI(void) const {
	return safi;
}

list<TblDumpV2RibEntry> *MRTTblDumpV2RibHeader::getRibEntries(void) const {
	return ribs;
}

void MRTTblDumpV2RibHeader::parseRibEntry(list<TblDumpV2RibEntry> *ribs, uint16_t entryCount, uint8_t **ptr) {
	PRINT_DBG("MRTTblDumpV2RibHeader::parseRibEntry");
	uint8_t *p;
	TblDumpV2RibEntry* pRib;
	uint16_t peerIndex;
	uint32_t originatedTime;
	uint16_t attributeLength;
	int ii;

	/* cast away the const of ptr */
	p = const_cast<uint8_t *>(*ptr);

	/* TODO: parse each RIB entry... consider moving to parent class */
	for (int i = 0; i < entryCount; i++) {
		pRib = new TblDumpV2RibEntry();
		TblDumpV2RibEntry& rib = *pRib;
		memcpy(&peerIndex, p, sizeof(uint16_t));
		p += sizeof(uint16_t);
		peerIndex = ntohs(peerIndex);
		rib.setPeerIndex(peerIndex);
		PRINT_DBGF("set peer index to %u\n", (uint32_t)peerIndex);

		MRTTblDumpV2PeerIndexTbl* peerIndexTbl = MRTTblDumpV2RibHeader::getPeerIndexTbl();
		list<MRTTblDumpV2PeerIndexTblPeerEntry>::iterator myPeerEntry;
		for (ii = 0, myPeerEntry = peerIndexTbl->getPeerEntries()->begin(); 
			(ii < peerIndex) && (myPeerEntry != peerIndexTbl->getPeerEntries()->end()); 
			myPeerEntry++, ii++) { }

		memcpy(&originatedTime, p, sizeof(uint32_t));
		p += sizeof(uint32_t);
		originatedTime = ntohl(originatedTime);
		rib.setOriginatedTime(originatedTime);
		PRINT_DBGF("set originated time to %u\n", (uint32_t)originatedTime);

		memcpy(&attributeLength, p, sizeof(uint16_t));
		p += sizeof(uint16_t);
		attributeLength = ntohs(attributeLength);
		rib.setAttributeLength(attributeLength);
		PRINT_DBGF("set attribute length to %u\n", (uint32_t)attributeLength);
		
		/* TODO: dynamically allocate attributes array... */
		/* TODO: is attribute lenght in bytes or number of attributes? */
		uint8_t *pattr;
		uint8_t *attributes = (uint8_t *)malloc(sizeof(uint8_t) * rib.getAttributeLength());

		memcpy(attributes, p, sizeof(uint8_t) * rib.getAttributeLength());
		/* p now points to next entry */
		p = p + (0x0000FFFF & rib.getAttributeLength());
		/* create a pointer to dynamically allocated memory */
		pattr = attributes;
		
		/* TODO: parse each BGP attribute? */
		list<BGPAttribute> *attrs = rib.getAttributes();
		MRTTblDumpV2RibHeader::processAttributes(attrs, pattr, pattr + rib.getAttributeLength(), myPeerEntry->isAS4);
	
		/* free dynamically allocated memory */
		free(attributes);

		/* add RIB entry to list */
		ribs->push_back(rib);
		delete pRib;
	}

	/* DONE: update ptr */
	*ptr = p;
	PRINT_DBG("END MRTTblDumpV2RibHeader::parseRibEntry(...)");
}


void MRTTblDumpV2RibHeader::processAttributes(
	list<BGPAttribute> *attributes, 
	uint8_t *ptr, 
	const uint8_t * const endptr, 
	bool isAS4) {

	PRINT_DBG("MRTTblDumpV2RibHeader::processAttributes(...)");
	uint8_t *p;
	BGPAttribute attr;

	p = (uint8_t*)ptr;
	while (p < (uint8_t*)endptr) {
		BGPAttribute attrib(p,isAS4,0);
		attributes->push_back(attrib);
		p += attrib.totalSize();
	}
	PRINT_DBG("END MRTTblDumpV2RibHeader::processAttributes(...)");

	// Post processing
        list<BGPAttribute>::iterator attrIter;
	AttributeTypeASPath*  as_path_attr  = NULL;
	AttributeTypeAS4Path* as4_path_attr = NULL;
        for (attrIter = attributes->begin(); attrIter != attributes->end(); attrIter++)
        {
            if (attrIter->getAttributeTypeCode() == AttributeType::AS_PATH)     { as_path_attr  = (AttributeTypeASPath*)attrIter->getAttributeValueMutable();  }
            if (attrIter->getAttributeTypeCode() == AttributeType::NEW_AS_PATH) { as4_path_attr = (AttributeTypeAS4Path*)attrIter->getAttributeValueMutable(); }
        }
	if (as_path_attr != NULL)
	{
		as_path_attr->genPathSegmentsComplete(as4_path_attr);
	}
}


