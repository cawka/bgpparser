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

#include "MRTTblDumpV2RibGeneric.h"

log4cxx::LoggerPtr MRTTblDumpV2RibGeneric::Logger = log4cxx::Logger::getLogger( "bgpparser.MRTTblDumpV2RibGeneric" );

MRTTblDumpV2RibGeneric::MRTTblDumpV2RibGeneric(void) {
	/* nothing */
}


MRTTblDumpV2RibGeneric::MRTTblDumpV2RibGeneric(uint8_t **ptr) 
					   : MRTTblDumpV2RibHeader((const uint8_t **)ptr) {
	uint8_t *p;
	uint16_t addressFamily;
	uint8_t subsequentAddressFamily;

	/* add sizeof(MRTCommonHeaderPacket) to ptr since ptr points to base of message */
	p = const_cast<uint8_t *>(*ptr) + sizeof(MRTCommonHeaderPacket);

	/* copy out the sequence number, increment the pointer, and convert to host order */
	memcpy(&sequenceNumber, p, sizeof(uint32_t));
	p += sizeof(uint32_t);
	sequenceNumber = ntohl(sequenceNumber);

	/* copy out the AFI, increment the pointer, and convert to host order */
	memcpy(&addressFamily, p, sizeof(uint16_t));
	p += sizeof(uint16_t);
	addressFamily = ntohs(addressFamily);

	if (addressFamily == AFI_IPv4) {
		afi = AFI_IPv4;
	} else if (addressFamily == AFI_IPv6) {
		afi = AFI_IPv6;
	} else {
		LOG4CXX_ERROR( Logger,"rib generic has unknown address family ["<< addressFamily <<"]" );
	}

	/* copy out the SAFI, increment the pointer */
	subsequentAddressFamily = *p++;

	if (subsequentAddressFamily == SAFI_UNICAST) {
		safi = (uint16_t) subsequentAddressFamily;
	} else if (subsequentAddressFamily == SAFI_MULTICAST) {
		safi = (uint16_t) subsequentAddressFamily;
	} else {
		LOG4CXX_ERROR( Logger,"rib generic has unknown subsequent address family ["<< subsequentAddressFamily <<"]" );
	}

	prefixLength = *p++;

	memset(&prefix, 0, sizeof(IPAddress));
	memcpy(&prefix, p, ((uint32_t)prefixLength + 7) / 8);
	p += (((uint32_t)prefixLength + 7) / 8);

	/* copy out the entry count, increment the pointer, and convert to host order */
	memcpy(&entryCount, p, sizeof(uint16_t));
	p += sizeof(uint16_t);
	entryCount = ntohs(entryCount);

	/* TODO: parse each RIB entry... */
	MRTTblDumpV2RibHeader::parseRibEntry(ribs, entryCount, &p);

	/* TODO: increment the pointer to the new location in the file stream */
	*ptr = p;
}

MRTTblDumpV2RibGeneric::~MRTTblDumpV2RibGeneric(void) {
	/* nothing */
}
