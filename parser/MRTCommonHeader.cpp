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
#include "MRTCommonHeader.h"

#include "MRTBgp4MPStateChange.h"
#include "MRTBgp4MPMessage.h"
#include "MRTBgp4MPEntry.h"
#include "MRTBgp4MPSnapshot.h"

#include "MRTTblDump.h"
#include "MRTTblDumpV2PeerIndexTbl.h"
#include "MRTTblDumpV2RibHeader.h"
#include "MRTTblDumpV2RibIPv4Unicast.h"
#include "MRTTblDumpV2RibIPv4Multicast.h"
#include "MRTTblDumpV2RibIPv6Unicast.h"
#include "MRTTblDumpV2RibIPv6Multicast.h"
#include "MRTTblDumpV2RibGeneric.h"

#include "BGPCommonHeader.h"

MRTCommonHeader::MRTCommonHeader(void) {
	/* nothing */
}


MRTCommonHeader::MRTCommonHeader(const uint8_t **ptr) {

	MRTCommonHeader();
	MRTCommonHeaderPacket pkt;
	memcpy(&pkt, *ptr, sizeof(MRTCommonHeaderPacket));

	timestamp = ntohl(pkt.timestamp);
	type = ntohs(pkt.type);
	subtype = ntohs(pkt.subtype);
	length = ntohl(pkt.length);
}

MRTCommonHeader::~MRTCommonHeader(void) {
	/* nothing */
}

time_t MRTCommonHeader::getTimestamp(void) const {
	return timestamp;
}

uint16_t MRTCommonHeader::getType(void) const {
	return type;
}

uint16_t MRTCommonHeader::getSubType(void) const {
	return subtype;
}

uint32_t MRTCommonHeader::getLength(void) const {
	return length;
}

void MRTCommonHeader::setType(uint16_t t) {
	type = t; 
}

void MRTCommonHeader::setSubType(uint16_t st) {
	subtype = st;
}

void MRTCommonHeader::setLength(uint32_t len) {
	length = len;
}

/* 
 * Factory method for creating MRTMessages
 */ 
MRTMessage * MRTCommonHeader::newMessage(uint8_t **ptr) {
	PRINT_DBG("MRTCommonHeader::newMessage(uint8_t **ptr)");
	//fprintf(stderr, "address of pointer: %p\n", *ptr);
	MRTMessage *msg = NULL;
	MRTCommonHeader header((const uint8_t **)ptr);
	
	PRINT_DBGF("  Type:      %u\n", header.getType());
	PRINT_DBGF("  SubType:   %u\n", header.getSubType());
	PRINT_DBGF("  Length:    %u\n", header.getLength());

	/* only accept BGP4MP message and TABLE_DUMP message types */
	if ((header.getType() != BGP4MP) && /*(header.getType() != BGP4MP_ET) && */
		(header.getType() != TABLE_DUMP) && (header.getType() != TABLE_DUMP_V2)) {
		*ptr += sizeof(MRTCommonHeaderPacket) + header.getLength();
		return NULL;
	}

	if (header.getType() == BGP4MP) {
		bool isAS4 = false;
		switch (header.getSubType()) {
		case BGP4MP_STATE_CHANGE:
			PRINT_DBG("  Creating MRTBgp4MPStateChange(ptr)");
			msg = new MRTBgp4MPStateChange(ptr, isAS4);
			break;
		case BGP4MP_MESSAGE:
			PRINT_DBG("  Creating MRTBgp4MPMessage(ptr);");
			msg = new MRTBgp4MPMessage(ptr, isAS4);
			break;
		case BGP4MP_STATE_CHANGE_AS4:
			PRINT_DBG("  Creating MRTBgp4MPStateChange(ptr);");
			isAS4 = true;
			msg = new MRTBgp4MPStateChange(ptr, isAS4);
			break;
		case BGP4MP_MESSAGE_AS4:
			PRINT_DBG("  Creating MRTBgp4MPMessage(ptr);");
			isAS4 = true;
			msg = new MRTBgp4MPMessage(ptr,isAS4);
			break;

		case BGP4MP_ENTRY:
			//PRINT_DBG("  Case BGP4MP_ENTRY");
			//msg = new MRTBgp4MPEntry(ptr);		/* not supported yet */
			//break;
		case BGP4MP_SNAPSHOT:
			//PRINT_DBG("  Case BGP4MP_SNAPSHOT");
			//msg = new MRTBgp4MPSnapshot(ptr);		/* not supported yet */
			//break;
		default:
			Logger::err("unrecognized subtype [%u] for bgp4mp.", header.getSubType());
			/* if failed to parse packet then update packet pointer */
			*ptr += sizeof(MRTCommonHeaderPacket) + header.getLength();
			break;
		}
	} else if ((header.getType() == TABLE_DUMP) || (header.getType() == TABLE_DUMP_V2)) {
		switch (header.getType()) {
		case TABLE_DUMP:
			msg = new MRTTblDump(ptr);
			break;
		case TABLE_DUMP_V2:
			switch (header.getSubType()) {
			case PEER_INDEX_TABLE:
				msg = new MRTTblDumpV2PeerIndexTbl(ptr);
			        MRTTblDumpV2RibHeader::setPeerIndexTbl((MRTTblDumpV2PeerIndexTbl*)msg);
				break;
			case RIB_IPV4_UNICAST:
				msg = new MRTTblDumpV2RibIPv4Unicast(ptr);
				break;
			case RIB_IPV4_MULTICAST:
				msg = new MRTTblDumpV2RibIPv4Multicast(ptr);
				break;
			case RIB_IPV6_UNICAST:
				msg = new MRTTblDumpV2RibIPv6Unicast(ptr);
				break;
			case RIB_IPV6_MULTICAST:
				msg = new MRTTblDumpV2RibIPv6Multicast(ptr);
				break;
			case RIB_GENERIC:
				msg = new MRTTblDumpV2RibGeneric(ptr);
				break;
			default:
				Logger::err("unrecognized subtype [%u] for table-dump-v2.", header.getSubType());
				/* if failed to parse packet then update packet pointer */
				ptr += sizeof(MRTCommonHeaderPacket) + header.getLength();
				break;
			}
			break;
		default:
			Logger::err("unrecognized type [%u] for table-dump-v2", header.getType());
			/* if failed to parse packet then update packet pointer */
			*ptr += sizeof(MRTCommonHeaderPacket) + header.getLength();
			break;
		}
	} else {
		*ptr += sizeof(MRTCommonHeaderPacket) + header.getLength();
	}
	PRINT_DBG("END MRTCommonHeader::newMessage(uint8_t **ptr)");
	return msg;
}
