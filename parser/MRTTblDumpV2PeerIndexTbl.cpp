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
#include <bgpparser.h>

#include "MRTTblDumpV2PeerIndexTbl.h"
using namespace std;

log4cxx::LoggerPtr MRTTblDumpV2PeerIndexTbl::Logger = log4cxx::Logger::getLogger( "bgpparser.MRTTblDumpV2PeerIndexTbl" );

MRTTblDumpV2PeerIndexTbl::MRTTblDumpV2PeerIndexTbl(void) {
	/* nothing */
}

MRTTblDumpV2PeerIndexTbl::MRTTblDumpV2PeerIndexTbl(uint8_t **ptr) 
						 : MRTCommonHeader((const uint8_t **)ptr) {
	uint8_t *p;
	uint8_t peerType;
	uint16_t peerAS;
	MRTTblDumpV2PeerIndexTblPeerEntry entry;

	/* add sizeof(MRTCommonHeaderPacket) to ptr since ptr points to base of message */
	p = const_cast<uint8_t *>(*ptr) + sizeof(MRTCommonHeaderPacket);

	/* copy out the collector BGP id, increment the pointer and convert to host order */
	memcpy(&collectorBGPId, p, sizeof(uint32_t));
	p += sizeof(uint32_t);
	collectorBGPId = ntohl(collectorBGPId);

	/* copy out the view name length, increment the pointer and convert to host order */
	memcpy(&viewNameLength, p, sizeof(uint16_t));
	p += sizeof(uint16_t);
	viewNameLength = ntohs(viewNameLength);

	if (viewNameLength > 0) {
		/* dynamically allocate memory to store view name */
		viewName = (uint8_t *)malloc(sizeof(uint8_t) * (viewNameLength + 1));
		if (viewName == NULL) {
			LOG4CXX_ERROR(Logger,"dynamic memory allocation of ["<< (viewNameLength+1) <<"] bytes failed." );
		} else {
			/* zero out memory of view name */
			memset(viewName, 0, sizeof(uint8_t) * (viewNameLength + 1));

			/* copy out the view name */
			memcpy(viewName, p, sizeof(uint8_t) * viewNameLength);

			/* increment pointer p */
			p += (sizeof(uint8_t) * viewNameLength);
		}
	} else {
		viewName = NULL;
	}

	/* copy out the peer count, increment the pointer and convert to host order */
	memcpy(&peerCount, p, sizeof(uint16_t));
	p += sizeof(uint16_t);
	peerCount = ntohs(peerCount);
	
	/* allocate peer entries list */
	peerEntries = new list<struct _MRTTblDumpV2PeerIndexTblPeerEntry>();

	/* parse out the peer entries */
	for (int i = 0; i < peerCount; i++) {
		memset(&entry, 0, sizeof(MRTTblDumpV2PeerIndexTblPeerEntry));

		/* copy out the peer type and increment the pointer */
		peerType = *p++;

		if ((peerType & 0x01) == 0) {
			/* IPv4 peer IP address */
			entry.IPType = AFI_IPv4;
		} else {
			/* IPv6 peer IP address */
			entry.IPType = AFI_IPv6;
		}
		if ((peerType & 0x02) == 0) {
			/* IPv4 peer IP address */
			entry.isAS4 = false;
		} else {
			/* IPv6 peer IP address */
			entry.isAS4 = true;
		}

		/* copy the peer BGP id, incrmement the pointer, conver to host order */
		memcpy(&(entry.peerBGPId), p, sizeof(uint32_t));
		p += sizeof(uint32_t);
		entry.peerBGPId = ntohl(entry.peerBGPId);

		/* copy the peer IP address and increment the pointer */
		if (entry.IPType == AFI_IPv4) {
			/* IPv4 */
			memcpy(&(entry.peerIP.ipv4), p, sizeof(entry.peerIP.ipv4));
			p += sizeof(entry.peerIP.ipv4);
		} else {
			/* IPv6 */
			memcpy(&(entry.peerIP.ipv6), p, sizeof(entry.peerIP.ipv6));
			p += sizeof(entry.peerIP.ipv6);
		}

		/* copy the peer AS and increment the pointer */
		if ((peerType & 0x02) == 0) {
			/* 16 bit peer AS */
			memcpy(&peerAS, p, sizeof(uint16_t));
			p += sizeof(uint16_t);
			entry.peerAS = ntohs(peerAS);
		} else {
			/* 32 bit peer AS */
			memcpy(&(entry.peerAS), p, sizeof(uint32_t));
			p += sizeof(uint32_t);
			entry.peerAS = ntohl(entry.peerAS);
		}

		/* add MRTTblDumpV2PeerIndexTblPeerEntry to list */
		peerEntries->push_back(entry);
	}

	/* TODO: increment the pointer to the new location in the file stream */
	if (*ptr + sizeof(MRTCommonHeaderPacket) + length != p) {
		// TODO: The pointer offset should only be worng if we're given bad data.
		*ptr += (sizeof(MRTCommonHeaderPacket) + length);
		return;
	}
	*ptr = p;
}


MRTTblDumpV2PeerIndexTbl::~MRTTblDumpV2PeerIndexTbl(void) {
	if (viewName != NULL)
		free(viewName);

	viewName = NULL;

	if (peerEntries != NULL)
		delete peerEntries;

	peerEntries = NULL;
}


uint32_t MRTTblDumpV2PeerIndexTbl::getCollectorBGPId(void) const {
	return collectorBGPId;
}

uint16_t MRTTblDumpV2PeerIndexTbl::getViewNameLength(void) const {
	return viewNameLength;
}

const uint8_t *MRTTblDumpV2PeerIndexTbl::getViewName(void) const {
	return viewName;
}

uint16_t MRTTblDumpV2PeerIndexTbl::getPeerCount(void) const {
	return peerCount;
}

list<struct _MRTTblDumpV2PeerIndexTblPeerEntry> *MRTTblDumpV2PeerIndexTbl::getPeerEntries() {
	return peerEntries;
}
