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

#include "MRTTblDumpV2RibIPv6Multicast.h"
using namespace std;

#include <boost/iostreams/read.hpp>
#include <boost/iostreams/skip.hpp>
namespace io = boost::iostreams;

log4cxx::LoggerPtr MRTTblDumpV2RibIPv6Multicast::Logger = log4cxx::Logger::getLogger( "bgpparser.MRTTblDumpV2RibIPv6Multicast" );


MRTTblDumpV2RibIPv6Multicast::MRTTblDumpV2RibIPv6Multicast( MRTCommonHeader &header, std::istream &input ) :
							  MRTTblDumpV2RibHeader(header) {
	/* set AFI and SAFI */
	afi = AFI_IPv6;
	safi = SAFI_MULTICAST;

	/* copy out the sequence number, increment the pointer, and convert to host order */
	io::read( input, reinterpret_cast<char*>(&sequenceNumber), sizeof(uint32_t) );
	sequenceNumber = ntohl(sequenceNumber);

	/* copy out the prefix length and increment the pointer */
	prefixLength = input.get( );
	NLRIReachable route( prefixLength, input );
	prefix=route.getPrefix( );

	/* copy out the entry count, increment the pointer, and convert to host order */
	io::read( input, reinterpret_cast<char*>(&entryCount), sizeof(uint16_t) );
	entryCount = ntohs(entryCount);

	for( int i=0; i<entryCount; i++ )
		ribs.push_back( TblDumpV2RibEntryPtr( new TblDumpV2RibEntry( input )) );
}


MRTTblDumpV2RibIPv6Multicast::~MRTTblDumpV2RibIPv6Multicast(void) {
}

