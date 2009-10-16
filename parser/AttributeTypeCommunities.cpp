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
#include "AttributeTypeCommunities.h"

AttributeTypeCommunities::AttributeTypeCommunities(void) {
	communityValues = new list<CommunityValue>();
}

AttributeTypeCommunities::AttributeTypeCommunities(const AttributeTypeCommunities& attr) {
	this->communityValues = new list<CommunityValue>(attr.communityValues->begin(),attr.communityValues->end());
}

AttributeTypeCommunities::AttributeTypeCommunities(uint16_t len, uint8_t* msg)
						 : AttributeType(len, msg) {
	communityValues = new list<CommunityValue>();
	uint8_t* ptr = msg;
	
	while (ptr < msg + len) {
		CommunityValue community;
		uint16_t asNum, commInfo;

		memcpy(&asNum, ptr, sizeof(uint16_t));
		ptr += sizeof(uint16_t);
		asNum = ntohs(asNum);

		memcpy(&commInfo, ptr, sizeof(uint16_t));
		ptr += sizeof(uint16_t);
		commInfo = ntohs(commInfo);

		community.ASnum = asNum;
		community.info = commInfo;
		
		communityValues->push_back(community);
	}
}

AttributeTypeCommunities::~AttributeTypeCommunities(void) {
	delete communityValues;
}

void AttributeTypeCommunities::printMe() {
	cout << "COMMUNITIES:";
	list<CommunityValue>::iterator iter;

	for(iter = communityValues->begin(); iter != communityValues->end(); iter++) {
		cout << " " << (*iter).ASnum << ":" << (*iter).info;
	}
}

void AttributeTypeCommunities::printMeCompact() {
	cout << "COMMUNITIES: ";
	list<CommunityValue>::iterator iter;
	bool isFirstLoop = true;
	for(iter = communityValues->begin(); iter != communityValues->end(); iter++) {
		cout << (isFirstLoop ? "" : " ");
		uint32_t combVal = (uint32_t)((*iter).ASnum << 16) | (*iter).info;
		switch(combVal) {
			case COMMUNITY_NO_EXPORT: {
				cout << "no-export";
				break;
			}
			case COMMUNITY_NO_ADVERTISE: {
				cout << "no-advertise";
				break;
			}
			case COMMUNITY_LOCAL_AS: {
				cout << "local-AS";
				break;
			}
			default: {
				cout << (*iter).ASnum << ":" << (*iter).info;
			}
		}
		isFirstLoop = false;
	}
}

AttributeType* AttributeTypeCommunities::clone() {
	AttributeTypeCommunities *atComm = new AttributeTypeCommunities();
	list<CommunityValue>::iterator it;
	for(it = communityValues->begin(); it != communityValues->end(); it++) {
		atComm->setCommunityValue(*it);
	}
	return atComm;
}


