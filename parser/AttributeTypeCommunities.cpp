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
#include <bgpparser.h>

#include "AttributeTypeCommunities.h"

using namespace std;
namespace io = boost::iostreams;

log4cxx::LoggerPtr AttributeTypeCommunities::Logger =
  log4cxx::Logger::getLogger("bgpparser.AttributeTypeCommunities");

AttributeTypeCommunities::AttributeTypeCommunities(AttributeType& header, std::istream& input)
  : AttributeType(header)
{
  LOG4CXX_TRACE(Logger, "");
  while (input.peek() != -1) {
    CommunityValue community;
    int len = io::read(input, reinterpret_cast<char*>(&community), sizeof(CommunityValue));
    if (len != sizeof(CommunityValue)) {
      LOG4CXX_ERROR(Logger, "Requested " << (int)sizeof(CommunityValue) << " bytes, got " << len
                                         << " bytes");
      throw BGPError();
    }

    community.ASnum = ntohs(community.ASnum);
    community.info = ntohs(community.info);

    communityValues.push_back(community);
  }
  LOG4CXX_TRACE(Logger, "count = " << communityValues.size());
}

AttributeTypeCommunities::~AttributeTypeCommunities(void)
{
}

// void AttributeTypeCommunities::printMe() {
//	cout << "COMMUNITIES:";
//	list<CommunityValue>::iterator iter;
//
//	for(iter = communityValues.begin(); iter != communityValues.end(); iter++) {
//		cout << " " << (*iter).ASnum << ":" << (*iter).info;
//	}
//}
//
// void AttributeTypeCommunities::printMeCompact() {
//	cout << "COMMUNITIES: ";
//	list<CommunityValue>::iterator iter;
//	bool isFirstLoop = true;
//	for(iter = communityValues.begin(); iter != communityValues.end(); iter++) {
//		cout << (isFirstLoop ? "" : " ");
//		uint32_t combVal = (uint32_t)((*iter).ASnum << 16) | (*iter).info;
//		switch(combVal) {
//			case COMMUNITY_NO_EXPORT: {
//				cout << "no-export";
//				break;
//			}
//			case COMMUNITY_NO_ADVERTISE: {
//				cout << "no-advertise";
//				break;
//			}
//			case COMMUNITY_LOCAL_AS: {
//				cout << "local-AS";
//				break;
//			}
//			default: {
//				cout << (*iter).ASnum << ":" << (*iter).info;
//			}
//		}
//		isFirstLoop = false;
//	}
//}
