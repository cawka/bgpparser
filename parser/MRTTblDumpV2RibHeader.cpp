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

// Author: Paul Wang, Jason Ryder
// Modified: Jonathan Park (jpark@cs.ucla.edu)
#include <bgpparser.h>

#include "MRTTblDumpV2RibHeader.h"
#include "MRTTblDumpV2PeerIndexTbl.h"

//#include "TblDumpV2RibEntry.h"
//#include "AttributeType.h"
//#include "AttributeTypeOrigin.h"
#include "AttributeTypeASPath.h"
#include "AttributeTypeAS4Path.h"
//#include "AttributeTypeNextHop.h"
//#include "AttributeTypeMultiExitDisc.h"
//#include "AttributeTypeLocalPref.h"
//#include "AttributeTypeAtomicAggregate.h"
//#include "AttributeTypeAggregator.h"
//#include "AttributeTypeCommunities.h"
//#include "AttributeTypeMPReachNLRI.h"
//#include "AttributeTypeMPUnreachNLRI.h"

log4cxx::LoggerPtr MRTTblDumpV2RibHeader::Logger = log4cxx::Logger::getLogger( "bgpparser.MRTTblDumpV2RibHeader" );

MRTTblDumpV2PeerIndexTblPtr MRTTblDumpV2RibHeader::_peerIndexTbl;

MRTTblDumpV2RibHeader::MRTTblDumpV2RibHeader( MRTCommonHeader &header )
: MRTCommonHeader(header)
{
}

MRTTblDumpV2RibHeader::~MRTTblDumpV2RibHeader(void) {
}

uint32_t MRTTblDumpV2RibHeader::getSequenceNumber(void) const {
	return sequenceNumber;
}

uint8_t MRTTblDumpV2RibHeader::getPrefixLength(void) const {
	return prefixLength;
}

IPAddress MRTTblDumpV2RibHeader::getPrefix(void) const {
	return prefix;
}

uint16_t MRTTblDumpV2RibHeader::getEntryCount(void) const {
	return entryCount;
}

uint16_t MRTTblDumpV2RibHeader::getAFI(void) const {
	return afi;
}

uint16_t MRTTblDumpV2RibHeader::getSAFI(void) const {
	return safi;
}

/* static */void MRTTblDumpV2RibHeader::processAttributes(
		list<BGPAttributePtr> &attributes, istream &input, int len, bool isAS4 )
{

	LOG4CXX_TRACE(Logger,"MRTTblDumpV2RibHeader::processAttributes(...)");

	int left=len;
	while( left>0 )
	{
		/**
		 * http://tools.ietf.org/id/draft-ietf-grow-mrt-09.txt
		 *
		 * There is one exception to the encoding of BGP attributes for the BGP
		 * MP_REACH_NLRI attribute (BGP Type Code 14) [RFC 4760].  Since the
		 * AFI, SAFI, and NLRI information is already encoded in the
		 * MULTIPROTOCOL header, only the Next Hop Address Length and Next Hop
		 * Address fields are included.  The Reserved field is omitted.  The
		 * attribute length is also adjusted to reflect only the length of the
		 * Next Hop Address Length and Next Hop Address fields.
		 *
         */
		boost::shared_ptr<BGPAttribute> attrib = boost::shared_ptr<BGPAttribute>( new BGPAttribute(input, isAS4) );
		attributes.push_back( attrib );
		left-=attrib->totalSize( );
	}

	// Post processing
	std::list<BGPAttributePtr>::iterator as_path=
		find_if( attributes.begin(), attributes.end(), findByType(AttributeType::AS_PATH) );

	std::list<BGPAttributePtr>::iterator as4_path=
		find_if( attributes.begin(), attributes.end(), findByType(AttributeType::NEW_AS_PATH) );

	if( as_path!=attributes.end() && as4_path!=attributes.end() )
	{
		AttributeTypeASPathPtr  as=
				boost::dynamic_pointer_cast<AttributeTypeASPath>( (*as_path)->getAttributeValueMutable() );
		AttributeTypeAS4PathPtr as4=
				boost::dynamic_pointer_cast<AttributeTypeAS4Path>( (*as4_path)->getAttributeValue() );

		as->genPathSegmentsComplete( *as4 );
	}
	else if( as_path!=attributes.end() )
		boost::dynamic_pointer_cast<AttributeTypeASPath>( (*as_path)->getAttributeValueMutable() )
			->genPathSegmentsComplete( );

	LOG4CXX_TRACE(Logger,"END MRTTblDumpV2RibHeader::processAttributes(...)");
}


void MRTTblDumpV2RibHeader::printMe()
{
	cout << "PREFIX: ";
	if( afi==AFI_IPv4 )
	{
		PRINT_IP_ADDR(prefix.ipv4);
	}
	else
	{
		PRINT_IPv6_ADDR(prefix.ipv6);
	}
	cout << "/" << (int)(prefixLength & BITMASK_8) << endl;
	cout << "SEQUENCE: " << sequenceNumber;
}

void MRTTblDumpV2RibHeader::printMe( const MRTTblDumpV2PeerIndexTblPtr &peerIndexTbl )
{
	printMe();
	cout << endl;
	// Now continue with information from the peer index table
	list<TblDumpV2RibEntryPtr>::iterator iter;
	for (iter = ribs.begin(); iter != ribs.end(); iter++)
	{
		int peerIndex = (*iter)->getPeerIndex();

		const MRTTblDumpV2PeerIndexTblPeerEntryPtr &peer=peerIndexTbl->getPeer( peerIndex );
		cout << "FROM: ";
		// print from IP
		if (peer->IPType == AFI_IPv4) {
			PRINT_IP_ADDR(peer->peerIP.ipv4);
		} else {
			PRINT_IPv6_ADDR(peer->peerIP.ipv6);
		}
		cout << " AS" << peer->peerAS;
		cout << endl;

		(*iter)->printMe();
		cout << endl;
	}
}

void MRTTblDumpV2RibHeader::printMeCompact() {
	PRINT_IP_ADDR(prefix.ipv4) ;
	cout << "/" << (int)(prefixLength & BITMASK_8);
	cout << "|" << sequenceNumber << "|";
}

void MRTTblDumpV2RibHeader::printMeCompact( const MRTTblDumpV2PeerIndexTblPtr &peerIndexTbl )
{
	list<TblDumpV2RibEntryPtr>::iterator iter;

	cout << "ENTRY_CNT: " << ribs.size() << endl;
	for (iter = ribs.begin(); iter != ribs.end(); iter++)
	{
		printMeCompact();

		int peerIndex=(*iter)->getPeerIndex( );

		const MRTTblDumpV2PeerIndexTblPeerEntryPtr &peer=peerIndexTbl->getPeer( peerIndex );

		// print from IP
		if (peer->IPType == AFI_IPv4) {
			PRINT_IP_ADDR(peer->peerIP.ipv4);
		} else {
			PRINT_IPv6_ADDR(peer->peerIP.ipv6);
		}

		// Print from AS
		uint16_t top, bottom;
		top = (uint16_t)((peer->peerAS>>16)&0xFFFF);
		bottom = (uint16_t)((peer->peerAS)&0xFFFF);
		printf("|");
		if( top == 0 ) {
			printf("%u",bottom);
		} else {
			printf("%u.%u",top,bottom);
		}
		printf("|");

		(*iter)->printMeCompact();
		cout << endl;
	}
}

