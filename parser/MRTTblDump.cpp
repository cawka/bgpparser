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
#include "MRTTblDump.h"

MRTTblDump::MRTTblDump(void) {
	attributes = new list<BGPAttribute>();
}


MRTTblDump::MRTTblDump(uint8_t **ptr) : MRTCommonHeader((const uint8_t **)ptr) {
	uint8_t *p;
	uint8_t *endptr;

	/* add sizeof(MRTCommonHeaderPacket) to ptr since ptr points to base of message */
	p = const_cast<uint8_t *>(*ptr) + sizeof(MRTCommonHeaderPacket);

	/* copy out the view number, increment the pointer and convert to host order */
	memcpy(&viewNumber, p, sizeof(uint16_t));
	p += sizeof(uint16_t);
	viewNumber = ntohs(viewNumber);

	/* copy out the sequence number, increment the pointer and convert to host order */
	memcpy(&sequenceNumber, p, sizeof(uint16_t));
	p += sizeof(uint16_t);
	sequenceNumber = ntohs(sequenceNumber);

	/* copy out the prefix and increment the pointer */
	if (getSubType() == AFI_IPv4) {
		memcpy(&(prefix.ipv4), p, sizeof(prefix.ipv4));
		p += sizeof(prefix.ipv4);
	} else if (getSubType() == AFI_IPv6) {
		memcpy(&(prefix.ipv6), p, sizeof(prefix.ipv6));
		p += sizeof(prefix.ipv6);
	} else {
		Logger::err("unsupported subtype [%u] for table-dump.", getSubType());
	}

	/* copy out the prefix length and increment the pointer */
	prefixLength = *p++;

	/* copy out the prefix length and increment the pointer */
	status = *p++;
	/* version 9 of the spec indicates that this field should always be set to 1 */
	status = (uint8_t)1;

	/* copy out the originated, increment the pointer and convert to host order */
	memcpy(&originatedTime, p, sizeof(uint32_t));
	p += sizeof(uint32_t);
	originatedTime = ntohl(originatedTime);

	/* copy out the peer IP and increment the pointer */
	if (getSubType() == AFI_IPv4) {
		memcpy(&(peerIP.ipv4), p, sizeof(peerIP.ipv4));
		p += sizeof(peerIP.ipv4);
	} else if (getSubType() == AFI_IPv6) {
		memcpy(&(peerIP.ipv6), p, sizeof(peerIP.ipv6));
		p += sizeof(peerIP.ipv6);
	} else {
		Logger::err("unsupported subtype [%u] for table-dump.", getSubType());
	}

	/* copy out the peer AS, increment the pointer and convert to host order */
	memcpy(&peerAS, p, sizeof(uint16_t));
	p += sizeof(uint16_t);
	peerAS = ntohs(peerAS);

	/* copy out the attribute length, increment the pointer and convert to host order */
	memcpy(&attributeLength, p, sizeof(uint16_t));
	p += sizeof(uint16_t);
	attributeLength = ntohs(attributeLength);

	/* TODO: p now points to beginning of BGP attributes... parse BGP attributes */
	attributes = new list<BGPAttribute>();
	endptr = p + attributeLength;
	MRTTblDumpV2RibHeader::processAttributes(attributes, p, endptr, false);
	/* increment pointer p to end of attributes */
	p += attributeLength;

	/* TODO: increment the pointer to the new location in the file stream */
	*ptr = p;
}

MRTTblDump::~MRTTblDump(void) {
	if (attributes != NULL) {
		list<BGPAttribute>::iterator it;

		for (it = attributes->begin(); it != attributes->end(); it++) {
			attributes->erase(it);
		}
		delete attributes;
	}
	attributes = NULL;
}

uint16_t MRTTblDump::getViewNumber(void) const {
	return viewNumber;
}

uint16_t MRTTblDump::getSequenceNumber(void) const {
	return sequenceNumber;
}

IPAddress MRTTblDump::getPrefix(void) const {
	return prefix;
}

uint8_t MRTTblDump::getPrefixLength(void) const {
	return prefixLength;
}

uint8_t MRTTblDump::getStatus(void) const {
	return status;
}

time_t MRTTblDump::getOriginatedTime(void) const {
	return originatedTime;
}

IPAddress MRTTblDump::getPeerIP(void) const {
	return peerIP;
}

uint16_t MRTTblDump::getPeerAS(void) const {
	return peerAS;
}

uint16_t MRTTblDump::getAttributeLength(void) const {
	return attributeLength;
}

void MRTTblDump::printMeCompact() {
	cout << viewNumber << "|" << sequenceNumber << "|";
	if (getSubType() == AFI_IPv4) {
		PRINT_IP_ADDR(prefix.ipv4);
	} else {
		PRINT_IPv6_ADDR(prefix.ipv6);
	}
	cout << "/";
	printf("%u", prefixLength);
	cout << "|";
	printf("%u", status);
	cout << "|";

	if (getSubType() == AFI_IPv4) {
		PRINT_IP_ADDR(peerIP.ipv4);
	} else {
		PRINT_IPv6_ADDR(peerIP.ipv6);
	}
	cout << "|" << peerAS;

	if (attributes != NULL) {
		list<BGPAttribute>::iterator it;
		for (it = attributes->begin(); it != attributes->end(); it++) {
			cout << "|";
			it->printMeCompact();
		}
	}
}
