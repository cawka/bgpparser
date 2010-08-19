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
#include "AttributeTypeAS4Path.h"

AttributeTypeAS4PathSegment::AttributeTypeAS4PathSegment(void) {
	this->pathSegmentValue = new list<uint32_t>();
}

AttributeTypeAS4PathSegment::AttributeTypeAS4PathSegment(const AttributeTypeAS4PathSegment& attr)
							: AttributeType(attr.length, attr.value, attr.isAS4) {
	pathSegmentType = attr.pathSegmentType;
	pathSegmentLength = attr.pathSegmentLength;
	this->pathSegmentValue = new list<uint32_t>(attr.pathSegmentValue->begin(),attr.pathSegmentValue->end());
}

AttributeTypeAS4PathSegment::AttributeTypeAS4PathSegment(uint16_t len, uint8_t* msg)
							: AttributeType(len, msg, isAS4) {
	uint8_t *ptr = msg;
	
	pathSegmentValue = new list<uint32_t>();
	pathSegmentType = *ptr++ & BITMASK_8;
	pathSegmentLength = *ptr++ & BITMASK_8;

	for (int i=0; i < pathSegmentLength; i++) {
		uint32_t segVal;
		memcpy(&segVal, (ptr + (i << 2)), sizeof(uint32_t));
		segVal = ntohl(segVal);
		pathSegmentValue->push_back(segVal);
	}
}

AttributeTypeAS4PathSegment::~AttributeTypeAS4PathSegment(void) {
	delete pathSegmentValue;
	pathSegmentValue = NULL;
}

AttributeType* AttributeTypeAS4PathSegment::clone() {
	AttributeTypeAS4PathSegment *atAS4PathSegment = new AttributeTypeAS4PathSegment();
	// TODO: copy each element in the list
	list<uint32_t>::iterator it;
	for (it = pathSegmentValue->begin(); it != pathSegmentValue->end(); it++) {
		atAS4PathSegment->setPathSegmentValue(*it);
	}	
	atAS4PathSegment->setPathSegmentType(getPathSegmentType());
	atAS4PathSegment->setPathSegmentLength(getPathSegmentLength());
	return atAS4PathSegment;
}

void AttributeTypeAS4PathSegment::printMe() { 
	switch (pathSegmentType) {
		case AS_SEQUENCE: break;
		case AS_SET: cout << "{";
	}
		
	if (pathSegmentValue != NULL) {	
		list<uint32_t>::iterator it;
		
		for (it = pathSegmentValue->begin(); it != pathSegmentValue->end(); it++) {
			cout << " " << *it;
		}	
	}
	
	switch (pathSegmentType) {
		case AS_SEQUENCE: break;
		case AS_SET: cout << "}";
	}
}

void AttributeTypeAS4PathSegment::printMeCompact() { 
	if (pathSegmentValue != NULL)
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
				for (it = pathSegmentValue->begin(); it != pathSegmentValue->end(); it++) {
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
				for (it = pathSegmentValue->begin(); it != pathSegmentValue->end(); it++) {
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
}


//////////////////////////////////////////////////////////////////////////////////////////
// AttributeTypeAS4Path
//
AttributeTypeAS4Path::AttributeTypeAS4Path(void)
{
	this->pathSegments = new list<AttributeTypeAS4PathSegment>();
}

AttributeTypeAS4Path::AttributeTypeAS4Path(const AttributeTypeAS4Path& attr)
: AttributeType(attr.length, attr.value, attr.isAS4)
{
	pathSegmentType = attr.pathSegmentType;
	pathSegmentLength = attr.pathSegmentLength;
	this->pathSegments = new list<AttributeTypeAS4PathSegment>(attr.pathSegments->begin(),attr.pathSegments->end());
}

AttributeTypeAS4Path::AttributeTypeAS4Path(uint16_t len, uint8_t* msg)
: AttributeType(len, msg, isAS4)
{
	PRINT_DBG("AttributeTypeAS4Path::AttributeTypeAS4Path()");	
	
	pathSegments = new list<AttributeTypeAS4PathSegment>();
	
	uint8_t* endMsg = msg + len;
	// TODO: Right now I'm keeping this for compatibility, but pathSegmentType
	//       and pathSegmentLength should not be members of AttributeTypeAS4Path 
	//       (they are members of AttributeTypeAS4PathSegment).
	if (len != 0)
	{
		pathSegmentType = *msg & BITMASK_8;
		pathSegmentLength = *(msg+1) & BITMASK_8;
		uint16_t firstTwoBytes = 0;
		firstTwoBytes = *((uint16_t*)(msg+2)) & BITMASK_16;
	}
	else
	{
		pathSegmentType = 0;
		pathSegmentLength = 0;
	}
	while (msg < endMsg)
	{
		uint8_t segLength = *(msg+1) & BITMASK_8;
		//AttributeTypeAS4PathSegment pathSegment(len, msg);
		AttributeTypeAS4PathSegment pathSegment(len, msg);
		msg += 2; // pathSegmentType, pathSegmentLength fields
		msg += segLength * sizeof(uint32_t);
		
		pathSegments->push_back(pathSegment);
	}
}

AttributeTypeAS4Path::~AttributeTypeAS4Path(void)
{
	delete pathSegments;
	pathSegments = NULL;
}

AttributeType* AttributeTypeAS4Path::clone() {
	AttributeTypeAS4Path *atAS4Path = new AttributeTypeAS4Path();
	// TODO: copy each element in pathSegments list using pathSegments's clone()
	list<AttributeTypeAS4PathSegment>::iterator it;
	for (it = pathSegments->begin(); it != pathSegments->end(); it++) {
		atAS4Path->setAS4PathSegment((*it));
	}	
	atAS4Path->setPathSegmentType(getPathSegmentType());
	atAS4Path->setPathSegmentLength(getPathSegmentLength());
	return atAS4Path;
}

void AttributeTypeAS4Path::printMe() 
{ 
	cout << "AS4_PATH:";
	if (pathSegments != NULL)
	{	
		list<AttributeTypeAS4PathSegment>::iterator it;
		
		for (it = pathSegments->begin(); it != pathSegments->end(); it++)
		{
			//cout << endl << "  ";
			(*it).printMe();
		}	
	}
}

void AttributeTypeAS4Path::printMeCompact()
{
	if (pathSegments != NULL)
	{	
		list<AttributeTypeAS4PathSegment>::iterator it;
		
		for (it = pathSegments->begin(); it != pathSegments->end(); it++)
		{
			//cout << endl << "  ";
			(*it).printMeCompact();
		}	
	}
}

list<uint32_t>* AttributeTypeAS4Path::getPathSegmentValue() const
{ 
	if (pathSegments->size() == 0)
		return NULL;
	else
	{
		AttributeTypeAS4PathSegment pathSegment = pathSegments->front();
		return pathSegment.pathSegmentValue; 
	}
}


