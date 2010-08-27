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

#include "BGPCommonHeader.h"

#include "BGPOpen.h"
#include "BGPUpdate.h"
#include "BGPKeepAlive.h"
#include "BGPNotification.h"
#include "BGPRouteRefresh.h"

#include <boost/iostreams/read.hpp>
#include <boost/iostreams/skip.hpp>
namespace io = boost::iostreams;

using namespace std;

log4cxx::LoggerPtr BGPCommonHeader::Logger = log4cxx::Logger::getLogger( "bgpparser.BGPCommonHeader" );

BGPCommonHeader::BGPCommonHeader( istream &input )
{
	io::read( input, reinterpret_cast<char*>(&marker), sizeof(marker) );

	io::read( input, reinterpret_cast<char*>(&length), sizeof(length) );
	length = ntohs(length);

	io::read( input, reinterpret_cast<char*>(&type), sizeof(type) );

//	/* octets */
//	int len = 16+2+1+length;
//	octets = NULL;
//	octets  = (uint8_t*) malloc(len);
//	memcpy(octets, msg, len);
//	error = 0;
}

BGPCommonHeader::~BGPCommonHeader()
{
//	if (octets != NULL) {
//		delete [] octets;
//	}
//	octets = NULL;
}

// This method will indirectly update the value of msg to point 
//  to the next byte after the message.  The sub-class constructors
//  will directly update the pointer.
BGPMessagePtr BGPCommonHeader::newMessage( istream &input, bool isAS4, uint16_t mrtLen )
{
	BGPMessagePtr bgpMsg;
	BGPCommonHeader header( input );

	switch( header.getType() )
	{
	case BGPCommonHeader::OPEN:
		LOG4CXX_INFO(Logger,"  case BGPCommonHeader::OPEN");
		bgpMsg = BGPMessagePtr( new BGPOpen(header,input) );
		break;

//	case BGPCommonHeader::UPDATE:
//		LOG4CXX_INFO(Logger,"  case BGPCommonHeader::UPDATE");
//		bgpMsg = BGPMessagePtr( new BGPUpdate(header,msg,isAS4,mrtLen) );
//		break;
//
//	case BGPCommonHeader::NOTIFICATION:
//		LOG4CXX_INFO(Logger,"  case BGPCommonHeader::NOTIFICAION");
//		bgpMsg = BGPMessagePtr( new BGPNotification(header,msg) );
//		break;
//
//	case BGPCommonHeader::KEEPALIVE:
//		LOG4CXX_INFO(Logger,"  case BGPCommonHeader::KEEPALIVE");
//		bgpMsg = BGPMessagePtr( new BGPKeepAlive(header,msg) );
//		break;
//
//	case BGPCommonHeader::ROUTE_REFRESH:
//		LOG4CXX_INFO(Logger,"  case BGPCommonHeader::ROUTE_REFRESH");
//		bgpMsg = BGPMessagePtr( new BGPRouteRefresh(header,msg) );
//		break;
	}
	return bgpMsg;
}
