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

#include "MRTBgp4MPStateChange.h"

using namespace std;
namespace io = boost::iostreams;

log4cxx::LoggerPtr MRTBgp4MPStateChange::Logger = log4cxx::Logger::getLogger( "bgpparser.MRTBgp4MPStateChange" );

MRTBgp4MPStateChange::MRTBgp4MPStateChange( MRTCommonHeader &header, istream &input )
: MRTCommonHeader( header )
{
	MRTBgp4MPStateChangePacket pkt;
	bool error= sizeof(MRTBgp4MPStateChangePacket)!=
				io::read( input, reinterpret_cast<char*>(&pkt), sizeof(MRTBgp4MPStateChangePacket) );

	if( error )
	{
		LOG4CXX_ERROR( Logger, "Parsing error" );
		throw BGPError( );
	}

	peerAS = ntohs(pkt.peerAS);
	localAS = ntohs(pkt.localAS);
	interfaceIndex = ntohs(pkt.interfaceIndex);
	addressFamily = ntohs(pkt.addressFamily);

	processIPs( input );
	processStates( input );
}

void MRTBgp4MPStateChange::processIPs( istream &input )
{
	size_t len=0;
	if( addressFamily == AFI_IPv4 )
		len=sizeof(peerIP.ipv4);
	else if( addressFamily == AFI_IPv6 )
		len=sizeof(peerIP.ipv6);
	else
		LOG4CXX_ERROR(Logger,"unsupported address family ["<< (int)addressFamily <<"]" );

	bool error=false;
	error|= len!=
			io::read( input, reinterpret_cast<char*>(&peerIP),  len );
	error|= len!=
			io::read( input, reinterpret_cast<char*>(&localIP), len );

	if( error )
	{
		LOG4CXX_ERROR( Logger, "Parsing error" );
		throw BGPError( );
	}
}

void MRTBgp4MPStateChange::processStates( istream &input )
{
	bool error=false;

	error|= sizeof(uint16_t)!=
			io::read( input, reinterpret_cast<char*>(&oldState),  sizeof(uint16_t) );
	oldState = ntohs(oldState);

	error|= sizeof(uint16_t)!=
			io::read( input, reinterpret_cast<char*>(&newState),  sizeof(uint16_t) );
	newState = ntohs(newState);

	if( error )
	{
		LOG4CXX_ERROR( Logger, "Parsing error" );
		throw BGPError( );
	}
}

MRTBgp4MPStateChange::~MRTBgp4MPStateChange(void) {
	/* nothing */
}


void MRTBgp4MPStateChange::printMe() {
	cout << "PEER_IP: ";
	switch(addressFamily) {
		case AFI_IPv4: PRINT_IP_ADDR(peerIP.ipv4); break;
		case AFI_IPv6: PRINT_IPv6_ADDR(peerIP.ipv6); break;
	}
	cout << endl;
	cout << "PEER_ASN: " << peerAS << endl;
	cout << "OLD_STATE: " << oldState << endl;
	cout << "NEW_STATE: " << newState << endl;
}

void MRTBgp4MPStateChange::printMeCompact() {
	uint16_t top, bottom;
	top = (peerAS>>16)&0xFFFF;
	bottom = (peerAS)&0XFFFF;
	switch(addressFamily) {
		case AFI_IPv4: PRINT_IP_ADDR(peerIP.ipv4); break;
		case AFI_IPv6: PRINT_IPv6_ADDR(peerIP.ipv6); break;
	}
	if( top > 0 ) {
		cout << "|" << top << "." << bottom << "|" << oldState << "|" << newState;
	} else {
		cout << "|" << bottom << "|" << oldState << "|" << newState;
	}
}
