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

#include "AttributeTypeMPUnreachNLRI.h"
#include "Exceptions.h"
using namespace std;

#include <boost/iostreams/read.hpp>
#include <boost/iostreams/skip.hpp>
#include <boost/foreach.hpp>
namespace io = boost::iostreams;

log4cxx::LoggerPtr AttributeTypeMPUnreachNLRI::Logger = log4cxx::Logger::getLogger( "bgpparser.AttributeTypeMPUnreachNLRI" );

AttributeTypeMPUnreachNLRI::AttributeTypeMPUnreachNLRI( AttributeType &header, std::istream &input )
						   : AttributeType(header)
{
	LOG4CXX_TRACE(Logger,"");
	bool error=false;

	error|= sizeof(uint16_t)!=
			io::read( input, reinterpret_cast<char*>(&afi), sizeof(uint16_t) );
	afi = ntohs(afi);
	if( error || ((afi != AFI_IPv4) &&  (afi != AFI_IPv6)) )
   	{
		LOG4CXX_ERROR(Logger,"unknown address family " << (int)afi );
		throw BGPError( );
	}

	error|= sizeof(uint8_t)!=
			io::read( input, reinterpret_cast<char*>(&safi), sizeof(uint8_t) );
	if( error || (safi != SAFI_UNICAST && safi!= SAFI_MULTICAST) )
	{
		LOG4CXX_ERROR(Logger,"unknown subsequent address family " << (int)safi );
		throw BGPError( );
	}

	int left=length - 2 - 1;

	while( left>0 )
	{
		uint8_t prefixLength;
	   	error|= sizeof(uint8_t)!=
			io::read( input, reinterpret_cast<char*>(&prefixLength), sizeof(uint8_t) );

		if( error || prefixLength > sizeof(IPAddress)*8 ) 
		{ 
			LOG4CXX_ERROR(Logger,"abnormal prefix-length ["<< (int)prefixLength <<"]. skip this record." );
			throw BGPError( );
		}

		NLRIUnReachablePtr route( new NLRIUnReachable(prefixLength, input) );

		left -= 1+route->getNumOctets();
		nlri.push_back(route);
	}
}

AttributeTypeMPUnreachNLRI::~AttributeTypeMPUnreachNLRI(void) {
}

void AttributeTypeMPUnreachNLRI::printMe() {
	cout << "MBGP-WITHDRAWN: ";
	
	BOOST_FOREACH( NLRIUnReachablePtr entry, nlri )
	{
		cout << endl;
		entry->printMe( afi );
	}
}

void AttributeTypeMPUnreachNLRI::printMeCompact()
{
	cout << "MBGP-WITHDRAWN:";

	BOOST_FOREACH( NLRIUnReachablePtr entry, nlri )
	{
		cout << " ";
		entry->printMeCompact( afi );
	}
}
