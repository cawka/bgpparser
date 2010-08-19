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

// Author: Jonathan Park (jpark@cs.ucla.edu)
#include "BGPOpen.h"

BGPOpen::BGPOpen(uint8_t** msg)
		: BGPCommonHeader(*msg) {
	PRINT_DBG("BGPOpen::BGPOpen()");
	*msg += MESSAGE_HEADER_SIZE;

	memcpy(&version,*msg,sizeof(uint8_t)); *msg += sizeof(uint8_t);
	memcpy(&myAS,*msg,sizeof(uint16_t)); *msg += sizeof(uint16_t);
	myAS = ntohs(myAS);
	memcpy(&holdTime,*msg,sizeof(uint16_t)); *msg += sizeof(uint16_t);
	holdTime = ntohs(holdTime);
	memcpy(&bgpId,*msg,sizeof(uint32_t)); *msg += sizeof(uint32_t);
	bgpId = ntohl(bgpId);
	memcpy(&optParmLen,*msg,sizeof(uint8_t)); *msg += sizeof(uint8_t);
	*msg += optParmLen;
}

BGPOpen::~BGPOpen() { 
	/* nothing */
}

void BGPOpen::printMe() {
	/* nothing */
}

void BGPOpen::printMeCompact() {
	/* nothing */
}
