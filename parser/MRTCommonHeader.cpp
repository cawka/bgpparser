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
#include "MRTBgp4MPStateChangeAS4.h"
#include "MRTBgp4MPMessage.h"
#include "MRTBgp4MPMessageAS4.h"

#include "MRTTblDump.h"
#include "MRTTblDumpV2PeerIndexTbl.h"
#include "MRTTblDumpV2RibHeader.h"
#include "MRTTblDumpV2RibIPv4Unicast.h"
#include "MRTTblDumpV2RibIPv4Multicast.h"
#include "MRTTblDumpV2RibIPv6Unicast.h"
#include "MRTTblDumpV2RibIPv6Multicast.h"
#include "MRTTblDumpV2RibGeneric.h"

#include "BGPCommonHeader.h"

using namespace std;
namespace io = boost::iostreams;

log4cxx::LoggerPtr MRTCommonHeader::Logger = log4cxx::Logger::getLogger( "bgpparser.MRTCommonHeader" );

MRTCommonHeader::MRTCommonHeader(void) {
	/* nothing */
	throw std::exception( );
}

MRTCommonHeader::MRTCommonHeader( istream &input )
{
	int len=io::read( input, reinterpret_cast<char*>(&pkt), sizeof(MRTCommonHeaderPacket) );
	if( len<0 ) throw EOFException( );
	if( len!=sizeof(MRTCommonHeaderPacket) ) throw MRTException( "MRT format error" );

	timestamp = ntohl(pkt.timestamp);
	type = ntohs(pkt.type);
	subtype = ntohs(pkt.subtype);
	length = ntohl(pkt.length);

	if( length==0 ) return;

	data=boost::shared_ptr<char>( new char[length] );
	len=io::read( input, data.get(), length );
	if( len!=length )
	{
		LOG4CXX_ERROR(Logger, "MRT length field = " << (int)length << ", but could read only " << (int)len << " bytes" );
		throw MRTException( "MRT read error" );
	}
}

MRTCommonHeader::~MRTCommonHeader(void) {
	/* nothing */
}

/* 
 * Factory method for creating MRTMessages
 */ 
MRTMessagePtr MRTCommonHeader::newMessage( istream &input ) {
	LOG4CXX_TRACE( Logger, "==> MRTCommonHeader::newMessage( istream &input )" );

	MRTMessagePtr msg;
	MRTMessagePtr header( new MRTCommonHeader(input) );
	msg=header;
	
	LOG4CXX_DEBUG( Logger, "Type:      " << (int)header->getType() );
	LOG4CXX_DEBUG( Logger, "SubType:   " << (int)header->getSubType() );
	LOG4CXX_DEBUG( Logger, "Length:    " << (int)header->getLength() );

	io::stream<io::array_source> in( header->data.get(), header->length );

	/* only accept BGP4MP message and TABLE_DUMP message types */
	if ((header->getType() != BGP4MP) && /*(header.getType() != BGP4MP_ET) && */
		(header->getType() != TABLE_DUMP) && (header->getType() != TABLE_DUMP_V2) )
	{
		LOG4CXX_INFO(Logger,"Nonsupported MRT type ["<<(int)header->getType()<<"]");
//		return msg;
	}


	MRTTblDumpV2PeerIndexTblPtr tbldumpv2_indextbl;

	try
	{
		if( header->getType( ) == BGP4MP )
		{
			switch( header->getSubType( ) )
			{
			case BGP4MP_STATE_CHANGE:
				LOG4CXX_TRACE(Logger,"MRTBgp4MPStateChange");
	
				msg = MRTMessagePtr( new MRTBgp4MPStateChange(*header, in) );
				break;
			case BGP4MP_STATE_CHANGE_AS4:
				LOG4CXX_TRACE(Logger,"MRTBgp4MPStateChange");
	
				msg = MRTMessagePtr( new MRTBgp4MPStateChangeAS4(*header, in) );
				break;
			case BGP4MP_MESSAGE:
				LOG4CXX_TRACE(Logger,"MRTBgp4MPMessage");
	
				msg = MRTMessagePtr( new MRTBgp4MPMessage(*header, in) );
				break;
			case BGP4MP_MESSAGE_AS4:
				LOG4CXX_TRACE(Logger,"MRTBgp4MPMessage");
	
				msg = MRTMessagePtr( new MRTBgp4MPMessageAS4(*header, in) );
				break;
	
			case BGP4MP_ENTRY:
				/* deprecated */
				msg=header;
				break;
			case BGP4MP_SNAPSHOT:
				/* deprecated */
				msg=header;
				break;
			default:
				LOG4CXX_ERROR( Logger, "unrecognized subtype ["<< (int)header->getSubType() <<"] for bgp4mp." );
				msg=header;
				break;
			}
		}
		else if( (header->getType( ) == TABLE_DUMP) ||
				 (header->getType( ) == TABLE_DUMP_V2) )
		{
			switch( header->getType( ) )
			{
			case TABLE_DUMP:
				msg = MRTMessagePtr( new MRTTblDump(*header,in) );
				LOG4CXX_TRACE(Logger,"MRTTblDump");
	
				break;
			case TABLE_DUMP_V2:
				switch( header->getSubType( ) )
				{
				case PEER_INDEX_TABLE:
				{
					LOG4CXX_TRACE(Logger,"MRTTblDumpV2PeerIndexTbl");
	
					tbldumpv2_indextbl = MRTTblDumpV2PeerIndexTblPtr( new MRTTblDumpV2PeerIndexTbl(*header, in) );
					msg = MRTMessagePtr( tbldumpv2_indextbl );
					break;
				}
				case RIB_IPV4_UNICAST:
				{
					LOG4CXX_TRACE(Logger,"MRTTblDumpV2RibIPv4Unicast");
	
					msg = MRTMessagePtr( new MRTTblDumpV2RibIPv4Unicast(tbldumpv2_indextbl, *header, in) );
					break;
				}
				case RIB_IPV4_MULTICAST:
				{
					LOG4CXX_TRACE(Logger,"MRTTblDumpV2RibIPv4Multicast");
	
					msg = MRTMessagePtr( new MRTTblDumpV2RibIPv4Multicast(tbldumpv2_indextbl, *header, in) );
					break;
				}
				case RIB_IPV6_UNICAST:
				{
					LOG4CXX_TRACE(Logger,"MRTTblDumpV2RibIPv6Unicast");
	
					msg = MRTMessagePtr( new MRTTblDumpV2RibIPv6Unicast(tbldumpv2_indextbl, *header, in) );
					break;
				}
				case RIB_IPV6_MULTICAST:
				{
					LOG4CXX_TRACE(Logger,"MRTTblDumpV2RibIPv6Multicast");
	
					msg = MRTMessagePtr( new MRTTblDumpV2RibIPv6Multicast(tbldumpv2_indextbl, *header, in) );
					break;
				}
				case RIB_GENERIC:
				{
					LOG4CXX_TRACE(Logger,"MRTTblDumpV2RibGeneric");
	
					msg = MRTMessagePtr( new MRTTblDumpV2RibGeneric(tbldumpv2_indextbl, *header, in) );
					break;
				}
				default:
					LOG4CXX_ERROR( Logger, "unrecognized subtype ["<< (int)header->getSubType() <<"] for table-dump-v2." );
	
					break;
				}
				break;
			default:
				LOG4CXX_ERROR( Logger, "unrecognized type ["<< (int)header->getType() <<"] for table-dump-v2" );
	
				break;
			}
		}
	}
	catch( std::istream::failure &e )
	{
		LOG4CXX_ERROR( Logger, "Error parsing MRT message (stream error). Setting type to MRT_INVALID" );
		header->type=MRT_INVALID;
		msg=header;
	}
	catch( BGPTextError &e )
	{
		LOG4CXX_ERROR( Logger, "Error parsing MRT message ("<< e.what() <<"). Setting type to MRT_INVALID" );
		header->type=MRT_INVALID;
		msg=header;
	}
	catch( BGPError &e )
	{
		LOG4CXX_ERROR( Logger, "Error parsing MRT message (format error). Setting type to MRT_INVALID" );
		header->type=MRT_INVALID;
		msg=header;
	}

	LOG4CXX_TRACE( Logger, "<== MRTCommonHeader::newMessage( istream &input )" );
	return msg;
}
