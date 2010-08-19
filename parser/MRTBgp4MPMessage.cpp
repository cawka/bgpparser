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
#include "MRTBgp4MPMessage.h"
#include "BGPCommonHeader.h"
#include "BGPUpdate.h"
#include "BGPNotification.h"
#include "BGPKeepAlive.h"
#include "BGPOpen.h"
#include "BGPRouteRefresh.h"

MRTBgp4MPMessage::MRTBgp4MPMessage(void) {
	/* nothing */
	payload = NULL;
}

MRTBgp4MPMessage::MRTBgp4MPMessage(uint8_t **ptr, bool _isAS4) 
: MRTCommonHeader((const uint8_t **)ptr) 
{
	uint8_t *p;
	uint8_t nBytesRead = 0;

	/* add sizeof(MRTCommonHeaderPacket) to ptr since ptr points to base of message */
	p = const_cast<uint8_t *>(*ptr) + sizeof(MRTCommonHeaderPacket);
	isAS4 = _isAS4;

	if( !isAS4 ) {
		MRTBgp4MPMessagePacket pkt;
		memcpy(&pkt, p, sizeof(MRTBgp4MPMessagePacket));
		peerAS = ntohs(pkt.peerAS);
		localAS = ntohs(pkt.localAS);
		interfaceIndex = ntohs(pkt.interfaceIndex);
		addressFamily = ntohs(pkt.addressFamily);
		p += sizeof(MRTBgp4MPMessagePacket);
		nBytesRead += sizeof(MRTBgp4MPMessagePacket);
	} else {
		MRTBgp4MPMessageAS4Packet pkt;
		memcpy(&pkt, p, sizeof(MRTBgp4MPMessageAS4Packet));
		peerAS = ntohl(pkt.peerAS);
		localAS = ntohl(pkt.localAS);
		uint32_t pt, pb, lt, lb;
		pt = (uint32_t)((peerAS>>16)&0xffff);
		pb = (uint32_t)((peerAS)&0xffff);
		lt = (uint32_t)((localAS>>16)&0xffff);
		lb = (uint32_t)((localAS)&0xffff);
		interfaceIndex = ntohs(pkt.interfaceIndex);
		addressFamily = ntohs(pkt.addressFamily);
		p += sizeof(MRTBgp4MPMessageAS4Packet);
		nBytesRead += sizeof(MRTBgp4MPMessagePacket);
	}

	/* increment pointer p by sizeof(MRTBgp4MPMessagePacket) to */
	/* point to beginning of variable length IP addresses */

	if (addressFamily == AFI_IPv4) {
		memcpy(&peerIP, p, sizeof(peerIP.ipv4));
		p += sizeof(peerIP.ipv4);
		nBytesRead += sizeof(peerIP.ipv4);
		memcpy(&localIP, p, sizeof(localIP.ipv4));
		p += sizeof(localIP.ipv4);
		nBytesRead += sizeof(localIP.ipv4);
	} else if (addressFamily == AFI_IPv6) {
		memcpy(&peerIP, p, sizeof(peerIP.ipv6));
		p += sizeof(peerIP.ipv6);
		nBytesRead += sizeof(peerIP.ipv6);
		memcpy(&localIP, p, sizeof(localIP.ipv6));
		p += sizeof(localIP.ipv6);
		nBytesRead += sizeof(localIP.ipv6);
	} else {
		Logger::err("unsupported address family [%u]", addressFamily);
	}

	/* TODO: pointer p now points to beginning of BGP message... parse BGP message */
	BGPMessage* bgpMsg = BGPCommonHeader::newMessage((uint8_t**)&p, isAS4, getLength()-nBytesRead);
	if (bgpMsg) {
		PRINT_DBGF("  bgpMsg.getType() = 0x%02x\n", bgpMsg->getType());
		switch(bgpMsg->getType()) {
		case BGPCommonHeader::OPEN: {
			PRINT_DBG("  case BGPCommonHeader::OPEN");
			BGPOpen* bgpOpen = (BGPOpen*)bgpMsg;
			payload = bgpOpen;
			}
			break;
		
		case BGPCommonHeader::UPDATE: {
			BGPUpdate* bgpUpdate = (BGPUpdate*)bgpMsg;
			PRINT_DBG("  case BGPCommonHeader::UPDATE");
			PRINT_DBG2("  bgpUpdate->length = ", bgpUpdate->getLength());
			PRINT_DBG2("  bgpUpdate->withdrawnRoutesLength = ", bgpUpdate->getWithdrawnRoutesLength());
			PRINT_DBG2("  bgpUpdate->pathAttributesLength = ", bgpUpdate->getPathAttributesLength());
			PRINT_DBG2("  bgpUpdate->nlriLength = ", bgpUpdate->getNlriLength());
			if( bgpUpdate->getWithdrawnRoutesLength() == 0 &&
				bgpUpdate->getPathAttributesLength() == 0 &&
				bgpUpdate->getNlriLength() == 0 ) {
				delete bgpUpdate;
				payload = NULL; 
			} else {
				payload = bgpUpdate;
			}
			}
			break;
		
		case BGPCommonHeader::NOTIFICATION: {
			BGPNotification* bgpNotification = (BGPNotification*)bgpMsg;
			payload = bgpNotification;
			}
			break;
		
		case BGPCommonHeader::KEEPALIVE: {
			BGPKeepAlive* bgpKeepAlive = (BGPKeepAlive*)bgpMsg;
			payload = bgpKeepAlive;
			}
			break;
		
		case BGPCommonHeader::ROUTE_REFRESH: {
			BGPRouteRefresh* bgpRouteRefresh = (BGPRouteRefresh*)bgpMsg;
			payload = bgpRouteRefresh;
			}
			break;
		}
	}
	else {
		Logger::err("bgp message is NULL.");
	}
	/* TODO: increment the pointer to the new location in the file stream */
	*ptr = p;
}

MRTBgp4MPMessage::~MRTBgp4MPMessage(void) {
	if (payload != NULL)
	{
		delete (BGPMessage*)payload;
		payload = NULL;
	}
}

uint32_t MRTBgp4MPMessage::getPeerAS(void) const {
	return peerAS;
}

uint32_t MRTBgp4MPMessage::getLocalAS(void) const {
	return localAS;
}

uint16_t MRTBgp4MPMessage::getInterfaceIndex(void) const {
	return interfaceIndex;
}

uint16_t MRTBgp4MPMessage::getAddressFamily(void) const {
	return addressFamily;
}

IPAddress MRTBgp4MPMessage::getPeerIP(void) const {
	return peerIP;
}

IPAddress MRTBgp4MPMessage::getLocalIP(void) const {
	return localIP;
}
