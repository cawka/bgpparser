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

#include "MRTBgp4MPEntry.h"

MRTBgp4MPEntry::MRTBgp4MPEntry(void) {
	/* nothing */
}

MRTBgp4MPEntry::MRTBgp4MPEntry(uint8_t **ptr) : MRTCommonHeader((const uint8_t **)ptr) {
	uint8_t *p;
	uint8_t prefixLength;
	int bytes;
	MRTBgp4MPEntryPacket pkt;

	/* add sizeof(MRTCommonHeaderPacket) to ptr since ptr points to base of message */
	p = const_cast<uint8_t *>(*ptr) + sizeof(MRTCommonHeaderPacket);

	memcpy(&pkt, p, sizeof(MRTBgp4MPEntryPacket));

	viewNumber = ntohs(pkt.viewNumber);
	status = ntohs(pkt.status);
	timeLastChange = ntohl(pkt.timeLastChange);
	addressFamily = ntohs(pkt.addressFamily);
	safi = pkt.safi;

	/* what does this field define */
	//pkt.nextHopLength;

	/* move pointer p to beginning of variable length next hop address field */
	p += sizeof(MRTBgp4MPEntryPacket);

	/* how to extract out next hop address based on next hop length? */

	/* move pointer p to beginning of prefix length field assuming next hop len field is in bytes */
	p += pkt.nextHopLength;

	/* set prefix length and increment pointer p to beginning of variable length address prefix field */
	prefixLength = *p++;

	bytes = ((uint32_t)prefixLength + ((uint32_t)prefixLength % sizeof(uint8_t))) % sizeof(uint8_t);

	/* TODO: this message format has been deprecated and is no longer supported */
	
	/* TODO: increment the pointer to the new location in the file stream */
	*ptr = p;
}

MRTBgp4MPEntry::~MRTBgp4MPEntry(void) {
	/* nothing */
}
