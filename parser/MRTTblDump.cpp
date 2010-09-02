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

#include "MRTTblDump.h"
#include "Exceptions.h"
using namespace std;

#include <boost/iostreams/read.hpp>
namespace io = boost::iostreams;

log4cxx::LoggerPtr MRTTblDump::Logger = log4cxx::Logger::getLogger( "bgpparser.MRTTblDump" );

MRTTblDump::MRTTblDump( MRTCommonHeader &header, istream &input )
: MRTCommonHeader( header )
{
	/* copy out the view number and convert to host order */
	io::read( input, reinterpret_cast<char*>(&viewNumber), sizeof(uint16_t) );
	viewNumber = ntohs(viewNumber);

	/* copy out the sequence number and convert to host order */
	io::read( input, reinterpret_cast<char*>(&sequenceNumber), sizeof(uint16_t) );
	sequenceNumber = ntohs(sequenceNumber);

	int left=length - 4;

	/* copy out the prefix and increment the pointer */
	if( getSubType() == AFI_IPv4 )
	{
		io::read( input, reinterpret_cast<char*>(&prefix.ipv4), sizeof(prefix.ipv4) );
		left-=sizeof(prefix.ipv4);
	}
	else if (getSubType() == AFI_IPv6)
	{
		io::read( input, reinterpret_cast<char*>(&prefix.ipv6), sizeof(prefix.ipv6) );
		left-=sizeof(prefix.ipv6);
	}
	else {
		LOG4CXX_ERROR(Logger,"unsupported subtype ["<< getSubType() <<"] for table-dump." );
		throw BGPError( );
	}

	/* copy out the prefix length */
	prefixLength = input.get( );

	/* copy out the status */
	status = input.get( );
	/* version 9 of the spec indicates that this field should always be set to 1 */
	status = (uint8_t)1;

	/* copy out the originated, increment the pointer and convert to host order */
	io::read( input, reinterpret_cast<char*>(&originatedTime), sizeof(uint32_t) );
	originatedTime = ntohl(originatedTime);

	left-=2 + 4;

	/* copy out the peer IP and increment the pointer */
	if (getSubType() == AFI_IPv4)
	{
		io::read( input, reinterpret_cast<char*>(&peerIP.ipv4), sizeof(peerIP.ipv4) );
		left-=sizeof(prefix.ipv4);
	}
	else if (getSubType() == AFI_IPv6)
	{
		io::read( input, reinterpret_cast<char*>(&peerIP.ipv6), sizeof(peerIP.ipv6) );
		left-=sizeof(prefix.ipv6);
	}

	/* copy out the peer AS, increment the pointer and convert to host order */
	io::read( input, reinterpret_cast<char*>(&peerAS), sizeof(uint16_t) );
	peerAS = ntohs(peerAS);

	/* copy out the attribute length, increment the pointer and convert to host order */
	io::read( input, reinterpret_cast<char*>(&attributeLength), sizeof(uint16_t) );
	attributeLength = ntohs(attributeLength);

	left-=4;

	MRTTblDumpV2RibHeader::processAttributes( attributes, input, left, false );
}

MRTTblDump::~MRTTblDump(void)
{
}

uint16_t MRTTblDump::getViewNumber(void) const {
	return viewNumber;
}

uint16_t MRTTblDump::getSequenceNumber(void) const {
	return sequenceNumber;
}

IPAddress MRTTblDump::getPrefix(void) const {
	return prefix;
}

uint8_t MRTTblDump::getPrefixLength(void) const {
	return prefixLength;
}

uint8_t MRTTblDump::getStatus(void) const {
	return status;
}

time_t MRTTblDump::getOriginatedTime(void) const {
	return originatedTime;
}

IPAddress MRTTblDump::getPeerIP(void) const {
	return peerIP;
}

uint16_t MRTTblDump::getPeerAS(void) const {
	return peerAS;
}

uint16_t MRTTblDump::getAttributeLength(void) const {
	return attributeLength;
}

void MRTTblDump::printMeCompact() {
	cout << viewNumber << "|" << sequenceNumber << "|";
	if (getSubType() == AFI_IPv4) {
		PRINT_IP_ADDR(prefix.ipv4);
	} else {
		PRINT_IPv6_ADDR(prefix.ipv6);
	}
	cout << "/";
	printf("%u", prefixLength);
	cout << "|";
	printf("%u", status);
	cout << "|";

	if (getSubType() == AFI_IPv4) {
		PRINT_IP_ADDR(peerIP.ipv4);
	} else {
		PRINT_IPv6_ADDR(peerIP.ipv6);
	}
	cout << "|" << peerAS;

	list<BGPAttributePtr>::iterator it;
	for (it = attributes.begin(); it != attributes.end(); it++) {
		cout << "|";
		(*it)->printMeCompact();
	}
}
