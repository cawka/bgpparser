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

// Author: Jonathan Park (jpark@cs.ucla.edu)
#include <bgpparser.h>

#include "BGPOpen.h"

#include <boost/iostreams/read.hpp>
namespace io = boost::iostreams;

log4cxx::LoggerPtr BGPOpen::Logger = log4cxx::Logger::getLogger( "bgpparser.BGPOpen" );

BGPOpen::BGPOpen( BGPCommonHeader &header, std::istream &input )
		: BGPCommonHeader( header )
{
	LOG4CXX_TRACE(Logger,"BGPOpen::BGPOpen()");

	io::read( input, reinterpret_cast<char*>(&version), sizeof(uint8_t) );

	io::read( input, reinterpret_cast<char*>(&myAS), sizeof(uint16_t) );
	myAS = ntohs(myAS);

	io::read( input, reinterpret_cast<char*>(&holdTime), sizeof(uint16_t) );
	holdTime = ntohs(holdTime);

	io::read( input, reinterpret_cast<char*>(&bgpId), sizeof(uint32_t) );
	bgpId = ntohl(bgpId);

	io::read( input, reinterpret_cast<char*>(&optParmLen), sizeof(uint8_t) );
}

BGPOpen::~BGPOpen() { 
	/* nothing */
}

void BGPOpen::printMe() {
	/* nothing */
}

void BGPOpen::printMeCompact() {
	/* nothing */
}
