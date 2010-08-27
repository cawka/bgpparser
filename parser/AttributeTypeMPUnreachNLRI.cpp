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

#include "AttributeTypeMPUnreachNLRI.h"

using namespace std;

log4cxx::LoggerPtr AttributeTypeMPUnreachNLRI::Logger = log4cxx::Logger::getLogger( "bgpparser.AttributeTypeMPUnreachNLRI" );

AttributeTypeMPUnreachNLRI::AttributeTypeMPUnreachNLRI(void) {
	nlri = new list<NLRIUnReachable>();
	corrupt = 0;
}

AttributeTypeMPUnreachNLRI::AttributeTypeMPUnreachNLRI(const AttributeTypeMPUnreachNLRI &attrMPUnreachNLRI) 
						   : AttributeType(attrMPUnreachNLRI) {
	afi = attrMPUnreachNLRI.afi;
	safi = attrMPUnreachNLRI.safi;
	corrupt = attrMPUnreachNLRI.corrupt;

	nlri = new list<NLRIUnReachable>(attrMPUnreachNLRI.nlri->begin(),attrMPUnreachNLRI.nlri->end());
}

AttributeTypeMPUnreachNLRI::AttributeTypeMPUnreachNLRI(uint16_t len, uint8_t* msg) 
						   : AttributeType(len, msg) {
	uint8_t *ptr = msg;
	uint8_t *endMsg = AttributeType::getEndMsg();

	nlri = new list<NLRIUnReachable>();
	corrupt = 0;
	memcpy(&afi, ptr, sizeof(uint16_t));
	ptr += sizeof(uint16_t);
	afi = ntohs(afi);

	if ((afi != AFI_IPv4) &&  (afi != AFI_IPv6)) {
		LOG4CXX_ERROR(Logger,"unknown address family " << afi );
		corrupt = 1;
		return;
	}

	safi = *ptr++;
	if ((((uint32_t)safi & BITMASK_8) != SAFI_UNICAST) && (((uint32_t)safi & BITMASK_8) != SAFI_MULTICAST)) {
		LOG4CXX_ERROR(Logger,"unknown subsequent address family " << (uint32_t)safi );
	}

	while (ptr < (msg + len) && ptr < endMsg ) {
		uint8_t prefixLength = BITMASK_8 & (*ptr);
		ptr++;
		if( ptr > endMsg ) {
			Logger->error( "message truncated! cannot read mbgp prefix.");
			break;
		}
		if( prefixLength > sizeof(IPAddress)*8) { 
			LOG4CXX_ERROR(Logger,"abnormal prefix-length ["<< prefixLength <<"]. skip this record." );
			break;
		}
		NLRIUnReachable route(prefixLength, ptr);
		if( route.getNumOctets()+ptr > endMsg ) { 
			LOG4CXX_ERROR(Logger,"message (mbgp w) truncated! need to read ["<< route.getNumOctets()
					<<"], but only have ["<< (int)(endMsg-ptr) <<"] bytes." );
			break;
		}
		nlri->push_back(route);
		ptr += route.getNumOctets();
	}
}

AttributeTypeMPUnreachNLRI::~AttributeTypeMPUnreachNLRI(void) {
	if (nlri != NULL) {
		delete nlri;
	}
	nlri = NULL;
}

AttributeType* AttributeTypeMPUnreachNLRI::clone() {
	AttributeTypeMPUnreachNLRI *atMPU = new AttributeTypeMPUnreachNLRI();
	
	atMPU->setAFI(getAFI());
	atMPU->setSAFI(getSAFI());
	atMPU->setCorrupt(getCorrupt());

	list<NLRIUnReachable>::iterator it;
	for(it = nlri->begin(); it != nlri->end(); it++) {
		atMPU->addNLRI(*it);
	}
	return atMPU;
}

void AttributeTypeMPUnreachNLRI::printMe() {
	cout << "MBGP-WITHDRAWN: ";
	list<NLRIUnReachable>::iterator iter;
	
	for(iter = nlri->begin(); iter != nlri->end(); iter++) {
		cout << endl;
		(*iter).printMe();
	}
}

void AttributeTypeMPUnreachNLRI::printMeCompact()
{
	cout << "MBGP-WITHDRAWN:";
	list<NLRIUnReachable>::iterator iter;
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
