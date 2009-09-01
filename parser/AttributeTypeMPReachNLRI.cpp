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
#include "AttributeTypeMPReachNLRI.h"

AttributeTypeMPReachNLRI::AttributeTypeMPReachNLRI(void) {
	afi  = 0;
	safi = 0;
	nextHopAddressLength = 0;
	memset(&nextHopAddress,      0, sizeof(IPAddress));
	memset(&nextHopAddressLocal, 0, sizeof(IPAddress));
	nlri = new list<NLRIReachable>();
	snpa = new list<NLRIReachable>();
	corrupt = 0;
}

AttributeTypeMPReachNLRI::AttributeTypeMPReachNLRI(const AttributeTypeMPReachNLRI &attrMPReachNLRI) 
						 : AttributeType(attrMPReachNLRI) {
	afi = attrMPReachNLRI.afi;
	safi = attrMPReachNLRI.safi;
	nextHopAddressLength = attrMPReachNLRI.nextHopAddressLength;
	memset(&nextHopAddress, 0, sizeof(IPAddress));
	memcpy(&nextHopAddress, &(attrMPReachNLRI.nextHopAddress), sizeof(IPAddress));
	memset(&nextHopAddressLocal, 0, sizeof(IPAddress));
	memcpy(&nextHopAddressLocal, &(attrMPReachNLRI.nextHopAddressLocal), sizeof(IPAddress));

	nlri = new list<NLRIReachable>(nlri->begin(),nlri->end());
	snpa = new list<NLRIReachable>(snpa->begin(),snpa->end());
	corrupt = attrMPReachNLRI.corrupt;
}

AttributeTypeMPReachNLRI::AttributeTypeMPReachNLRI(uint16_t len, uint8_t* msg, bool isCompact) 
						 : AttributeType(len, msg) {
	uint8_t *ptr = msg;
	uint8_t *endMsg = AttributeType::getEndMsg();

	nlri = new list<NLRIReachable>();
	snpa = new list<NLRIReachable>();
	corrupt = 0;

	// Test if there is only "NextHopLength" and "NextHopAddress" in this attribute (MRT TABLE_DUMP_V2 format)
	// http://tools.ietf.org/id/draft-ietf-grow-mrt-09.txt
	memcpy(&nextHopAddressLength, ptr, sizeof(uint8_t));
	if ( isCompact || len == 1 + nextHopAddressLength) {
		afi  = 0;
		safi = 0;
		memset(&nextHopAddress, 0,     sizeof(IPAddress));
		memcpy(&nextHopAddress, ptr+1, ((uint32_t)nextHopAddressLength & BITMASK_8));
/*
		nlri = new list<NLRIReachable>();
		snpa = new list<NLRIReachable>();
*/
		return;
	}

	// Else parse every fields specified in RFC 4760
	memcpy(&afi, ptr, sizeof(uint16_t));
	ptr += sizeof(uint16_t);
	afi = ntohs(afi);
	PRINT_DBGFL(0,"  afi = %u\n", afi);

	/* SAFI */
	safi = *ptr++;
	PRINT_DBGFL(0,"  safi = %u\n", safi);
	// SAFI not valid in MP_REACH_NLRI

	/* Next hop length and address */
	nextHopAddressLength = *ptr++;
	PRINT_DBGFL(0,"  nextHopAddressLength = %u\n", nextHopAddressLength);

	memset(&nextHopAddress, 0, sizeof(IPAddress));
	memset(&nextHopAddressLocal, 0, sizeof(IPAddress));
	if( nextHopAddressLength == 32 ) {
		/* this must be ipv6 with local address as well */
		memcpy(&nextHopAddress, ptr, sizeof(IPAddress));
		memcpy(&nextHopAddressLocal, ptr+sizeof(IPAddress), sizeof(IPAddress));
	} else if( nextHopAddressLength == 16 ) {
		/* this must be ipv6 */
		memcpy(&nextHopAddress, ptr, sizeof(IPAddress));
	} else if( nextHopAddressLength == 4 ) {
		/* this must be ipv4 */
		memcpy(&nextHopAddress, ptr, nextHopAddressLength);
	}
	ptr += nextHopAddressLength;

	/* JP: let's not worry about SNPA for now */
	uint8_t unSNPACnt = 0;
	uint8_t unSNPALen = 0;
	unSNPACnt = *ptr++;
	PRINT_DBGFL(0,"  unSNPACnt = %u\n", unSNPACnt);
	for(int ic=0; ic<unSNPACnt; ic++) {
		unSNPALen = *ptr++;
		ptr += unSNPALen;
	}

	while (ptr < (msg + len) && ptr < endMsg ) {
		uint8_t prefixLength = 0;
		prefixLength = *ptr++;
		if( ptr > endMsg ) {
			Logger::err("message truncated! cannot read mbgp prefix.");
			break;
		}
		if( prefixLength > sizeof(IPAddress)*8) { 
			Logger::err("abnormal prefix-length [%u]. skip this record.", prefixLength );
			break;
		}
		PRINT_DBGFL(0,"  prefixLength = %u\n", prefixLength);
		
		NLRIReachable route(prefixLength, ptr);
		if( route.getNumOctets()+ptr > endMsg ) { 
			Logger::err("message (mbgp a) truncated! need to read [%d], but only have [%d] bytes.",
						 route.getNumOctets(), endMsg-ptr);
			break;
		}
		ptr += route.getNumOctets();
		nlri->push_back(route);
		PRINT_DBGFL(0,"  num_octets = %u\n", route.getNumOctets());
	}
}

AttributeTypeMPReachNLRI::~AttributeTypeMPReachNLRI(void) {
	if (nlri != NULL) {
		delete nlri;
		nlri = NULL;
	}
	if (snpa != NULL) {
		delete snpa;
		snpa = NULL;
	}
}

void AttributeTypeMPReachNLRI::printMe() {
	cout << "ANNOUNCE:";
	list<NLRIReachable>::iterator iter;
	
	for(iter = nlri->begin(); iter != nlri->end(); iter++) {
		cout << endl;
		(*iter).printMe();
	}
}

void AttributeTypeMPReachNLRI::printMeCompact() {
	cout << "MBGP-NEXTHOP: ";
	if( afi == AFI_IPv4 ) {
		PRINT_IP_ADDR(nextHopAddress.ipv4);
	} else {
		PRINT_IPv6_ADDR(nextHopAddress.ipv6);
	}
	cout << "^MBGP-ANNOUNCE:";
	list<NLRIReachable>::iterator iter;
	for(iter = nlri->begin(); iter != nlri->end(); iter++) {
		cout << " ";
		if( afi == AFI_IPv4 ) {
			/* ipv4 */
			(*iter).printMeCompact();
		} else {
			/* ipv6 */
			PRINT_IPv6_ADDR((*iter).getPrefix().ipv6);
			cout << "/";
			printf("%u", (*iter).getLength());
		}
	}
}

AttributeType* AttributeTypeMPReachNLRI::clone() {
	PRINT_DBG("AttributeTypeMPReachNLRI::clone()");
	AttributeTypeMPReachNLRI *atMPR = new AttributeTypeMPReachNLRI();
	IPAddress nhaddr = getNextHopAddress();
	IPAddress nhaddrl = getNextHopAddressLocal();
	
	atMPR->setAFI(getAFI());
	atMPR->setSAFI(getSAFI());
	atMPR->setNextHopAddressLength(getNextHopAddressLength());
	atMPR->setNextHopAddress(&nhaddr);
	atMPR->setNextHopAddressLocal(&nhaddrl);
	atMPR->setCorrupt(getCorrupt());

	list<NLRIReachable>::iterator it;
	for(it = nlri->begin(); it != nlri->end(); it++) {
		atMPR->addNLRI(*it);
	}
	for(it = snpa->begin(); it != snpa->end(); it++) {
		atMPR->addSNPA(*it);
	}
	return atMPR;
	PRINT_DBG("END AttributeTypeMPReachNLRI::clone()");
}

