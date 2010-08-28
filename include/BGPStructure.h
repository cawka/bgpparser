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

// Author: Jason Ryder
// Common file for containing BGP structures

#ifndef __BGPSTRUCTURE_H_
#define __BGPSTRUCTURE_H_

#include "MRTStructure.h"
#include "MRTCommonHeader.h"

#include <string>

class Route
{
public:
	Route( uint8_t aLength, std::istream &input );
	//	Route(const Route &rt) : length(rt.length), numOctets(rt.numOctets)
	//	{
	//		memset(&prefix, 0, sizeof(IPAddress));
	//		memcpy(&prefix, &rt.prefix, sizeof(IPAddress));
	//	}
/* THIS COPY CONSTRUCTOR IS CAUSING A LOT OF WEIRDNESS -- DOES NOT COPY CORRECTLY!
	// Copy constructor
	Route(const Route& rt)
	{
		PRINT_DBG("  Copying a Route");
		Route(rt.length, (uint8_t*)(&rt.prefix));
	}
*/
	virtual ~Route() { }

	inline uint8_t getLength() { return length; }
	inline uint8_t setLength(uint8_t aLength) { length = aLength; return length; }
	inline IPAddress getPrefix() const { return prefix; }
	inline IPAddress setPrefix(IPAddress in_prefix) { memcpy(&prefix, &in_prefix, sizeof(IPAddress)); return prefix; }
	inline int getNumOctets() { return numOctets; }

	std::string toString(uint16_t afi=0);
	
	void printMe();
	void printMeCompact();

private:
	Route( ) {}

protected:
	Route( uint8_t aLength, IPAddress aPrefix )
	{
		length = aLength;
		numOctets = aLength / 8 + ((aLength % 8) ? 1 : 0);
		memcpy( &prefix, &aPrefix, sizeof(IPAddress) );
	}

protected:
	uint8_t length; // number of bits in the IP prefix.
	IPAddress prefix; // Variable length prefix - padded to equal ceil(length / 8) octets. 
	int numOctets;

	static log4cxx::LoggerPtr Logger;
};

typedef boost::shared_ptr<Route> RoutePtr;

class NLRIReachable : public Route
{
public:
	NLRIReachable(uint8_t aLength, std::istream &input)  : Route(aLength, input) {}
	NLRIReachable(uint8_t aLength, IPAddress aPrefix) : Route(aLength, aPrefix) {}

	~NLRIReachable() {}
};

typedef boost::shared_ptr<NLRIReachable> NLRIReachablePtr;


class NLRIUnReachable : public Route
{
public:
	NLRIUnReachable(uint8_t aLength, std::istream &input)  : Route(aLength, input) {}
	NLRIUnReachable(uint8_t aLength, IPAddress aPrefix): Route(aLength, aPrefix) {}

	~NLRIUnReachable() {}
};

typedef boost::shared_ptr<NLRIUnReachable> NLRIUnReachablePtr;

#endif /* __BGPSTRUCTURE_H_ */

