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
class AttributeTypeAS4Path;

const int AS_TRANS=23456;

class AttributeTypeASPathSegment : public Node
{
public:
	AttributeTypeASPathSegment( std::istream &input, bool is4byte );
	AttributeTypeASPathSegment( uint8_t type ) : pathSegmentType(type), pathSegmentLength(0) {;}
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

	const std::list<uint32_t> &getPathSegmentValue(void) const { return pathSegmentValue; };
		  std::list<uint32_t> &getPathSegmentValue(void)  	 { return pathSegmentValue; };
//	void setPathSegmentValue(uint32_t value) { pathSegmentValue->push_back(value); };

	void add( uint32_t asn )								 { pathSegmentValue.push_back(asn); }

	virtual void accept( Visitor &v ) 							{ v.visit( *this ); }
	virtual void accept( GJVoidVisitor &v, boost::any param )   { v.visit( *this, param ); }
	virtual boost::any accept( GJNoArguVisitor &v ) 		    { return v.visit( *this ); }
	virtual boost::any accept( GJVisitor &v, boost::any param ) { return v.visit( *this, param ); }

protected:
	uint8_t pathSegmentType;
	uint8_t pathSegmentLength; // Number of path segments (not octets!)
	std::list<uint32_t> pathSegmentValue;

	static log4cxx::LoggerPtr Logger;
};

typedef boost::shared_ptr<AttributeTypeASPathSegment> AttributeTypeASPathSegmentPtr;

class AttributeTypeASPath :
	public AttributeType
{	
public:
	AttributeTypeASPath( AttributeType &header, std::istream &input );
	virtual ~AttributeTypeASPath(void);

	void genPathSegmentsComplete( const AttributeTypeAS4Path &as4_path );
	void genPathSegmentsComplete( ) { pathSegmentsComplete=pathSegments; }

	const std::list<AttributeTypeASPathSegmentPtr> &getPathSegments(void)         const { return pathSegments;         }
	const std::list<AttributeTypeASPathSegmentPtr> &getPathSegmentsComplete(void) const { return pathSegmentsComplete; }
	
	uint32_t getCountOfASNs( ) const;

	virtual void accept( Visitor &v ) 							{ v.visit( *this ); }
	virtual void accept( GJVoidVisitor &v, boost::any param )   { v.visit( *this, param ); }
	virtual boost::any accept( GJNoArguVisitor &v ) 		    { return v.visit( *this ); }
	virtual boost::any accept( GJVisitor &v, boost::any param ) { return v.visit( *this, param ); }

private:
	std::list<AttributeTypeASPathSegmentPtr> pathSegments;
	std::list<AttributeTypeASPathSegmentPtr> pathSegmentsComplete;

	static log4cxx::LoggerPtr Logger;
};

typedef boost::shared_ptr<AttributeTypeASPath> AttributeTypeASPathPtr;

#endif	/* _ATTRIBUTETYPEASPATH_H_ */

