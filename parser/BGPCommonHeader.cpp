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
#include "Exceptions.h"

#include "BGPOpen.h"
#include "BGPUpdate.h"
#include "BGPKeepAlive.h"
#include "BGPNotification.h"
#include "BGPRouteRefresh.h"

#include <boost/iostreams/read.hpp>
#include <boost/iostreams/skip.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
namespace io = boost::iostreams;

using namespace std;

log4cxx::LoggerPtr BGPCommonHeader::Logger = log4cxx::Logger::getLogger( "bgpparser.BGPCommonHeader" );

/*protected*/BGPCommonHeader::BGPCommonHeader( uint8_t _type )
{
	memset( reinterpret_cast<char*>(&marker), 255, sizeof(marker) );
	length=19; //only headers
	type=_type;
}

BGPCommonHeader::BGPCommonHeader( istream &input )
{
	LOG4CXX_TRACE(Logger,"");
	io::read( input, reinterpret_cast<char*>(&marker), sizeof(marker) );

	io::read( input, reinterpret_cast<char*>(&length), sizeof(length) );
	length = ntohs(length);

	type=input.get( );

	uint32_t msg_length=length-/*marker*/16-/*length*/2-/*type*/1;
	if( msg_length==0 ) return; //there is no need to do anything

	data=boost::shared_ptr<char>( new char[msg_length] );

	int read=io::read( input, data.get(), msg_length );
	if( read==-1 || read!=msg_length ) 
	{
		LOG4CXX_ERROR(Logger,msg_length << " bytes was requested, read only " << read << " bytes");

		if( read==-1 ) throw BGPError( ); //there is nothing else to do

		//otherwise continue parsing and prey there will be no errors
		//in the worst case, some other exception will fire
	}
}

BGPCommonHeader::~BGPCommonHeader()
{
}

BGPMessagePtr BGPCommonHeader::newMessage( istream &input, bool isAS4 )
{
	BGPMessagePtr bgpMsg;
	BGPMessagePtr header( new BGPCommonHeader(input) );

	io::stream<io::array_source> in( header->data.get(), header->length );

	switch( header->getType() )
	{
	case BGPCommonHeader::OPEN:
		LOG4CXX_TRACE(Logger,"BGP_OPEN");
		bgpMsg = BGPMessagePtr( new BGPOpen(*header,input) );
		break;

	case BGPCommonHeader::UPDATE:
		LOG4CXX_TRACE(Logger,"BGP_UPDATE");
		bgpMsg = BGPMessagePtr( new BGPUpdate(*header,in,isAS4) );
		break;

	case BGPCommonHeader::NOTIFICATION:
		LOG4CXX_TRACE(Logger,"BGP_NOTIFICAION");
		bgpMsg = BGPMessagePtr( new BGPNotification(*header,in) );
		break;

	case BGPCommonHeader::KEEPALIVE:
		LOG4CXX_TRACE(Logger,"BGP_KEEPALIVE");
		bgpMsg = BGPMessagePtr( new BGPKeepAlive(*header,in) );
		break;

	case BGPCommonHeader::ROUTE_REFRESH:
		LOG4CXX_TRACE(Logger,"BGP_ROUTE_REFRESH");
		bgpMsg = BGPMessagePtr( new BGPRouteRefresh(*header,in) );
		break;
	default:
		LOG4CXX_ERROR(Logger,"Unsupported BGP message type");
		bgpMsg = header;
		break;
	}
	return bgpMsg;
}
