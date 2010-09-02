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

#include "BGPAttribute.h"
#include "Exceptions.h"
using namespace std;

#include <boost/iostreams/read.hpp>
#include <boost/iostreams/skip.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
namespace io = boost::iostreams;

log4cxx::LoggerPtr BGPAttribute::Logger = log4cxx::Logger::getLogger( "bgpparser.BGPAttribute" );

BGPAttribute::BGPAttribute( istream &input, bool isAS4 )
{
	bool error=false;
	error|= sizeof(uint8_t)!=
			io::read( input, reinterpret_cast<char*>(&attributeFlags),    sizeof(uint8_t) );
	error|= sizeof(uint8_t)!=
			io::read( input, reinterpret_cast<char*>(&attributeTypeCode), sizeof(uint8_t) );

	uint16_t len = 0;
	// This methods handles endianess conversion in a generic way. If they were to
	//  change network byte order to little endian, this code would still be semantically
	//  correct.
	if( isExtendedLength( ) )
	{
		error|= sizeof(uint16_t)!=
				io::read( input, reinterpret_cast<char*>(&len), sizeof(uint16_t) );
		len = ntohs(len);
	}
	else
	{
		uint8_t len8;
		error|= sizeof(uint8_t)!=
				io::read( input, reinterpret_cast<char*>(&len8), sizeof(uint8_t) );
		len=len8;
	}
	
	if( error )
	{
		LOG4CXX_ERROR( Logger, "Parsing error" );
		throw BGPError( );
	}

	setAttributeLength( len );

	value = AttributeType::newAttribute( attributeTypeCode, len, input, isAS4 );
}

BGPAttribute::~BGPAttribute(void)
{
}


