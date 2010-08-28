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

#include "AttributeTypeMPReachNLRI.h"
#include "Exceptions.h"
using namespace std;

#include <boost/iostreams/read.hpp>
#include <boost/iostreams/skip.hpp>
namespace io = boost::iostreams;

log4cxx::LoggerPtr AttributeTypeMPReachNLRI::Logger = log4cxx::Logger::getLogger( "bgpparser.AttributeTypeMPReachNLRI" );

AttributeTypeMPReachNLRI::AttributeTypeMPReachNLRI( AttributeType &header, istream &input )
						 : AttributeType(header)
{
	LOG4CXX_TRACE(Logger,"");
	corrupt = 0;

	// Test if there is only "NextHopLength" and "NextHopAddress" in this attribute (MRT TABLE_DUMP_V2 format)
	// http://tools.ietf.org/id/draft-ietf-grow-mrt-09.txt
	if( length == 1 + nextHopAddressLength )
	{
		throw BGPTextError( "NOTIFY: ok, we got here" );
		nextHopAddressLength=input.get( );
		
		afi = 0;
		safi = 0;
		memset( reinterpret_cast<char*>(&nextHopAddress), 0, sizeof(IPAddress) );
		io::read( input, reinterpret_cast<char*>(&nextHopAddress), nextHopAddressLength );
		return;
	}

	// Else parse every fields specified in RFC 4760
	io::read( input, reinterpret_cast<char*>(&afi), sizeof(uint16_t) );
	afi = ntohs(afi);
	LOG4CXX_TRACE(Logger,"  afi = " << (int)afi);

	/* SAFI */
	safi = input.get( );
	LOG4CXX_TRACE(Logger,"  safi = " << (int)safi);
	// SAFI not valid in MP_REACH_NLRI

	/* Next hop length and address */
	nextHopAddressLength = input.get( );
	LOG4CXX_TRACE(Logger,"  nextHopAddressLength = " << (int)nextHopAddressLength);

	memset( reinterpret_cast<char*> ( &nextHopAddress ),      0, sizeof(IPAddress) );
	memset( reinterpret_cast<char*> ( &nextHopAddressLocal ), 0, sizeof(IPAddress) );

	if( nextHopAddressLength == 32 ) {
		/* this must be ipv6 with local address as well */
		io::read( input, reinterpret_cast<char*>(&nextHopAddress.ipv6),      sizeof(nextHopAddress.ipv6) );
		io::read( input, reinterpret_cast<char*>(&nextHopAddressLocal.ipv6), sizeof(nextHopAddressLocal.ipv6) );
	} else if( nextHopAddressLength == 16 ) {
		/* this must be ipv6 */
		io::read( input, reinterpret_cast<char*>(&nextHopAddress.ipv6),      sizeof(nextHopAddress.ipv6) );
	} else if( nextHopAddressLength == 4 ) {
		/* this must be ipv4 */
		io::read( input, reinterpret_cast<char*>(&nextHopAddress.ipv4),      sizeof(nextHopAddress.ipv4) );
	}

	int left=length - /*afi*/2 - /*safi*/1 - /*nextHopAddressLength*/1 - nextHopAddressLength;

	/* JP: let's not worry about SNPA for now */
	uint8_t unSNPACnt = 0;
	uint8_t unSNPALen = 0;
	unSNPACnt = input.get( );
	LOG4CXX_TRACE(Logger,"  unSNPACnt = " << (int)unSNPACnt);

	left--;

	for(int ic=0; ic<unSNPACnt; ic++) {
		unSNPALen = input.get( );
		io::detail::skip( input, unSNPALen, boost::mpl::false_() );

		left-=1+unSNPALen;
	}

	while( left>0 ) {
		uint8_t prefixLength = 0;
		prefixLength = input.get( );
//		if( ptr > endMsg ) {
//			LOG4CXX_ERROR(Logger,"message truncated! cannot read mbgp prefix.");
//			break;
//		}
		if( prefixLength > sizeof(IPAddress)*8) { 
			LOG4CXX_ERROR(Logger,"abnormal prefix-length ["<< (int)prefixLength <<"]. skip this record." );

			LOG4CXX_ERROR(Logger,"Length = " << (int)length << ", left = " << left );
			throw BGPError( );
			break;
		}
		LOG4CXX_TRACE(Logger,"prefixLength = " << (int)prefixLength);
		
		NLRIReachablePtr route( new NLRIReachable(prefixLength, input) );
//		if( route.getNumOctets()+ptr > endMsg ) {
//			LOG4CXX_ERROR(Logger,"message (mbgp a) truncated! need to read ["<< route.getNumOctets() <<"], "<<
//					"but only have ["<< (int)(endMsg-ptr) <<"] bytes." );
//			break;
//		}
		left -= 1+route->getNumOctets();
		nlri.push_back(route);
		LOG4CXX_TRACE(Logger, "num_octets = " << (int)route->getNumOctets() );
	}
}

AttributeTypeMPReachNLRI::~AttributeTypeMPReachNLRI(void)
{
}

void AttributeTypeMPReachNLRI::printMe() {
	cout << "ANNOUNCE:";
	list<NLRIReachablePtr>::iterator iter;
	
	for(iter = nlri.begin(); iter != nlri.end(); iter++) {
		cout << endl;
		(*iter)->printMe();
	}
}

void AttributeTypeMPReachNLRI::printMeCompact() {
	cout << "MBGP-NEXTHOP: ";
	if( afi == AFI_IPv4 ) {
		PRINT_IP_ADDR(nextHopAddress.ipv4);
	} else {
		PRINT_IPv6_ADDR(nextHopAddress.ipv6);
	}
	cout << "^MBGP-ANNOUNCE:";
	list<NLRIReachablePtr>::iterator iter;
	for(iter = nlri.begin(); iter != nlri.end(); iter++) {
		cout << " ";
		if( afi == AFI_IPv4 ) {
			/* ipv4 */
			(*iter)->printMeCompact();
		} else {
			/* ipv6 */
			PRINT_IPv6_ADDR((*iter)->getPrefix().ipv6);
			cout << "/";
			printf("%u", (*iter)->getLength());
		}
	}
}
