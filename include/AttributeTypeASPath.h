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

// Author: Paul Wang
// Author: Jason Ryder

#ifndef _ATTRIBUTETYPEASPATH_H_
#define _ATTRIBUTETYPEASPATH_H_

#include "AttributeType.h"
#include "AttributeTypeAS4Path.h"

#include <list>
using namespace std;

class AttributeTypeASPathSegment :
	public AttributeType
{
	friend class AttributeTypeASPath;
	
public:
	AttributeTypeASPathSegment(void);
	AttributeTypeASPathSegment(uint8_t type, uint8_t len);
	AttributeTypeASPathSegment(uint16_t len, uint8_t* msg, bool is4byte = false);
	AttributeTypeASPathSegment(const AttributeTypeASPathSegment&);
	AttributeTypeASPathSegment(const AttributeTypeAS4PathSegment&);
	virtual ~AttributeTypeASPathSegment(void);
	
	enum PathSegment
	{
		AS_SET = 1,
		AS_SEQUENCE,
		AS_CONFED_SEQUENCE, 
		AS_CONFED_SET
	};
	
	uint8_t getPathSegmentType(void) const                   { return pathSegmentType; };
	void    setPathSegmentType(uint32_t pathSegmentType)     { this->pathSegmentType = pathSegmentType; };
	uint8_t getPathSegmentLength(void) const                 { return pathSegmentLength; };
	void    setPathSegmentLength(uint32_t pathSegmentLength) { this->pathSegmentLength = pathSegmentLength; };

	list<uint32_t> *getPathSegmentValue(void) const { return pathSegmentValue; };
	void setPathSegmentValue(uint32_t value) { pathSegmentValue->push_back(value); };
	
	virtual void printMe();
	virtual void printMeCompact();
	virtual AttributeType* clone();
	
private:
	uint8_t pathSegmentType;
	uint8_t pathSegmentLength; // Number of path segments (not octets!)
	list<uint32_t> *pathSegmentValue;
};

class AttributeTypeASPath :
	public AttributeType
{	
public:
	AttributeTypeASPath(void);
	AttributeTypeASPath(uint16_t len, uint8_t* msg);
	AttributeTypeASPath(uint16_t len, uint8_t* msg, bool isAS4);
	AttributeTypeASPath(const AttributeTypeASPath&);
	virtual ~AttributeTypeASPath(void);

	void genPathSegmentsComplete(AttributeTypeAS4Path*);

	list<AttributeTypeASPathSegment> *getPathSegments(void)         { return pathSegments;         }
	list<AttributeTypeASPathSegment> *getPathSegmentsComplete(void) { return pathSegmentsComplete; }
	
	virtual void printMe();
	virtual void printMeCompact();
	virtual AttributeType* clone();
	
private:
	list<AttributeTypeASPathSegment> *pathSegments;
	list<AttributeTypeASPathSegment> *pathSegmentsComplete;
};


#endif	/* _ATTRIBUTETYPEASPATH_H_ */

