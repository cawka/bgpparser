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
#include "AttributeTypeExtCommunities.h"

AttributeTypeExtCommunities::AttributeTypeExtCommunities(void) {
	extCommunityValues = new list<ExtCommunityValue>();
}

AttributeTypeExtCommunities::AttributeTypeExtCommunities(uint16_t len, uint8_t* msg)
							: AttributeType(len, msg) {
	PRINT_DBG("AttributeTypeExtCommunities::AttributeTypeExtCommunities()");
	extCommunityValues = new list<ExtCommunityValue>();
	uint8_t* ptr = msg;
	
	while (ptr < msg + len) {
		ExtCommunityValue ExtCommunity;
		uint16_t unTypeHigh, unTypeLow;

		memcpy(&unTypeHigh, ptr, sizeof(uint16_t));
		ptr += sizeof(uint16_t);
		unTypeHigh = ntohs(unTypeHigh);

		memcpy(&unTypeLow, ptr, sizeof(uint16_t));
		ptr += sizeof(uint16_t);
		unTypeLow = ntohs(unTypeLow);

		ExtCommunity.typeHigh = unTypeHigh;
		ExtCommunity.typeLow = unTypeLow;

		memcpy(ExtCommunity.rchValue, ptr, sizeof(ExtCommunity.rchValue));
		ptr += sizeof(ExtCommunity.rchValue);
		
		extCommunityValues->push_back(ExtCommunity);
	}
}

AttributeTypeExtCommunities::AttributeTypeExtCommunities(const AttributeTypeExtCommunities& attr) {
	extCommunityValues = new list<ExtCommunityValue>(extCommunityValues->begin(),extCommunityValues->end());
}

AttributeTypeExtCommunities::~AttributeTypeExtCommunities(void) {
	delete extCommunityValues;
}

void AttributeTypeExtCommunities::printMe() {
	cout << "EXTENDED_COMMUNITIES:";
	list<ExtCommunityValue>::iterator iter;

	for(iter = extCommunityValues->begin(); iter != extCommunityValues->end(); iter++) {
		cout << " " << (*iter).typeHigh << ":" << (*iter).typeLow << ":"; 
		printf("%02x%02x%02x%02x%02x%02x", 
					(*iter).rchValue[0], (*iter).rchValue[1], (*iter).rchValue[2],
					(*iter).rchValue[3], (*iter).rchValue[4], (*iter).rchValue[5] );
	}
}

void AttributeTypeExtCommunities::printMeCompact() {
	cout << "EXTENDED_COMMUNITIES: ";
	list<ExtCommunityValue>::iterator iter;
	bool isFirstLoop = true;
	for(iter = extCommunityValues->begin(); iter != extCommunityValues->end(); iter++) {
		cout << (isFirstLoop ? "" : " ") << (*iter).typeHigh << ":" << (*iter).typeLow << ":";
		printf("%02x%02x%02x%02x%02x%02x", 
					(*iter).rchValue[0], (*iter).rchValue[1], (*iter).rchValue[2],
					(*iter).rchValue[3], (*iter).rchValue[4], (*iter).rchValue[5] );
		isFirstLoop = false;
	}
}

AttributeType* AttributeTypeExtCommunities::clone() {
	AttributeTypeExtCommunities *atExtComm = new AttributeTypeExtCommunities();
	list<ExtCommunityValue>::iterator it;
	for(it = extCommunityValues->begin(); it != extCommunityValues->end(); it++) {
		atExtComm->setExtCommunityValue(*it);
	}
	return atExtComm;
}
