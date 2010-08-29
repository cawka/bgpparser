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

#include "AttributeTypeAS4Path.h"
#include "Exceptions.h"
using namespace std;

#include <boost/iostreams/read.hpp>
#include <boost/foreach.hpp>
namespace io = boost::iostreams;

log4cxx::LoggerPtr AttributeTypeAS4Path::Logger = log4cxx::Logger::getLogger( "bgpparser.AttributeTypeAS4Path" );
log4cxx::LoggerPtr AttributeTypeAS4PathSegment::Logger = log4cxx::Logger::getLogger( "bgpparser.AttributeTypeAS4PathSegment" );

void AttributeTypeAS4PathSegment::printMeCompact()
{
	uint16_t top, bottom;
	list<uint32_t>::iterator it;
	if( pathSegmentType == AS_SEQUENCE ) {
		cout << "AS4_SEQUENCE: ";
	}
	switch (pathSegmentType) {
		case AS_SEQUENCE: {
			cout << " ";
			bool isFirst = true;
			for (it = pathSegmentValue.begin(); it != pathSegmentValue.end(); it++) {
				cout << (isFirst ? "" : " ");
				top = (uint16_t)(((*it)>>16)&0xFFFF);
				bottom = (uint16_t)((*it)&0xFFFF);
				if( top == 0 ) {
					printf("%u", bottom);
				} else {
					printf("%u.%u", top, bottom);
				}
				isFirst = false;
			}
			break;
		}
		case AS_SET: {
			cout << " {";
			bool isFirst = true;
			for (it = pathSegmentValue.begin(); it != pathSegmentValue.end(); it++) {
				cout << (isFirst ? "" : ",");
				top = (uint16_t)(((*it)>>16)&0xFFFF);
				bottom = (uint16_t)((*it)&0xFFFF);
				if( top == 0 ) {
					printf("%u", bottom);
				} else {
					printf("%u.%u", top, bottom);
				}
				isFirst = false;
			}
			cout << "}";
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
// AttributeTypeAS4Path
//
AttributeTypeAS4Path::AttributeTypeAS4Path( AttributeType &header, std::istream &input )
: AttributeType( header )
{
	LOG4CXX_TRACE(Logger,"");
	
	while( input.peek()!=-1 )
	{
		pathSegments.push_back( AttributeTypeAS4PathSegmentPtr(new AttributeTypeAS4PathSegment( input )) );
	}
	LOG4CXX_TRACE(Logger,"segments = " << pathSegments.size() );
}

AttributeTypeAS4Path::~AttributeTypeAS4Path(void)
{
}

//AttributeType* AttributeTypeAS4Path::clone() {
//	AttributeTypeAS4Path *atAS4Path = new AttributeTypeAS4Path();
//	// TODO: copy each element in pathSegments list using pathSegments's clone()
//	list<AttributeTypeAS4PathSegment>::iterator it;
//	for (it = pathSegments->begin(); it != pathSegments->end(); it++) {
//		atAS4Path->setAS4PathSegment((*it));
//	}
//	atAS4Path->setPathSegmentType(getPathSegmentType());
//	atAS4Path->setPathSegmentLength(getPathSegmentLength());
//	return atAS4Path;
//}

void AttributeTypeAS4Path::printMe() 
{ 
	cout << "AS4_PATH:";
	list<AttributeTypeASPathSegmentPtr>::iterator it;

	for (it = pathSegments.begin(); it != pathSegments.end(); it++)
	{
		//cout << endl << "  ";
		(*it)->printMe();
	}
}

void AttributeTypeAS4Path::printMeCompact()
{
	list<AttributeTypeASPathSegmentPtr>::iterator it;

	for (it = pathSegments.begin(); it != pathSegments.end(); it++)
	{
		//cout << endl << "  ";
		(*it)->printMeCompact();
	}
}

uint32_t AttributeTypeAS4Path::getCountOfASNs( ) const
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
