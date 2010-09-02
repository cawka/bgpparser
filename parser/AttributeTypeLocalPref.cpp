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

#include "AttributeTypeLocalPref.h"
using namespace std;

#include <boost/iostreams/read.hpp>
namespace io = boost::iostreams;

log4cxx::LoggerPtr AttributeTypeLocalPref::Logger = log4cxx::Logger::getLogger( "bgpparser.AttributeTypeLocalPref" );

AttributeTypeLocalPref::AttributeTypeLocalPref( AttributeType &header, std::istream &input )
					   : AttributeType(header) {
	LOG4CXX_DEBUG(Logger,"");

	io::read( input, reinterpret_cast<char*>(&localPref), sizeof(&localPref) );
	localPref = ntohl(localPref);
}

AttributeTypeLocalPref::~AttributeTypeLocalPref(void) {
	/* nothing */
}

void AttributeTypeLocalPref::printMeCompact() {
	cout << "LOCAL_PREF: " << localPref;
}

void AttributeTypeLocalPref::printMe() {
	cout << "LOCAL_PREF: " << localPref;
}

