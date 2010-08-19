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
#include "MRTCommonHeader.h"
#include "BGPCommonHeader.h"
#include "BGPOpen.h"
#include "BGPUpdate.h"
#include "BGPKeepAlive.h"
#include "BGPNotification.h"
#include "BGPRouteRefresh.h"

BGPCommonHeader::BGPCommonHeader() {
	/* marker */
	memset(marker, 255, sizeof(marker));
	length = 0;
	setType((uint8_t)0);

	/* octets */
	octets  = NULL;
	error = 0;
}

BGPCommonHeader::BGPCommonHeader(const uint8_t* const msg) {
	setMarker(msg);
	//msg += 16;
	memcpy(&length, (msg+16), sizeof(length));
	length = ntohs(length);
	//msg += 2;
	setType((uint8_t)*(msg + 18));
	//msg++;

	/* octets */
	int len = 16+2+1+length;
	octets = NULL;
	octets  = (uint8_t*) malloc(len);
	memcpy(octets, msg, len); 
	error = 0;
}

BGPCommonHeader::~BGPCommonHeader() {
	if (octets != NULL) {
		delete [] octets;
	}
	octets = NULL;
}

// This method will indirectly update the value of msg to point 
//  to the next byte after the message.  The sub-class constructors
//  will directly update the pointer.
BGPCommonHeader* BGPCommonHeader::newMessage(uint8_t** msg, bool isAS4, uint16_t mrtLen) {
	BGPMessage* bgpMsg = NULL;
	
	BGPCommonHeader bgpHeader(*msg);
	switch(bgpHeader.getType()) {
	case BGPCommonHeader::OPEN:
		PRINT_DBG("  case BGPCommonHeader::OPEN");
		bgpMsg = new BGPOpen(msg);
		break;

	case BGPCommonHeader::UPDATE:
		PRINT_DBG("  case BGPCommonHeader::UPDATE");
		bgpMsg = new BGPUpdate(msg,isAS4,mrtLen);
		break;
		
	case BGPCommonHeader::NOTIFICATION:
		PRINT_DBG("  case BGPCommonHeader::NOTIFICAION");
		bgpMsg = new BGPNotification(msg);	
		break;
		
	case BGPCommonHeader::KEEPALIVE:
		PRINT_DBG("  case BGPCommonHeader::KEEPALIVE");
		bgpMsg = new BGPKeepAlive(msg);	
		break;
		
	case BGPCommonHeader::ROUTE_REFRESH:
		PRINT_DBG("  case BGPCommonHeader::ROUTE_REFRESH");
		bgpMsg = new BGPRouteRefresh(msg);	
		break;
	}
	return bgpMsg;
}
