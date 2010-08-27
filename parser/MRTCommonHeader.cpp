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

#include "MRTCommonHeader.h"

#include "MRTBgp4MPStateChange.h"
#include "MRTBgp4MPMessage.h"
#include "MRTBgp4MPEntry.h"
#include "MRTBgp4MPSnapshot.h"

#include "MRTTblDump.h"
#include "MRTTblDumpV2PeerIndexTbl.h"
#include "MRTTblDumpV2RibHeader.h"
#include "MRTTblDumpV2RibIPv4Unicast.h"
#include "MRTTblDumpV2RibIPv4Multicast.h"
#include "MRTTblDumpV2RibIPv6Unicast.h"
#include "MRTTblDumpV2RibIPv6Multicast.h"
#include "MRTTblDumpV2RibGeneric.h"

#include "BGPCommonHeader.h"

#include <Exceptions.h>

#include <boost/iostreams/read.hpp>
#include <boost/iostreams/skip.hpp>
namespace io = boost::iostreams;

log4cxx::LoggerPtr MRTCommonHeader::Logger = log4cxx::Logger::getLogger( "bgpparser.MRTCommonHeader" );

MRTCommonHeader::MRTCommonHeader(void) {
	/* nothing */
	throw std::exception( );
}

MRTCommonHeader::MRTCommonHeader( const uint8_t **input )
{
	throw std::exception( );
}

MRTCommonHeader::MRTCommonHeader( istream &input )
{
	MRTCommonHeader();
	MRTCommonHeaderPacket pkt;

	int len=io::read( input, reinterpret_cast<char*>(&pkt), sizeof(MRTCommonHeaderPacket) );
	if( len<0 ) throw EOFException( );
	if( len!=sizeof(MRTCommonHeaderPacket) ) throw MRTException( "MRT format error" );

	timestamp = ntohl(pkt.timestamp);
	type = ntohs(pkt.type);
	subtype = ntohs(pkt.subtype);
	length = ntohl(pkt.length);
}

MRTCommonHeader::~MRTCommonHeader(void) {
	/* nothing */
}

time_t MRTCommonHeader::getTimestamp(void) const {
	return timestamp;
}

uint16_t MRTCommonHeader::getType(void) const {
	return type;
}

uint16_t MRTCommonHeader::getSubType(void) const {
	return subtype;
}

uint32_t MRTCommonHeader::getLength(void) const {
	return length;
}

/* 
 * Factory method for creating MRTMessages
 */ 
MRTMessagePtr MRTCommonHeader::newMessage( istream &input ) {
	LOG4CXX_TRACE( Logger, "==> MRTCommonHeader::newMessage( istream &input )" );

	MRTMessagePtr msg;
	MRTCommonHeader header( input );
	
	LOG4CXX_DEBUG( Logger, "Type:      %u" << header.getType() );
	LOG4CXX_DEBUG( Logger, "SubType:   %u" << header.getSubType() );
	LOG4CXX_DEBUG( Logger, "Length:    %u" << header.getLength() );

	/* only accept BGP4MP message and TABLE_DUMP message types */
	if ((header.getType() != BGP4MP) && /*(header.getType() != BGP4MP_ET) && */
		(header.getType() != TABLE_DUMP) && (header.getType() != TABLE_DUMP_V2))
	{
		io::detail::skip( input, header.getLength(), boost::mpl::false_() );
		return msg;
	}

	if( header.getType( ) == BGP4MP )
	{
		switch( header.getSubType( ) )
		{
		case BGP4MP_STATE_CHANGE:
			LOG4CXX_INFO(Logger,"MRTBgp4MPStateChange");

			msg = MRTMessagePtr( new MRTBgp4MPStateChange(header, input) );
			break;
		case BGP4MP_STATE_CHANGE_AS4:
			LOG4CXX_INFO(Logger,"MRTBgp4MPStateChange");

			msg = MRTMessagePtr( new MRTBgp4MPStateChangeAS4(header, input) );
			break;
		case BGP4MP_MESSAGE:
			LOG4CXX_INFO(Logger,"MRTBgp4MPMessage");

			msg = MRTMessagePtr( new MRTBgp4MPMessage(header, input) );
			break;
		case BGP4MP_MESSAGE_AS4:
			LOG4CXX_INFO(Logger,"MRTBgp4MPMessage");

			msg = MRTMessagePtr( new MRTBgp4MPMessageAS4(header, input) );
			break;

		case BGP4MP_ENTRY:
			//PRINT_DBG("  Case BGP4MP_ENTRY");
			//msg = new MRTBgp4MPEntry(ptr);		/* not supported yet */
			//break;
		case BGP4MP_SNAPSHOT:
			//PRINT_DBG("  Case BGP4MP_SNAPSHOT");
			//msg = new MRTBgp4MPSnapshot(ptr);		/* not supported yet */
			//break;
		default:
			LOG4CXX_ERROR( Logger, "unrecognized subtype ["<< header.getSubType() <<"] for bgp4mp." );

			io::detail::skip( input, header.getLength(), boost::mpl::false_() );
			break;
		}
	}
//	else if( (header.getType( ) == TABLE_DUMP) ||
//			 (header.getType( ) == TABLE_DUMP_V2) )
//	{
//		switch( header.getType( ) )
//		{
//		case TABLE_DUMP:
//			msg = new MRTTblDump( ptr );
//			LOG4CXX_INFO(Logger,"MRTTblDump");
//
//			break;
//		case TABLE_DUMP_V2:
//			switch( header.getSubType( ) )
//			{
//			case PEER_INDEX_TABLE:
//				LOG4CXX_INFO(Logger,"MRTTblDumpV2PeerIndexTbl");
//
//				msg = new MRTTblDumpV2PeerIndexTbl( ptr );
//				MRTTblDumpV2RibHeader::setPeerIndexTbl(
//						(MRTTblDumpV2PeerIndexTbl*)msg );
//				break;
//			case RIB_IPV4_UNICAST:
//				LOG4CXX_INFO(Logger,"MRTTblDumpV2RibIPv4Unicast");
//
//				msg = new MRTTblDumpV2RibIPv4Unicast( ptr );
//				break;
//			case RIB_IPV4_MULTICAST:
//				LOG4CXX_INFO(Logger,"MRTTblDumpV2RibIPv4Multicast");
//
//				msg = new MRTTblDumpV2RibIPv4Multicast( ptr );
//				break;
//			case RIB_IPV6_UNICAST:
//				LOG4CXX_INFO(Logger,"MRTTblDumpV2RibIPv6Unicast");
//
//				msg = new MRTTblDumpV2RibIPv6Unicast( ptr );
//				break;
//			case RIB_IPV6_MULTICAST:
//				LOG4CXX_INFO(Logger,"MRTTblDumpV2RibIPv6Multicast");
//
//				msg = new MRTTblDumpV2RibIPv6Multicast( ptr );
//				break;
//			case RIB_GENERIC:
//				LOG4CXX_INFO(Logger,"MRTTblDumpV2RibGeneric");
//
//				msg = new MRTTblDumpV2RibGeneric( ptr );
//				break;
//			default:
//				LOG4CXX_ERROR( Logger, str(format("unrecognized subtype [%u] for table-dump-v2.") % header.getSubType()) );
//
//				/* if failed to parse packet then update packet pointer */
//				ptr += sizeof(MRTCommonHeaderPacket) + header.getLength( );
//				break;
//			}
//			break;
//		default:
//			LOG4CXX_ERROR( Logger, str(format("unrecognized type [%u] for table-dump-v2") % header.getType()) );
//
//			/* if failed to parse packet then update packet pointer */
//			*ptr += sizeof(MRTCommonHeaderPacket) + header.getLength( );
//			break;
//		}
//	}
	else
	{
		io::detail::skip( input, header.getLength( ), boost::mpl::false_( ) );
	}

	LOG4CXX_TRACE( Logger, "<== MRTCommonHeader::newMessage( istream &input )" );
	return msg;
}
