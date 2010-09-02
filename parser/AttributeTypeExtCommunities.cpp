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

// Author: Jason Ryder, Paul Wang
// Modified: Jonathan Park (jpark@cs.ucla.edu)
#include <bgpparser.h>

#include "AttributeTypeExtCommunities.h"
#include "Exceptions.h"
using namespace std;

#include <boost/iostreams/read.hpp>
namespace io = boost::iostreams;

log4cxx::LoggerPtr AttributeTypeExtCommunities::Logger = log4cxx::Logger::getLogger( "bgpparser.AttributeTypeExtCommunities" );

AttributeTypeExtCommunities::AttributeTypeExtCommunities( AttributeType &header, std::istream &input )
							: AttributeType(header)
{
	LOG4CXX_DEBUG( Logger,"" );
	
	while( input.peek()!=-1 )
	{
		ExtCommunityValue extCommunity;

		bool error=false;
		error|= sizeof(uint8_t)!=
				io::read( input, reinterpret_cast<char*>(&extCommunity.typeHigh), sizeof(uint8_t) );

		error|= sizeof(uint8_t)!=
				io::read( input, reinterpret_cast<char*>(&extCommunity.typeLow),  sizeof(uint8_t) );

		error|= sizeof(extCommunity.rchValue)!=
				io::read( input, reinterpret_cast<char*>(extCommunity.rchValue), sizeof(extCommunity.rchValue) );

		if( error )
		{
			LOG4CXX_ERROR(Logger,"Parsing error");
			throw BGPError( );
		}

		extCommunityValues.push_back( extCommunity );
	}
}

AttributeTypeExtCommunities::~AttributeTypeExtCommunities(void)
{
}

void AttributeTypeExtCommunities::printMe() {
	cout << "EXTENDED_COMMUNITIES:";
	list<ExtCommunityValue>::iterator iter;

	for(iter = extCommunityValues.begin(); iter != extCommunityValues.end(); iter++) {
		cout << " " << (int)(*iter).typeHigh << ":" << (int)(*iter).typeLow << ":";
		printf("%02x%02x%02x%02x%02x%02x", 
					(*iter).rchValue[0], (*iter).rchValue[1], (*iter).rchValue[2],
					(*iter).rchValue[3], (*iter).rchValue[4], (*iter).rchValue[5] );
	}
}

void AttributeTypeExtCommunities::printMeCompact() {
	cout << "EXTENDED_COMMUNITIES: ";
	list<ExtCommunityValue>::iterator iter;
	bool isFirstLoop = true;
	for(iter = extCommunityValues.begin(); iter != extCommunityValues.end(); iter++) {
		cout << (isFirstLoop ? "" : " ") << (int)(*iter).typeHigh << ":" << (int)(*iter).typeLow << ":";
		printf("%02x%02x%02x%02x%02x%02x", 
					(*iter).rchValue[0], (*iter).rchValue[1], (*iter).rchValue[2],
					(*iter).rchValue[3], (*iter).rchValue[4], (*iter).rchValue[5] );
		isFirstLoop = false;
	}
}
