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
#include "BGPUpdate.h"
#include "AttributeTypeASPath.h"
#include "AttributeTypeAS4Path.h"
#include "AttributeTypeAggregator.h"
#include "AttributeTypeAS4Aggregator.h"

BGPUpdate::BGPUpdate(uint8_t** msg, bool isAS4, uint16_t maxLen)
		 : BGPCommonHeader(*msg) {
	uint8_t* ptr = *msg + MESSAGE_HEADER_SIZE; // 19
	uint32_t one = (uint32_t)maxLen;
	uint32_t two = (uint32_t)getLength();
	uint8_t* maxptr = *msg;
	// use whichever smaller (mrt length vs bgp length) as the end-of-msg indicator
	if( one > two ) { maxptr += two; } else { maxptr += one; }
	withdrawnRoutesLength = pathAttributesLength = nlriLength = 0;   
	
	withdrawnRoutes = new list<Route>();
	pathAttributes = new list<BGPAttribute>();
	announcedRoutes = new list<Route>();
	
	memcpy(&withdrawnRoutesLength, ptr, sizeof(withdrawnRoutesLength));
	withdrawnRoutesLength = ntohs(withdrawnRoutesLength);
	ptr += sizeof(withdrawnRoutesLength);
	uint8_t* p = ptr;
	
	while (p < ptr + withdrawnRoutesLength && p <= maxptr) {
		uint8_t prefixLen = *p;
		if( prefixLen > sizeof(IPAddress)*8 ) {
			*msg = maxptr;
			Logger::err("abnormal prefix-length [%d]. skip this record.", prefixLen );
			return;
		}
		Route wRoute(prefixLen, ++p);
		if( p+wRoute.getNumOctets() > maxptr ) {
			*msg = maxptr;
			Logger::err("message truncated! need to read [%d], but only have [%d] bytes.",
						 wRoute.getNumOctets(), maxptr-p);
			return;
		}
		withdrawnRoutes->push_back(wRoute);
		p += wRoute.getNumOctets(); 
	}
	
	ptr += withdrawnRoutesLength;
	
	memcpy(&pathAttributesLength, ptr, sizeof(pathAttributesLength));
	ptr += sizeof(pathAttributesLength);
	pathAttributesLength = ntohs(pathAttributesLength);
	p = ptr;
	
	while (p < ptr + pathAttributesLength && p <= maxptr)
	{
		BGPAttribute attrib(p,isAS4,maxptr);
		if( p+attrib.totalSize() > maxptr ) {
			/* error message is already diplayed by BGPAttribute object */
			*msg = maxptr;
			return;
		}
		if( attrib.getAttributeValue() == NULL ) {
			Logger::err("malformed attribute. skip.");
			return;
		}
		pathAttributes->push_back(attrib);
		p += attrib.totalSize();
	}
	ptr += pathAttributesLength;

	// Post processing
	// 1. Merge AS_PATH and AS4_PATH
        list<BGPAttribute>::iterator attrIter;
	AttributeTypeASPath*  as_path_attr  = NULL;
	AttributeTypeAS4Path* as4_path_attr = NULL;
        for (attrIter = pathAttributes->begin(); attrIter != pathAttributes->end(); attrIter++)
        {
            if (attrIter->getAttributeTypeCode() == AttributeType::AS_PATH)     { as_path_attr  = (AttributeTypeASPath*)attrIter->getAttributeValueMutable();  }
            if (attrIter->getAttributeTypeCode() == AttributeType::NEW_AS_PATH) { as4_path_attr = (AttributeTypeAS4Path*)attrIter->getAttributeValueMutable(); }
        }
	if (as_path_attr != NULL)
	{
		as_path_attr->genPathSegmentsComplete(as4_path_attr);
	}
	// 2. Merge AGGREGATOR and AS4_AGGREGATOR
	AttributeTypeAggregator*    agg_attr     = NULL;
	AttributeTypeAS4Aggregator* as4_agg_attr = NULL;
        for (attrIter = pathAttributes->begin(); attrIter != pathAttributes->end(); attrIter++)
        {
            if (attrIter->getAttributeTypeCode() == AttributeType::AGGREGATOR)     { agg_attr     = (AttributeTypeAggregator*)attrIter->getAttributeValueMutable();    }
            if (attrIter->getAttributeTypeCode() == AttributeType::NEW_AGGREGATOR) { as4_agg_attr = (AttributeTypeAS4Aggregator*)attrIter->getAttributeValueMutable(); }
        }
	if (agg_attr != NULL)
	{
		uint32_t as = (as4_agg_attr != NULL) ? as4_agg_attr->getAggregatorLastAS() : agg_attr->getAggregatorLastAS();
		agg_attr->setAggregatorLastASComplete(as);
	}
	
	// This information is not part of the BGP message.
	nlriLength = length 
				 - MESSAGE_HEADER_SIZE
				 - sizeof(withdrawnRoutesLength) 
				 - sizeof(pathAttributesLength) 
				 - withdrawnRoutesLength 
				 - pathAttributesLength;
	//DONE: Get NRLI information
	p = ptr;
	while (p < ptr + nlriLength && p <= maxptr) {
		uint8_t prefixLen; // = BITMASK_8 & *p;
		memcpy(&prefixLen, p, sizeof(uint8_t));
		if( prefixLen > sizeof(IPAddress)*8 ) {
			*msg = maxptr;
			Logger::err("abnormal prefix-length [%d]. skip this record.", prefixLen );
			return;
		}
		Route aRoute(prefixLen, ++p);
		if( p+aRoute.getNumOctets() > maxptr ) {
			nlriLength = maxptr - ptr;
			*msg = maxptr;
			Logger::err("message truncated! need to read [%d], but only have [%d] bytes.",
						 aRoute.getNumOctets(), maxptr-p);
			return;
		}
		announcedRoutes->push_back(aRoute);
		p += aRoute.getNumOctets(); 
		IPAddress ipaddr = aRoute.getPrefix();
	}
	ptr += nlriLength;
	
	// Set *msg to the value reached by ptr.
	*msg = ptr;
}

BGPUpdate::BGPUpdate(bool isAS4)
		 : BGPCommonHeader() {
	setLength(0);
	setType((uint8_t)BGPCommonHeader::UPDATE);

	withdrawnRoutes = new list<Route>();
	pathAttributes  = new list<BGPAttribute>();
	announcedRoutes = new list<Route>();
}

BGPUpdate::~BGPUpdate() { 
	delete withdrawnRoutes; withdrawnRoutes = NULL;
	delete pathAttributes;  pathAttributes  = NULL;
	delete announcedRoutes; announcedRoutes = NULL;
}

void BGPUpdate::printMe() {
	list<Route>::iterator routeIter;

	if (withdrawnRoutes->size() > 0) {
		cout << "WITHDRAWN:" << endl;
		for (routeIter = withdrawnRoutes->begin(); routeIter != withdrawnRoutes->end(); routeIter++) {
			Route rt = *routeIter;
			cout << "  ";
			rt.printMe();
			cout << endl;
		}
	}
	
	list<BGPAttribute>* pathAttrs = this->getPathAttributes();
	list<BGPAttribute>::iterator attrIter;
	for (attrIter = pathAttrs->begin(); attrIter != pathAttrs->end(); attrIter++) {
		BGPAttribute attr = *attrIter;
		attr.printMe();
		cout << endl;
	}
	
	if (announcedRoutes->size() > 0) {
		cout << "ANNOUNCED:" << endl;
		for( routeIter = announcedRoutes->begin(); 
			 routeIter != announcedRoutes->end(); 
			 routeIter++) {
			Route rt = *routeIter;
			cout << "  ";
			rt.printMe();
			cout << endl;
		}
	}
}

void BGPUpdate::printMeCompact() {
	list<Route>::iterator routeIter;
	bool isFirstLoop = true;
	cout << "WITH_CNT: " << withdrawnRoutes->size() << "|";
	for (routeIter = withdrawnRoutes->begin(); routeIter != withdrawnRoutes->end(); routeIter++) {
		Route rt = *routeIter;
		isFirstLoop ? isFirstLoop = false : cout << " ";
		rt.printMe();
	}
	cout << "|";
	
	list<BGPAttribute>* pathAttrs = this->getPathAttributes();
	cout << "ATTR_CNT: " << pathAttrs->size() << "|";

	list<BGPAttribute>::iterator attrIter;
	for (attrIter = pathAttrs->begin(); attrIter != pathAttrs->end(); attrIter++) {
		BGPAttribute attr = *attrIter;
		attr.printMeCompact();
		cout << "|";
	}
	
	isFirstLoop = true;
	cout << "ANN_CNT: " << announcedRoutes->size() << "|";
	for (routeIter = announcedRoutes->begin(); routeIter != announcedRoutes->end(); routeIter++) {
		Route rt = *routeIter;
		isFirstLoop ? isFirstLoop = false : cout << " ";
		rt.printMe();
	}
	cout << "|";
}




