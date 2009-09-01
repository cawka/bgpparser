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

// Author: Jason Ryder, Paul Wang
// Modified: Jonathan Park (jpark@cs.ucla.edu)
#include "AttributeTypeNextHop.h"

AttributeTypeNextHop::AttributeTypeNextHop(void) {
	/* nothing */
}

AttributeTypeNextHop::AttributeTypeNextHop(const AttributeTypeNextHop& attrNextHop)
					 : AttributeType(attrNextHop) {
	memcpy(&nextHop, &attrNextHop.nextHop, sizeof(IPAddress));
}

AttributeTypeNextHop::AttributeTypeNextHop(uint16_t len, uint8_t* msg)
					 : AttributeType(len, msg) {
	PRINT_DBG("AttributeTypeNextHop::AttributeTypeNextHop()");
	if (len > 0) {
		if (len == 4) {
			afi = AFI_IPv4;
		} else {
			afi = AFI_IPv6;
		}
		memcpy(&nextHop, msg, sizeof(IPAddress));
	} else {
		memset(&nextHop, 0, sizeof(IPAddress));
	}
}

AttributeTypeNextHop::~AttributeTypeNextHop(void) {
	/* nothing */
}

void AttributeTypeNextHop::printMeCompact() {
	cout << "NEXT_HOP: ";
	switch(afi) {
		case AFI_IPv4: PRINT_IP_ADDR(nextHop.ipv4); break;
		case AFI_IPv6: PRINT_IPv6_ADDR(nextHop.ipv6); break;
	}
}

void AttributeTypeNextHop::printMe() {
	cout << "NEXT_HOP: ";
	switch(afi) {
		case AFI_IPv4: PRINT_IP_ADDR(nextHop.ipv4); break;
		case AFI_IPv6: PRINT_IPv6_ADDR(nextHop.ipv6); break;
	}
}

AttributeType* AttributeTypeNextHop::clone() {
	IPAddress ipaddr;
	AttributeTypeNextHop *patNH = new AttributeTypeNextHop();
	patNH->setNextHopAFI(getNextHopAFI());
	ipaddr = getNextHopIPAddress();
	patNH->setNextHopIPAddress(&ipaddr);
	return patNH;
}
