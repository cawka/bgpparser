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

#include "AttributeTypeASPath.h"
#include "AttributeTypeAS4Path.h"

using namespace std;
namespace io = boost::iostreams;

log4cxx::LoggerPtr AttributeTypeASPath::Logger = log4cxx::Logger::getLogger( "bgpparser.AttributeTypeASPath" );
log4cxx::LoggerPtr AttributeTypeASPathSegment::Logger = log4cxx::Logger::getLogger( "bgpparser.AttributeTypeASPathSegment" );

////////////////////////////////////////////////////////////////////
// AttributeTypeASPathSegment
//

AttributeTypeASPathSegment::AttributeTypeASPathSegment( istream &input, bool is4byte )
{
    bool error=false;
    error|= sizeof(uint8_t)!=
			io::read( input, reinterpret_cast<char*>(&pathSegmentType),   sizeof(uint8_t) );
    error|= sizeof(uint8_t)!=
			io::read( input, reinterpret_cast<char*>(&pathSegmentLength), sizeof(uint8_t) );

    if( error )
    {
        LOG4CXX_ERROR(Logger, "Parsing error");
        throw BGPError( );
    }

	for( int i = 0; i < pathSegmentLength; i++ )
	{
		uint32_t segVal;
		if( is4byte )
		{
			int len=io::read( input, reinterpret_cast<char*>(&segVal), sizeof(uint32_t) );
			if( len!=sizeof(uint32_t) ) 
            {
                LOG4CXX_ERROR(Logger, "4-byte AS sement requested, read " << len << "bytes");
                throw BGPError( );
            }
		}
		else
		{
			int len=io::read( input, reinterpret_cast<char*>(&segVal), sizeof(uint16_t) );
			if( len!=sizeof(uint16_t) ) 
            {
                LOG4CXX_ERROR(Logger, "2-byte AS sement requested, read " << len << "bytes");
                throw BGPError( );
            }
		}

		segVal = is4byte ? ntohl(segVal) : ntohs(segVal);
		pathSegmentValue.push_back( segVal );
	}
}

AttributeTypeASPathSegment::~AttributeTypeASPathSegment(void)
{
}

//void AttributeTypeASPathSegment::printMe() {
//	switch (pathSegmentType) {
//		case AS_SEQUENCE: break;
//		case AS_SET: cout << " {";
//	}
//
//	list<uint32_t>::iterator it;
//	for (it = pathSegmentValue.begin(); it != pathSegmentValue.end(); it++) {
//		cout << " " << *it;
//	}
//
//	switch (pathSegmentType) {
//		case AS_SEQUENCE: break;
//		case AS_SET: cout << "}";
//	}
//}
//
//void AttributeTypeASPathSegment::printMeCompact()
//{
//	uint16_t top, bottom;
//	list<uint32_t>::iterator it;
//	switch (pathSegmentType) {
//		case AS_SEQUENCE: {
//			cout << " ";
//			bool isFirst = true;
//			for (it = pathSegmentValue.begin(); it != pathSegmentValue.end(); it++) {
//				cout << (isFirst ? "" : " ");
//				top = (uint16_t)(((*it)>>16)&0xFFFF);
//				bottom = (uint16_t)((*it)&0xFFFF);
//				if( top == 0 ) {
//					printf("%u", bottom);
//				} else {
//					printf("%u.%u", top, bottom);
//				}
//				isFirst = false;
//			}
//			break;
//		}
//		case AS_SET: {
//			cout << " {";
//			bool isFirst = true;
//			for (it = pathSegmentValue.begin(); it != pathSegmentValue.end(); it++) {
//				cout << (isFirst ? "" : ",");
//				top = (uint16_t)(((*it)>>16)&0xFFFF);
//				bottom = (uint16_t)((*it)&0xFFFF);
//				if( top == 0 ) {
//					printf("%u", bottom);
//				} else {
//					printf("%u.%u", top, bottom);
//				}
//				isFirst = false;
//			}
//			cout << "}";
//			break;
//		}
//	}
//}

//////////////////////////////////////////////////////////////////////////////////////////
// AttributeTypeASPath
//

AttributeTypeASPath::AttributeTypeASPath( AttributeType &header, istream &input )
: AttributeType(header)
{
	LOG4CXX_TRACE(Logger,"" );
//	// TODO: Add a routine to check is4byte in more reliable way
//	// check is4byte here
//	bool is4byte = false;
//	if (len != 0) {
//		uint16_t firstTwoBytes = 0;
//		firstTwoBytes = *((uint16_t*)(msg+2)) & BITMASK_16;
//		is4byte = firstTwoBytes == 0;
//	}

	// @TODO: test if without "autodetection" this may cause troubles

	while( input.peek()!=-1 )
	{
		pathSegments.push_back( AttributeTypeASPathSegmentPtr(new AttributeTypeASPathSegment( input,isAS4 )) );
	}

	LOG4CXX_TRACE(Logger,"segments = " << pathSegments.size() );
}

AttributeTypeASPath::~AttributeTypeASPath(void) {
}


//void AttributeTypeASPath::printMe()
//{
//	cout << "ASPATH :";
//	for (list<AttributeTypeASPathSegmentPtr>::iterator it = pathSegments.begin();
//			it != pathSegments.end(); it++) {
//		(*it)->printMe();
//	}
//
//	cout << endl;
//	cout << "ASPATH':";
//	for (list<AttributeTypeASPathSegmentPtr>::iterator it = pathSegmentsComplete.begin();
//			it != pathSegmentsComplete.end(); it++)
//	{
//		(*it)->printMe();
//	}
//}
//
//void AttributeTypeASPath::printMeCompact()
//{
//	cout << "AS_SEQUENCE: ";
//	list<AttributeTypeASPathSegmentPtr>::iterator it;
//
//	for (it = pathSegments.begin(); it != pathSegments.end(); it++) {
//		(*it)->printMeCompact();
//	}
//}

void AttributeTypeASPath::genPathSegmentsComplete( const AttributeTypeAS4Path &as4_path )
{
	LOG4CXX_TRACE(Logger,"genPathSegmentsComplete");
	if( as4_path.getPathSegments().size()==0 )
	{
		LOG4CXX_TRACE(Logger,"pathSegmentsComplete = pathSegments");
		pathSegmentsComplete = pathSegments;
		return;
	}

	if( getCountOfASNs()<as4_path.getCountOfASNs() )
	{
		LOG4CXX_WARN(Logger,"Count of ASNs in AS_PATH (" << getCountOfASNs()
				<<") --- less than in AS4_PATH ("<<as4_path.getCountOfASNs()<<")");
	}


	const list<AttributeTypeASPathSegmentPtr> &as_path_segs  = getPathSegments( );
	const list<AttributeTypeASPathSegmentPtr> &as4_path_segs = as4_path.getPathSegments( );

	list<AttributeTypeASPathSegmentPtr>::const_reverse_iterator  it  = as_path_segs.rbegin();
	list<AttributeTypeASPathSegmentPtr>::const_reverse_iterator  it4 = as4_path_segs.rbegin();

	while( it != as_path_segs.rend() && it4 != as4_path_segs.rend() )
	{
		LOG4CXX_TRACE(Logger,"next segment");

		// Merge as_path and as4_path
		AttributeTypeASPathSegmentPtr pathSegment( 
			new AttributeTypeASPathSegment((*it)->getPathSegmentType( )) );

		const list<uint32_t> &as_seg_value  = (*it)->getPathSegmentValue();
		const list<uint32_t> &as4_seg_value = (*it4)->getPathSegmentValue();

		list<uint32_t>::const_iterator it_seg  = as_seg_value.begin();
		list<uint32_t>::const_iterator it_seg4 = as4_seg_value.begin();

		int size_as_seg  = as_seg_value.size();
		int size_as4_seg = as4_seg_value.size();
		int from_as_seg  = size_as_seg  - size_as4_seg;

		for( int idx=1; idx<=size_as_seg; idx++ )
		{
			if (idx <= from_as_seg)
			{
				pathSegment->add( *it_seg );
				it_seg++;
			}
			else
			{
				pathSegment->add( *it_seg4 );
				it_seg4++;
			}
		}
		pathSegmentsComplete.push_front( pathSegment );
		it++;
		it4++;
	}

	while( it != as_path_segs.rend() )
	{
		// Copy remainings of the as_path
		pathSegmentsComplete.push_front( *it );
		it++;
	}

#if 0
	// [TODO] AS4_PATH is longer than AS_PATH
	// Raise an error here
	while (it4 != as4_path_segs->rend())
	{
		// Copy from as4_path
		AttributeTypeASPathSegment pathSegment(it->getPathSegmentType(), it->getPathSegmentLength());

		AttributeTypeAS4PathSegment* as4_seg = &(*it4);
		list<uint32_t> *pathSegmentValue = as4_seg->getPathSegmentValue();
		if (pathSegmentValue != NULL)
		{
			list<uint32_t>::iterator it;
			for (it = pathSegmentValue->begin(); it != pathSegmentValue->end(); it++)
			{
				pathSegment.pathSegmentValue->push_back(*it);
			}
		}
		pathSegmentsComplete->push_front(pathSegment);
		it4++;
	}
#else
	/* if there is an inconsistency, we do not process this update */
	if( it4 != as4_path_segs.rend() )
	{
		/* currently we do not have an indicator for errors.  Using NULL value to indicate error... */
		//error = 1;
	}
#endif
}

uint32_t AttributeTypeASPath::getCountOfASNs( ) const
{
	uint32_t count=0;
	BOOST_FOREACH( const AttributeTypeASPathSegmentPtr &segment, pathSegments )
	{
		if( segment->getPathSegmentType()==AttributeTypeASPathSegment::AS_SET ||
			segment->getPathSegmentType()==AttributeTypeASPathSegment::AS_CONFED_SET )
		{
			count++;
		}
		else
			count+=segment->getPathSegmentValue().size();
	}
	
	return count;
}

// vim: sw=4 ts=4 sts=4 expandtab
