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
#include "AttributeTypeASPath.h"


////////////////////////////////////////////////////////////////////
// AttributeTypeASPathSegment
//

AttributeTypeASPathSegment::AttributeTypeASPathSegment(void) {
	pathSegmentType   = 0;
	pathSegmentLength = 0;
	this->pathSegmentValue = new list<uint32_t>();
}

AttributeTypeASPathSegment::AttributeTypeASPathSegment(uint8_t type, uint8_t len) {
	pathSegmentType   = type;
	pathSegmentLength = len;
	this->pathSegmentValue = new list<uint32_t>();
}

AttributeTypeASPathSegment::AttributeTypeASPathSegment(const AttributeTypeASPathSegment& attr)
: AttributeType(attr) {
	pathSegmentType   = attr.pathSegmentType;
	pathSegmentLength = attr.pathSegmentLength;
	this->pathSegmentValue = new list<uint32_t>(attr.pathSegmentValue->begin(),attr.pathSegmentValue->end());
}

AttributeTypeASPathSegment::AttributeTypeASPathSegment(uint16_t len, uint8_t* msg, bool isAS4)
						   : AttributeType(len, msg, isAS4) {
	uint8_t *ptr = msg;
	
	pathSegmentValue = new list<uint32_t>();
	pathSegmentType = *ptr++ & BITMASK_8;
	pathSegmentLength = *ptr++ & BITMASK_8;

	for (int i=0; i < pathSegmentLength; i++) {
		uint32_t segVal;
		memcpy(&segVal, (ptr + (i << (isAS4 ? 2 : 1))), (isAS4 ? sizeof(uint32_t) : sizeof(uint16_t)));
		segVal = isAS4 ? ntohl(segVal) : ntohs(segVal);
		pathSegmentValue->push_back(segVal);
	}
}

AttributeTypeASPathSegment::~AttributeTypeASPathSegment(void) {
    delete pathSegmentValue;
	pathSegmentValue = NULL;
}

AttributeType* AttributeTypeASPathSegment::clone() {
	AttributeTypeASPathSegment* atASPathSeg = new AttributeTypeASPathSegment();
    atASPathSeg->setPathSegmentType(getPathSegmentType());
    atASPathSeg->setPathSegmentLength(getPathSegmentLength());
	list<uint32_t>::iterator it;
	for (it = pathSegmentValue->begin(); it != pathSegmentValue->end(); it++) {
        atASPathSeg->pathSegmentValue->push_back(*it);
    }
	return atASPathSeg;
}

void AttributeTypeASPathSegment::printMe() { 
	switch (pathSegmentType) {
		case AS_SEQUENCE: break;
		case AS_SET: cout << " {";
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

void AttributeTypeASPathSegment::printMeCompact() { 
	if (pathSegmentValue != NULL) {	
		uint16_t top, bottom;
		list<uint32_t>::iterator it;
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
// AttributeTypeASPath
//
AttributeTypeASPath::AttributeTypeASPath(void) {
	this->pathSegments         = new list<AttributeTypeASPathSegment>();
	this->pathSegmentsComplete = new list<AttributeTypeASPathSegment>();
}

AttributeTypeASPath::AttributeTypeASPath(const AttributeTypeASPath& attr)
: AttributeType(attr.length, attr.value, attr.isAS4) {
	this->pathSegments         = new list<AttributeTypeASPathSegment>(attr.pathSegments->begin(),attr.pathSegments->end()); 
	this->pathSegmentsComplete = new list<AttributeTypeASPathSegment>(attr.pathSegmentsComplete->begin(),attr.pathSegmentsComplete->end()); 
}

AttributeTypeASPath::AttributeTypeASPath(uint16_t len, uint8_t* msg)
: AttributeType(len, msg, isAS4) {
	PRINT_DBG("AttributeTypeASPath::AttributeTypeASPath()");	
	
	pathSegments         = new list<AttributeTypeASPathSegment>();
	pathSegmentsComplete = new list<AttributeTypeASPathSegment>();

	// TODO: Add a routine to check is4byte in more reliable way
	// check is4byte here
	uint8_t* endMsg = msg + len;
	bool is4byte = false;
	if (len != 0) {
		uint16_t firstTwoBytes = 0;
		firstTwoBytes = *((uint16_t*)(msg+2)) & BITMASK_16;
		is4byte = firstTwoBytes == 0;
	}
	while (msg < endMsg)
	{
		uint8_t segLength = *(msg+1) & BITMASK_8;
		AttributeTypeASPathSegment pathSegment(len, msg, is4byte);
		msg += 2; // pathSegmentType, pathSegmentLength fields
		msg += segLength * (is4byte ? sizeof(uint32_t) : sizeof(uint16_t));
		pathSegments->push_back(pathSegment);
	}
}

AttributeTypeASPath::AttributeTypeASPath(uint16_t len, uint8_t* msg, bool isAS4)
: AttributeType(len, msg, isAS4) {
	PRINT_DBG("AttributeTypeASPath::AttributeTypeASPath()");	
	
	pathSegments         = new list<AttributeTypeASPathSegment>();
	pathSegmentsComplete = new list<AttributeTypeASPathSegment>();

	uint8_t* endMsg = msg + len;
	while (msg < endMsg) {
		uint8_t segLength = *(msg+1) & BITMASK_8;
		AttributeTypeASPathSegment pathSegment(len, msg, isAS4);
		msg += 2; // pathSegmentType, pathSegmentLength fields
		msg += segLength * (isAS4 ? sizeof(uint32_t) : sizeof(uint16_t));
		pathSegments->push_back(pathSegment);
	}
}

AttributeTypeASPath::~AttributeTypeASPath(void) {
	delete pathSegments;         pathSegments = NULL;
	delete pathSegmentsComplete; pathSegmentsComplete = NULL;
}

AttributeType* AttributeTypeASPath::clone() {
    AttributeTypeASPath *atASPath = new AttributeTypeASPath();
	list<AttributeTypeASPathSegment>::iterator it;
	for (it = pathSegments->begin(); it != pathSegments->end(); it++) {
        atASPath->pathSegments->push_back(*it);
    }
	for (it = pathSegmentsComplete->begin(); it != pathSegmentsComplete->end(); it++) {
        atASPath->pathSegmentsComplete->push_back(*it);
    }
    return atASPath;
}

void AttributeTypeASPath::printMe() { 
	cout << "ASPATH :";
	if (pathSegments != NULL)
	{	
		list<AttributeTypeASPathSegment>::iterator it;
		for (it = pathSegments->begin(); it != pathSegments->end(); it++) {
			//cout << endl << "  ";
			(*it).printMe();
		}	
	}
	cout << endl;
	cout << "ASPATH':";
	if (pathSegmentsComplete != NULL)
	{	
		list<AttributeTypeASPathSegment>::iterator it;
		for (it = pathSegmentsComplete->begin(); it != pathSegmentsComplete->end(); it++)
		{
			//cout << endl << "  ";
			(*it).printMe();
		}	
	}
}

void AttributeTypeASPath::printMeCompact() {
	if (pathSegments != NULL) {	
		cout << "AS_SEQUENCE: ";
		list<AttributeTypeASPathSegment>::iterator it;
		
		for (it = pathSegments->begin(); it != pathSegments->end(); it++) {
			(*it).printMeCompact();
		}	
	}
}

void AttributeTypeASPath::genPathSegmentsComplete(AttributeTypeAS4Path* as4_path)
{
	if (as4_path == NULL)
	{
		*pathSegmentsComplete = *pathSegments;
		return;
	}

	list<AttributeTypeASPathSegment>*  as_path_segs  = this->getPathSegments();
	list<AttributeTypeAS4PathSegment>* as4_path_segs = as4_path->getPathSegments();

	if (as_path_segs != NULL)
	{

		list<AttributeTypeASPathSegment>::reverse_iterator  it  = as_path_segs->rbegin();
		list<AttributeTypeAS4PathSegment>::reverse_iterator it4 = as4_path_segs->rbegin();

        while (it != as_path_segs->rend() && it4 != as4_path_segs->rend())
        {
            // Merge as_path and as4_path
            AttributeTypeASPathSegment pathSegment(it->getPathSegmentType(), it->getPathSegmentLength());

            AttributeTypeASPathSegment*  as_seg  = &(*it);
            AttributeTypeAS4PathSegment* as4_seg = &(*it4);

            list<uint32_t> *as_seg_value  = as_seg->getPathSegmentValue();
            list<uint32_t> *as4_seg_value = as4_seg->getPathSegmentValue();

            list<uint32_t>::iterator it_seg  = as_seg_value->begin();
            list<uint32_t>::iterator it_seg4 = as4_seg_value->begin();

            int size_as_seg  = as_seg_value->size();
            int size_as4_seg = as4_seg_value->size();
            int from_as_seg  = size_as_seg  - size_as4_seg;

            for(int idx=1; idx<=size_as_seg; idx++)
            {
                if (idx <= from_as_seg)
                {
                    pathSegment.pathSegmentValue->push_back(*it_seg);
                    it_seg++;
                }
                else
                {
                    pathSegment.pathSegmentValue->push_back(*it_seg4);
                    it_seg4++;
                }
            }
            pathSegmentsComplete->push_front(pathSegment);
            it++;
            it4++;
        }

        while (it != as_path_segs->rend())
        {
			// Copy from as_path
            AttributeTypeASPathSegment pathSegment(*it);
            pathSegmentsComplete->push_front(pathSegment);
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
        if (it4 != as4_path_segs->rend()) {
            /* currently we do not have an indicator for errors.  Using NULL value to indicate error... */
            //error = 1;
        }
#endif
	}
}

// vim: sw=4 ts=4 sts=4 expandtab
