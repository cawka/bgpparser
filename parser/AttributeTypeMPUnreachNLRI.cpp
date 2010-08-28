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
using namespace std;

#include <boost/iostreams/read.hpp>
#include <boost/iostreams/skip.hpp>
namespace io = boost::iostreams;

log4cxx::LoggerPtr AttributeTypeMPUnreachNLRI::Logger = log4cxx::Logger::getLogger( "bgpparser.AttributeTypeMPUnreachNLRI" );

AttributeTypeMPUnreachNLRI::AttributeTypeMPUnreachNLRI( AttributeType &header, std::istream &input )
						   : AttributeType(header)
{
	LOG4CXX_TRACE(Logger,"");
	corrupt = 0;

	io::read( input, reinterpret_cast<char*>(&afi), sizeof(uint16_t) );
	afi = ntohs(afi);
	if((afi != AFI_IPv4) &&  (afi != AFI_IPv6)) {
		LOG4CXX_ERROR(Logger,"unknown address family " << afi );
		corrupt = 1;
		return;
	}

	safi = input.get( );
	if ((((uint32_t)safi & BITMASK_8) != SAFI_UNICAST) && (((uint32_t)safi & BITMASK_8) != SAFI_MULTICAST)) {
		LOG4CXX_ERROR(Logger,"unknown subsequent address family " << (uint32_t)safi );
	}

	int left=length - 2 - 1;

	while( left>0 )
	{
		uint8_t prefixLength = input.get( );

		if( prefixLength > sizeof(IPAddress)*8) { 
			LOG4CXX_ERROR(Logger,"abnormal prefix-length ["<< prefixLength <<"]. skip this record." );
			break;
		}
		NLRIUnReachablePtr route( new NLRIUnReachable(prefixLength, input) );
//		if( route.getNumOctets()+ptr > endMsg ) {
//			LOG4CXX_ERROR(Logger,"message (mbgp w) truncated! need to read ["<< route.getNumOctets()
//					<<"], but only have ["<< (int)(endMsg-ptr) <<"] bytes." );
//			break;
//		}
		left -= 1+route->getNumOctets();
		nlri.push_back(route);
	}
}

AttributeTypeMPUnreachNLRI::~AttributeTypeMPUnreachNLRI(void) {
}

void AttributeTypeMPUnreachNLRI::printMe() {
	cout << "MBGP-WITHDRAWN: ";
	list<NLRIUnReachablePtr>::iterator iter;
	
	for(iter = nlri.begin(); iter != nlri.end(); iter++) {
		cout << endl;
		(*iter)->printMe();
	}
}

void AttributeTypeMPUnreachNLRI::printMeCompact()
{
	cout << "MBGP-WITHDRAWN:";
	list<NLRIUnReachablePtr>::iterator iter;
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
