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
// Modified: Jonathan Park
#include <bgpparser.h>

#include "AttributeTypeOrigin.h"

using namespace std;

log4cxx::LoggerPtr AttributeTypeOrigin::Logger = log4cxx::Logger::getLogger( "bgpparser.AttributeTypeOrigin" );

AttributeTypeOrigin::AttributeTypeOrigin(void) {
	/* nothing */
}

AttributeTypeOrigin::AttributeTypeOrigin(uint16_t len, uint8_t* msg)
					: AttributeType(len, msg) {
	LOG4CXX_TRACE(Logger,"AttributeTypeOrigin::AttributeTypeOrigin()");
	origin = (Origin)*msg;
}

AttributeTypeOrigin::AttributeTypeOrigin(uint16_t len, Origin org, uint8_t* msg)
					: AttributeType(len, msg) {
	origin = org;
}

AttributeTypeOrigin::AttributeTypeOrigin(const AttributeTypeOrigin& attr) {
	length = attr.length;
	origin = attr.origin;
	if (attr.value) {
		value = (uint8_t*)malloc(length);
		memcpy(value, attr.value, length);
	} else {
		value = NULL;
	}
}


AttributeTypeOrigin::~AttributeTypeOrigin(void) {
	/* nothing */
}

void AttributeTypeOrigin::printMe() { 
	cout << "ORIGIN: ";
	switch(origin) {
	case IGP: cout << "IGP"; break;
	case EGP: cout << "EGP"; break;
	default:  cout << "INCOMPLETE"; break;
	}
}

void AttributeTypeOrigin::printMeCompact() { 
	cout << "ORIGIN: ";
	switch(origin) {
	case IGP: cout << "IGP"; break;
	case EGP: cout << "EGP"; break;
	default:  cout << "INCOMPLETE"; break;
	}
}
