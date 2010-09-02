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
// Modified: Jonathan Park (jpark@cs.ucla.edu)
#include <bgpparser.h>

#include "BGPUpdate.h"
#include "AttributeTypeASPath.h"
#include "AttributeTypeAS4Path.h"
#include "AttributeTypeAggregator.h"
#include "AttributeTypeAS4Aggregator.h"
#include "Exceptions.h"
using namespace std;

#include <boost/iostreams/read.hpp>
#include <boost/iostreams/skip.hpp>
namespace io = boost::iostreams;

log4cxx::LoggerPtr BGPUpdate::Logger = log4cxx::Logger::getLogger( "bgpparser.BGPUpdate" );

/*protected*/BGPUpdate::BGPUpdate( )
		: BGPCommonHeader( BGPCommonHeader::UPDATE )
{
	withdrawnRoutesLength=0;
	pathAttributesLength=0;
	nlriLength=0;
}

BGPUpdate::BGPUpdate(BGPCommonHeader &header, istream &input, bool isAS4 )
		 : BGPCommonHeader(header)
{
	withdrawnRoutesLength = pathAttributesLength = nlriLength = 0;   
	
	io::read( input, reinterpret_cast<char*>(&withdrawnRoutesLength), sizeof(uint16_t) );
	withdrawnRoutesLength = ntohs(withdrawnRoutesLength);

	uint16_t left=withdrawnRoutesLength;
	while( left>0 )
	{
		uint8_t prefixLen = input.get( );

		if( prefixLen > sizeof(IPAddress) * 8 )
		{
			LOG4CXX_ERROR(Logger,"abnormal prefix-length ["<< (int)prefixLen <<"]. skip this record." );
			throw BGPError( );
		}

		boost::shared_ptr<Route> wRoute=boost::shared_ptr<Route>( new Route( prefixLen, input ) );
		withdrawnRoutes.push_back( wRoute );

		left -= 1+wRoute->getNumOctets( );
	}
	
	io::read( input, reinterpret_cast<char*>(&pathAttributesLength), sizeof(pathAttributesLength) );
	pathAttributesLength = ntohs(pathAttributesLength);

	left=pathAttributesLength;
	while( left>0 )
	{
		boost::shared_ptr<BGPAttribute> attrib = boost::shared_ptr<BGPAttribute>( new BGPAttribute(input, isAS4) );

		if( attrib->getAttributeValue( ).get() == NULL )
		{
			LOG4CXX_ERROR(Logger,"malformed attribute. skip.");
			throw BGPError( );
		}
		pathAttributes.push_back( attrib );

		left-=attrib->totalSize( );
	}

	// Post processing
	// 1. Merge AS_PATH and AS4_PATH

	std::list<BGPAttributePtr>::iterator as_path=
		find_if( pathAttributes.begin(), pathAttributes.end(), findByType(AttributeType::AS_PATH) );

	std::list<BGPAttributePtr>::iterator as4_path=
		find_if( pathAttributes.begin(), pathAttributes.end(), findByType(AttributeType::NEW_AS_PATH) );

	if( as_path!=pathAttributes.end() && as4_path!=pathAttributes.end() )
	{
		AttributeTypeASPathPtr  as=
				boost::dynamic_pointer_cast<AttributeTypeASPath>( (*as_path)->getAttributeValueMutable() );
		AttributeTypeAS4PathPtr as4=
				boost::dynamic_pointer_cast<AttributeTypeAS4Path>( (*as4_path)->getAttributeValue() );

		as->genPathSegmentsComplete( *as4 );
	}
	else if( as_path!=pathAttributes.end() )
		boost::dynamic_pointer_cast<AttributeTypeASPath>( (*as_path)->getAttributeValueMutable() )
			->genPathSegmentsComplete( );

	// 2. Merge AGGREGATOR and AS4_AGGREGATOR
	std::list<BGPAttributePtr>::iterator agg2=
			find_if( pathAttributes.begin(), pathAttributes.end(), findByType(AttributeType::AGGREGATOR) );

	std::list<BGPAttributePtr>::iterator agg4=
			find_if( pathAttributes.begin(), pathAttributes.end(), findByType(AttributeType::NEW_AGGREGATOR) );

	if( agg2!=pathAttributes.end() && agg4!=pathAttributes.end() )
	{
		AttributeTypeAggregatorPtr    agg_attr     =
				boost::dynamic_pointer_cast<AttributeTypeAggregator>( (*agg2)->getAttributeValueMutable() );

		AttributeTypeAS4AggregatorPtr as4_agg_attr =
				boost::dynamic_pointer_cast<AttributeTypeAS4Aggregator>( (*agg4)->getAttributeValueMutable() );

		agg_attr->setAggregatorLastASComplete( as4_agg_attr->getAggregatorLastAS() );
	}
	else if( agg2!=pathAttributes.end() )
	{
		boost::dynamic_pointer_cast<AttributeTypeAggregator>( (*agg2)->getAttributeValueMutable() )
			->setAggregatorLastASComplete( );
	}


	// This information is not part of the BGP message.
	nlriLength = length 
				 - MESSAGE_HEADER_SIZE
				 - sizeof(withdrawnRoutesLength) 
				 - sizeof(pathAttributesLength) 
				 - withdrawnRoutesLength 
				 - pathAttributesLength;
	//DONE: Get NRLI information
	left=nlriLength;
	while( left>0 )
	{
		uint8_t prefixLen = input.get( );

		if( prefixLen > sizeof(IPAddress) * 8 )
		{
			LOG4CXX_ERROR(Logger,"abnormal prefix-length ["<< (int)prefixLen <<"]. skip this record." );
			throw BGPError( );
		}

		boost::shared_ptr<Route> aRoute=boost::shared_ptr<Route>( new Route( prefixLen, input ) );
		announcedRoutes.push_back( aRoute );

		left -= 1+aRoute->getNumOctets( );
	}
}

//BGPUpdate::BGPUpdate(bool isAS4)
//		 : BGPCommonHeader() {
//	setLength(0);
//	setType((uint8_t)BGPCommonHeader::UPDATE);
//
//	withdrawnRoutes = new list<Route>();
//	pathAttributes  = new list<BGPAttribute>();
//	announcedRoutes = new list<Route>();
//}

BGPUpdate::~BGPUpdate() { 
}

void BGPUpdate::printMe()
{
	if (withdrawnRoutes.size() > 0)
	{
		cout << "WITHDRAWN:" << endl;
		for( std::list<RoutePtr>::iterator routeIter = withdrawnRoutes.begin( );
				routeIter != withdrawnRoutes.end( ); routeIter++ )
		{
			cout << "  ";
			(*routeIter)->printMe( );
			cout << endl;
		}
	}
	
	for( std::list<BGPAttributePtr>::iterator attrIter = pathAttributes.begin( );
			attrIter != pathAttributes.end( ); attrIter++ )
	{
		(*attrIter)->printMe( );
		cout << endl;
	}
	
	if( announcedRoutes.size( ) > 0 )
	{
		cout << "ANNOUNCED:" << endl;
		for( std::list<RoutePtr>::iterator routeIter = announcedRoutes.begin( );
				routeIter != announcedRoutes.end( ); routeIter++ )
		{
			cout << "  ";
			(*routeIter)->printMe( );
			cout << endl;
		}
	}
}

void BGPUpdate::printMeCompact()
{
	bool isFirstLoop = true;
	cout << "WITH_CNT: " << withdrawnRoutes.size() << "|";
	for( std::list<RoutePtr>::iterator routeIter = withdrawnRoutes.begin( );
					routeIter != withdrawnRoutes.end( ); routeIter++ )
	{
		isFirstLoop ? isFirstLoop = false : cout << " ";
		(*routeIter)->printMe( );
	}
	cout << "|";
	
	cout << "ATTR_CNT: " << pathAttributes.size() << "|";

	for( std::list<BGPAttributePtr>::iterator attrIter = pathAttributes.begin( );
				attrIter != pathAttributes.end( ); attrIter++ )
	{
		(*attrIter)->printMeCompact();
		cout << "|";
	}
	
	isFirstLoop = true;
	cout << "ANN_CNT: " << announcedRoutes.size() << "|";
	for( std::list<RoutePtr>::iterator routeIter = announcedRoutes.begin( );
					routeIter != announcedRoutes.end( ); routeIter++ )
	{
		isFirstLoop ? isFirstLoop = false : cout << " ";
		(*routeIter)->printMe( );
	}
	cout << "|";
}
