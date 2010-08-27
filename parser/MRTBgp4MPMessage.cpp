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

#include <boost/iostreams/read.hpp>
namespace io = boost::iostreams;

log4cxx::LoggerPtr MRTBgp4MPMessage::Logger = log4cxx::Logger::getLogger( "bgpparser.MRTBgp4MPMessage" );

MRTBgp4MPMessage::MRTBgp4MPMessage(void)
{
	/* nothing */
}

MRTBgp4MPMessage::MRTBgp4MPMessage( MRTCommonHeader &header, uint8_t **ptr )
: MRTCommonHeader( header )
{
	MRTBgp4MPMessagePacket pkt;
	io::read( input, reinterpret_cast<char*>(&pkt), sizeof(MRTBgp4MPMessagePacket) );

	peerAS = ntohs(pkt.peerAS);
	localAS = ntohs(pkt.localAS);
	interfaceIndex = ntohs(pkt.interfaceIndex);
	addressFamily = ntohs(pkt.addressFamily);

	processIPs( input );
	processMessage( input, false );
}

void MRTBgp4MPMessage::processIPs( istream &input )
{
	size_t len=0;
	if( addressFamily == AFI_IPv4 )
		len=sizeof(peerIP.ipv4);
	else if( addressFamily == AFI_IPv6 )
		len=sizeof(peerIP.ipv6);
	else
		LOG4CXX_ERROR(Logger,"unsupported address family ["<< addressFamily <<"]" );

	io::read( input, reinterpret_cast<char*>(&peerIP),  len );
	io::read( input, reinterpret_cast<char*>(&localIP), len );
}

void MRTBgp4MPMessage::processMessage( std::istream &input, bool isAS4 )
{
	BGPMessage* bgpMsg = BGPCommonHeader::newMessage( (uint8_t**)&p, isAS4,
			getLength( ) - nBytesRead );
	if( bgpMsg )
	{
		LOG4CXX_TRACE(Logger,"bgpMsg.getType() = " << bgpMsg->getType());
		switch( bgpMsg->getType( ) )
		{
		case BGPCommonHeader::OPEN:
		{
			LOG4CXX_TRACE(Logger,"case BGPCommonHeader::OPEN");
			BGPOpen* bgpOpen = (BGPOpen*)bgpMsg;
			payload = BGPMessagePtr( bgpOpen );
			break;
		}
		case BGPCommonHeader::UPDATE:
		{
			BGPUpdate* bgpUpdate = (BGPUpdate*)bgpMsg;
			LOG4CXX_TRACE(Logger,"  case BGPCommonHeader::UPDATE");
			LOG4CXX_TRACE(Logger,"  bgpUpdate->length = " << bgpUpdate->getLength());
			LOG4CXX_TRACE(Logger,"  bgpUpdate->withdrawnRoutesLength = " << bgpUpdate->getWithdrawnRoutesLength());
			LOG4CXX_TRACE(Logger,"  bgpUpdate->pathAttributesLength = " << bgpUpdate->getPathAttributesLength());
			LOG4CXX_TRACE(Logger,"  bgpUpdate->nlriLength = " << bgpUpdate->getNlriLength());
			if( bgpUpdate->getWithdrawnRoutesLength( ) == 0
					&& bgpUpdate->getPathAttributesLength( ) == 0
					&& bgpUpdate->getNlriLength( ) == 0 )
			{
				delete bgpUpdate;
				payload.reset( );
			}
			else
			{
				payload = BGPMessagePtr( bgpUpdate );
			}
			break;
		}
		case BGPCommonHeader::NOTIFICATION:
		{
			BGPNotification* bgpNotification = (BGPNotification*)bgpMsg;
			payload = BGPMessagePtr( bgpNotification );
			break;
		}
		case BGPCommonHeader::KEEPALIVE:
		{
			BGPKeepAlive* bgpKeepAlive = (BGPKeepAlive*)bgpMsg;
			payload = BGPMessagePtr( bgpKeepAlive );
			break;
		}
		case BGPCommonHeader::ROUTE_REFRESH:
		{
			BGPRouteRefresh* bgpRouteRefresh = (BGPRouteRefresh*)bgpMsg;
			payload = BGPMessagePtr( bgpRouteRefresh );
			break;
		}
		}
	}
	else
	{
		Logger->error( "bgp message is NULL." );
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
