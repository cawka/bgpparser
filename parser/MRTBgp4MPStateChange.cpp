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
#include "MRTBgp4MPStateChange.h"

MRTBgp4MPStateChange::MRTBgp4MPStateChange(void) {
	/* nothing */
}

MRTBgp4MPStateChange::MRTBgp4MPStateChange(uint8_t **ptr, bool _isAS4) 
: MRTCommonHeader((const uint8_t **)ptr) 
{
	uint8_t *p;

	/* add sizeof(MRTCommonHeaderPacket to ptr since ptr points to base of message */
	p = const_cast<uint8_t *>(*ptr) + sizeof(MRTCommonHeaderPacket);
	isAS4 = _isAS4;

	if( !isAS4 ) {
		MRTBgp4MPStateChangePacket pkt;
		memcpy(&pkt, p, sizeof(MRTBgp4MPStateChangePacket));
		peerAS = ntohs(pkt.peerAS);
		localAS = ntohs(pkt.localAS);
		interfaceIndex = ntohs(pkt.interfaceIndex);
		addressFamily = ntohs(pkt.addressFamily);
		p += sizeof(MRTBgp4MPStateChangePacket);
	} else {
		MRTBgp4MPStateChangeAS4Packet pkt;
		memcpy(&pkt, p, sizeof(MRTBgp4MPStateChangeAS4Packet));
		peerAS = ntohl(pkt.peerAS);
		localAS = ntohl(pkt.localAS);
		uint32_t pt, pb, lt, lb;
		pt = (uint32_t)((peerAS>>16)&0xffff);
		pb = (uint32_t)((peerAS)&0xffff);
		lt = (uint32_t)((localAS>>16)&0xffff);
		lb = (uint32_t)((localAS)&0xffff);
		interfaceIndex = ntohs(pkt.interfaceIndex);
		addressFamily = ntohs(pkt.addressFamily);
		p += sizeof(MRTBgp4MPStateChangeAS4Packet);
	}


	if (addressFamily == AFI_IPv4) {
		memcpy(&peerIP, p, sizeof(peerIP.ipv4));
		p += sizeof(peerIP.ipv4);
		memcpy(&localIP, p, sizeof(localIP.ipv4));
		p += sizeof(localIP.ipv4);
	} else if (addressFamily == AFI_IPv6) {
		memcpy(&peerIP, p, sizeof(peerIP.ipv6));
		p += sizeof(peerIP.ipv6);
		memcpy(&localIP, p, sizeof(localIP.ipv6));
		p += sizeof(localIP.ipv6);
	} else {
		Logger::err("unsupported address family [%u]", addressFamily);
	}

	/* pointer p now points to beginning of old state */
	memcpy(&oldState, p, sizeof(uint16_t));
	p += sizeof(uint16_t);
	oldState = ntohs(oldState);

	memcpy(&newState, p, sizeof(uint16_t));
	p += sizeof(uint16_t);
	newState = ntohs(newState);

	/* TODO: increment the pointer to the new location in the file stream */
	*ptr = p;
}

MRTBgp4MPStateChange::~MRTBgp4MPStateChange(void) {
	/* nothing */
}

uint32_t MRTBgp4MPStateChange::getPeerAS(void) const {
	return peerAS;
}

uint32_t MRTBgp4MPStateChange::getLocalAS(void) const {
	return localAS;
}

uint16_t MRTBgp4MPStateChange::getInterfaceIndex(void) const {
	return interfaceIndex;
}

uint16_t MRTBgp4MPStateChange::getAddressFamily(void) const {
	return addressFamily;
}

IPAddress MRTBgp4MPStateChange::getPeerIP(void) const {
	return peerIP;
}

IPAddress MRTBgp4MPStateChange::getLocalIP(void) const {
	return localIP;
}

uint16_t MRTBgp4MPStateChange::getOldState(void) const {
	return oldState;
}

uint16_t MRTBgp4MPStateChange::getNewState(void) const {
	return newState;
}

void MRTBgp4MPStateChange::printMe() {
	cout << "PEER_IP: ";
	switch(addressFamily) {
		case AFI_IPv4: PRINT_IP_ADDR(peerIP.ipv4); break;
		case AFI_IPv6: PRINT_IPv6_ADDR(peerIP.ipv6); break;
	}
	cout << endl;
	cout << "PEER_ASN: " << peerAS << endl;
	cout << "OLD_STATE: " << oldState << endl;
	cout << "NEW_STATE: " << newState << endl;
}

void MRTBgp4MPStateChange::printMeCompact() {
	uint16_t top, bottom;
	top = (peerAS>>16)&0xFFFF;
	bottom = (peerAS)&0XFFFF;
	switch(addressFamily) {
		case AFI_IPv4: PRINT_IP_ADDR(peerIP.ipv4); break;
		case AFI_IPv6: PRINT_IPv6_ADDR(peerIP.ipv6); break;
	}
	if( top > 0 ) {
		cout << "|" << top << "." << bottom << "|" << oldState << "|" << newState;
	} else {
		cout << "|" << bottom << "|" << oldState << "|" << newState;
	}
}
