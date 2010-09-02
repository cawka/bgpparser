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

#include "MRTTblDumpV2PeerIndexTbl.h"
#include "Exceptions.h"
using namespace std;

#include <boost/iostreams/read.hpp>
#include <boost/iostreams/skip.hpp>
namespace io = boost::iostreams;

log4cxx::LoggerPtr MRTTblDumpV2PeerIndexTbl::Logger = log4cxx::Logger::getLogger( "bgpparser.MRTTblDumpV2PeerIndexTbl" );

MRTTblDumpV2PeerIndexTbl::MRTTblDumpV2PeerIndexTbl( MRTCommonHeader &header, std::istream &input )
						 : MRTCommonHeader( header )
{
	LOG4CXX_TRACE(Logger,"");

	bool error=false;	

	/* copy out the collector BGP id, increment the pointer and convert to host order */
	error|= sizeof(uint32_t)!=
			io::read( input, reinterpret_cast<char*>(&collectorBGPId), sizeof(uint32_t) );
	collectorBGPId = ntohl(collectorBGPId);

	/* copy out the view name length, increment the pointer and convert to host order */
	error|= sizeof(uint16_t)!=
			io::read( input, reinterpret_cast<char*>(&viewNameLength), sizeof(uint16_t) );
	viewNameLength = ntohs(viewNameLength);

	ostringstream os;
	for( int i=0; i<viewNameLength; i++ ) os << (char)input.get();
	viewName=os.str( );

	/* copy out the peer count, increment the pointer and convert to host order */
	error|= sizeof(uint16_t)!=
			io::read( input, reinterpret_cast<char*>(&peerCount), sizeof(uint16_t) );
	peerCount = ntohs(peerCount);
	
	if( error )
	{
		LOG4CXX_ERROR( Logger, "Parsing error" );
		throw BGPError( );
	}

	/* parse out the peer entries */
	for (int i = 0; i < peerCount; i++)
	{
		MRTTblDumpV2PeerIndexTblPeerEntryPtr entry( new MRTTblDumpV2PeerIndexTblPeerEntry() );

		uint8_t peerType;
		error|= sizeof(uint8_t)!=
				io::read( input, reinterpret_cast<char*>(&peerType), sizeof(uint8_t) );

		if ((peerType & 0x01) == 0)
		{
			/* IPv4 peer IP address */
			entry->IPType = AFI_IPv4;
		} else {
			/* IPv6 peer IP address */
			entry->IPType = AFI_IPv6;
		}
		if ((peerType & 0x02) == 0) {
			/* IPv4 peer IP address */
			entry->isAS4 = false;
		} else {
			/* IPv6 peer IP address */
			entry->isAS4 = true;
		}

		/* copy the peer BGP id, convert to host order */
		error|= sizeof(uint32_t)!=
				io::read( input, reinterpret_cast<char*>(&entry->peerBGPId), sizeof(uint32_t) );
		entry->peerBGPId = ntohl(entry->peerBGPId);

		/* copy the peer IP address and increment the pointer */
		if( entry->IPType == AFI_IPv4 )
		{
			error|= sizeof(entry->peerIP.ipv4)!=
					io::read( input, reinterpret_cast<char*>(&entry->peerIP.ipv4), sizeof(entry->peerIP.ipv4) );
		}
		else
		{
			error|= sizeof(entry->peerIP.ipv6)!=
					io::read( input, reinterpret_cast<char*>(&entry->peerIP.ipv6), sizeof(entry->peerIP.ipv6) );
		}

		/* copy the peer AS and increment the pointer */
		if( !entry->isAS4)
		{
			/* 16 bit peer AS */
			uint16_t peerAS;
			error|= sizeof(uint16_t)!=
					io::read( input, reinterpret_cast<char*>(&peerAS), sizeof(uint16_t) );
			entry->peerAS = ntohs(peerAS);
		}
		else {
			/* 32 bit peer AS */
			error|= sizeof(uint32_t)!=
					io::read( input, reinterpret_cast<char*>(&entry->peerAS), sizeof(uint32_t) );
			entry->peerAS = ntohl(entry->peerAS);
		}

		if( error )
		{
			LOG4CXX_ERROR( Logger, "Entry parsing error" );
			throw BGPError( );
		}

		/* add MRTTblDumpV2PeerIndexTblPeerEntry to list */
		peerEntries.push_back( entry );
	}

	LOG4CXX_TRACE(Logger,"entries = " << peerEntries.size() );
}


MRTTblDumpV2PeerIndexTbl::~MRTTblDumpV2PeerIndexTbl(void) {
}

const MRTTblDumpV2PeerIndexTblPeerEntryPtr MRTTblDumpV2PeerIndexTbl::getPeer( uint16_t peerIndex )
{
	if( peerIndex>peerEntries.size() ) throw BGPTextError( "Corrupted PeerIndexTbl" );
	return peerEntries[peerIndex];
}


uint32_t MRTTblDumpV2PeerIndexTbl::getCollectorBGPId(void) const {
	return collectorBGPId;
}

uint16_t MRTTblDumpV2PeerIndexTbl::getViewNameLength(void) const {
	return viewNameLength;
}

uint16_t MRTTblDumpV2PeerIndexTbl::getPeerCount(void) const {
	return peerCount;
}

