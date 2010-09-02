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

#include "MRTBgp4MPMessage.h"
#include "BGPCommonHeader.h"
#include "BGPUpdate.h"
#include "BGPNotification.h"
#include "BGPKeepAlive.h"
#include "BGPOpen.h"
#include "BGPRouteRefresh.h"
#include "Exceptions.h"

using namespace std;

#include <boost/iostreams/read.hpp>
namespace io = boost::iostreams;

log4cxx::LoggerPtr MRTBgp4MPMessage::Logger = log4cxx::Logger::getLogger( "bgpparser.MRTBgp4MPMessage" );

MRTBgp4MPMessage::MRTBgp4MPMessage( MRTCommonHeader &header, istream &input )
: MRTCommonHeader( header )
{
	LOG4CXX_TRACE( Logger, "" );

	MRTBgp4MPMessagePacket pkt;
	bool error= sizeof(MRTBgp4MPMessagePacket)!=
				io::read( input, reinterpret_cast<char*>(&pkt), sizeof(MRTBgp4MPMessagePacket) );

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
	payload = BGPCommonHeader::newMessage( input, false );
}

void MRTBgp4MPMessage::processIPs( istream &input )
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

MRTBgp4MPMessage::~MRTBgp4MPMessage(void)
{
}

uint32_t MRTBgp4MPMessage::getPeerAS(void) const {
	return peerAS;
}

uint32_t MRTBgp4MPMessage::getLocalAS(void) const {
	return localAS;
}

uint16_t MRTBgp4MPMessage::getInterfaceIndex(void) const {
	return interfaceIndex;
}

uint16_t MRTBgp4MPMessage::getAddressFamily(void) const {
	return addressFamily;
}

IPAddress MRTBgp4MPMessage::getPeerIP(void) const {
	return peerIP;
}

IPAddress MRTBgp4MPMessage::getLocalIP(void) const {
	return localIP;
}
