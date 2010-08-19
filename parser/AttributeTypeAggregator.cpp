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
#include "AttributeTypeAggregator.h"

AttributeTypeAggregator::AttributeTypeAggregator(void) {
	/* nothing */
}

AttributeTypeAggregator::AttributeTypeAggregator(uint16_t len, uint8_t* msg)
						: AttributeType(len, msg) {
	uint8_t *ptr = msg;
	if (len == 6) {
		aggregatorLastAS = 0;
		memcpy(&aggregatorLastAS, ptr, sizeof(uint16_t));
		ptr += sizeof(uint16_t);
		aggregatorLastAS = ntohs(aggregatorLastAS);
	} else {
		memcpy(&aggregatorLastAS, ptr, sizeof(aggregatorLastAS));
		ptr += sizeof(uint32_t);
		aggregatorLastAS = ntohl(aggregatorLastAS);
	}
	memcpy(&bgpSpeakerIPAddress, ptr, sizeof(bgpSpeakerIPAddress.ipv4));
}

AttributeTypeAggregator::AttributeTypeAggregator(const AttributeTypeAggregator& attr) {
	this->aggregatorLastAS = attr.aggregatorLastAS;
	memcpy(&(this->bgpSpeakerIPAddress),&(attr.bgpSpeakerIPAddress),sizeof(IPAddress));
}

AttributeTypeAggregator::~AttributeTypeAggregator(void) {
	/* nothing */
}

AttributeType* AttributeTypeAggregator::clone() {
	AttributeTypeAggregator* atAgg = new AttributeTypeAggregator();
	IPAddress ipaddr = getAggregatorBGPSpeakerIPAddress();
	atAgg->setAggregatorLastAS(getAggregatorLastAS());
	atAgg->setAggregatorBGPSpeakerIPAddress(&ipaddr);
	return atAgg;
}

void AttributeTypeAggregator::printMe() { 
	cout << "AGGREGATOR: AS" << aggregatorLastAS << " "; 
	PRINT_IP_ADDR(bgpSpeakerIPAddress.ipv4);
}

void AttributeTypeAggregator::printMeCompact() { 
	cout << "AGGREGATOR: ";

	uint16_t t, b;
	t = (uint16_t)((aggregatorLastAS>>16)&0xFFFF); 
	b = (uint16_t)((aggregatorLastAS)&0xFFFF); 
	if( t == 0 ) { 
		printf("%u ",b);
	} else {
		printf("%u.%u ",t,b);
	}
	PRINT_IP_ADDR(bgpSpeakerIPAddress.ipv4);
}

