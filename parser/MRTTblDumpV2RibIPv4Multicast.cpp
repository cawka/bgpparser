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

// Modified: Jonathan Park (jpark@cs.ucla.edu)
#include "MRTTblDumpV2RibIPv4Multicast.h"

MRTTblDumpV2RibIPv4Multicast::MRTTblDumpV2RibIPv4Multicast(void) {
	/* nothing */
}

MRTTblDumpV2RibIPv4Multicast::MRTTblDumpV2RibIPv4Multicast(uint8_t **ptr) 
							 : MRTTblDumpV2RibHeader((const uint8_t **)ptr) {
	uint8_t *p;

	/* set AFI and SAFI */
	afi = AFI_IPv4;
	safi = SAFI_MULTICAST;

	/* add sizeof(MRTCommonHeaderPacket) to ptr since ptr points to base of message */
	p = const_cast<uint8_t *>(*ptr) + sizeof(MRTCommonHeaderPacket);

	/* copy out the sequence number, increment the pointer, and convert to host order */
	memcpy(&sequenceNumber, p, sizeof(uint32_t));
	p += sizeof(uint32_t);
	sequenceNumber = ntohl(sequenceNumber);

	/* copy out the prefix length and increment the pointer */
	prefixLength = *p++;

	/* zero the prefix field and copy out the bytes necessary for the prefix */
	memset(&prefix, 0, sizeof(IPAddress));
	memcpy(&prefix, p, ((uint32_t)prefixLength + 7) / 8);
	p += ((uint32_t)prefixLength + 7) / 8;

	/* copy out the entry count, increment the pointer, and convert to host order */
	memcpy(&entryCount, p, sizeof(uint16_t));
	p += sizeof(uint16_t);
	entryCount = ntohs(entryCount);

	/* TODO: parse each RIB entry... */
	MRTTblDumpV2RibHeader::parseRibEntry(ribs, entryCount, &p);

	/* TODO: increment the pointer to the new location in the file stream */
	*ptr = p;
}


MRTTblDumpV2RibIPv4Multicast::~MRTTblDumpV2RibIPv4Multicast(void) {
	/* nothing */
}

void MRTTblDumpV2RibIPv4Multicast::printMe() {
	cout << "PREFIX: ";
	PRINT_IP_ADDR(prefix.ipv4) ;
	cout << "/" << (int)(prefixLength & BITMASK_8) << endl;
	cout << "SEQUENCE: " << sequenceNumber;
}

void MRTTblDumpV2RibIPv4Multicast::printMe(MRTTblDumpV2PeerIndexTbl *peerIndexTbl) {
	printMe();
	cout << endl;
	// Now continue with infor from the peer index table
	list<TblDumpV2RibEntry>::iterator iter;
	for (iter = ribs->begin(); iter != ribs->end(); iter++) {
		int peerIndex = (*iter).getPeerIndex();
		list<struct _MRTTblDumpV2PeerIndexTblPeerEntry>::iterator indexIter;
		list<struct _MRTTblDumpV2PeerIndexTblPeerEntry>* peerEntries;
		if (peerIndexTbl) {
			peerEntries = peerIndexTbl->getPeerEntries();
			if (peerEntries == NULL)
				cout << "peerEntries == NULL" << endl;
			int i=0;
			for (indexIter = peerEntries->begin(); 
				 i < (int)peerIndex && i < (int)peerEntries->size() ; 
				 indexIter++, i++)
			{ /* Do nothing -- looking up the correct index */ }
		
			cout << "FROM: ";
			PRINT_IP_ADDR((*indexIter).peerIP.ipv4);
			cout << " AS" << (*indexIter).peerAS;
			cout << endl;
			
			(*iter).printMe();
			cout << endl;
		}
	}
}

void MRTTblDumpV2RibIPv4Multicast::printMeCompact() {
	PRINT_IP_ADDR(prefix.ipv4) ;
	cout << "/" << (int)(prefixLength & BITMASK_8);
	cout << "|" << sequenceNumber << "|";
}

void MRTTblDumpV2RibIPv4Multicast::printMeCompact(MRTTblDumpV2PeerIndexTbl *tbl) {
	int i = 0;
	list<TblDumpV2RibEntry>::iterator iter;
	list<MRTTblDumpV2PeerIndexTblPeerEntry> *peerEntries = tbl->getPeerEntries();
	list<MRTTblDumpV2PeerIndexTblPeerEntry>::iterator peerIterator;

	cout << "ENTRY_CNT: " << ribs->size() << endl;
	for (iter = ribs->begin(); iter != ribs->end(); iter++) {
		printMeCompact();

		for (i = 0, peerIterator = peerEntries->begin(); 
			(i < iter->getPeerIndex()) && (peerIterator != peerEntries->end()); 
			peerIterator++, i++) 
		{ /* find proper location into table */ }

		// print from IP
		if (peerIterator->IPType == AFI_IPv4) {
			PRINT_IP_ADDR(peerIterator->peerIP.ipv4);
		} else {
			PRINT_IPv6_ADDR(peerIterator->peerIP.ipv6);
		}

		// Print from AS
		uint16_t top, bottom;
		top = (uint16_t)((peerIterator->peerAS>>16)&0xFFFF);
		bottom = (uint16_t)((peerIterator->peerAS)&0xFFFF);
		printf("|");
		if( top == 0 ) {
			printf("%u",bottom);
		} else {
			printf("%u.%u",top,bottom);
		}
		printf("|");

		iter->printMeCompact();
		cout << endl;
	}
}
