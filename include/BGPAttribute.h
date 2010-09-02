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
#ifndef _BGPATTRIBUTE_H_
#define	_BGPATTRIBUTE_H_

#include <stdint.h>

#ifdef WIN32
#define _USE_32BIT_TIME_T
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#include <netinet/in.h>
#endif	/* WIN32 */

#include "MRTStructure.h"
#include "BGPStructure.h"
#include "AttributeType.h"

class BGPAttribute
{
public:
	BGPAttribute( std::istream &input, bool isAS4 );
	virtual ~BGPAttribute(void);

	uint8_t getAttributeFlags(void) const { return attributeFlags; };
	void setAttributeFlags(uint8_t attributeFlags) { this->attributeFlags = attributeFlags; };
	
	uint8_t getAttributeTypeCode(void) const { return attributeTypeCode; };
	void setAttributeTypeCode(uint8_t attributeTypeCode) { this->attributeTypeCode = attributeTypeCode; };

	std::string getAttributeTypeStr(void) const { return AttributeType::getTypeStr(attributeTypeCode); };
	
	uint16_t getAttributeLength(void) const 
		{ return isExtendedLength() ? attributeLength.twoOctet : BITMASK_8 & attributeLength.oneOctet; };
	void setAttributeLength(uint16_t attribLength) 
		{ isExtendedLength() ? attributeLength.twoOctet = attribLength : attributeLength.oneOctet = BITMASK_8 & attribLength; };

	AttributeTypePtr getAttributeValue(void) const  { return value; };
	AttributeTypePtr getAttributeValueMutable(void) { return value; };
//	// The instance takes ownership of value.
//	void setAttributeValue(AttributeType *value) { this->value = value; };
	
	int totalSize() { return sizeof(attributeFlags) 
						 + sizeof(attributeTypeCode) 
						 + (isExtendedLength() ? 2 : 1)
						 + getAttributeLength(); }

	bool isOptional(void) const { return ((attributeFlags & 0x80) == 0) ? false : true; };
	bool isWellKnown(void) const { return !isOptional(); };
	bool isTransitive(void) const { return ((attributeFlags & 0x40) == 0) ? false : true; };
	bool isNonTransitive(void) const { return !isTransitive(); };
	bool isPartial(void) const { return ((attributeFlags & 0x20) == 0) ? false : true; };
	bool isComplete(void) const { return !isPartial(); };
	bool isExtendedLength(void) const { return ((attributeFlags & 0x10) == 0) ? false : true; };

	virtual void printMe() { value->printMe(); };
	virtual void printMeCompact() { value->printMeCompact(); };
	
private:
	BGPAttribute( ) { ; }
	BGPAttribute(const BGPAttribute& bgpA) { ; };  // Copy constructor
	BGPAttribute(uint8_t* msg, bool isAS4);

protected:
	uint8_t attributeFlags;
	uint8_t attributeTypeCode;
	union
	{
		uint8_t oneOctet;	/* info used in parsing */
		uint16_t twoOctet;
	} attributeLength;

	/* define an attribute value class here */
	AttributeTypePtr value;

	static log4cxx::LoggerPtr Logger;
};

typedef boost::shared_ptr<BGPAttribute> BGPAttributePtr;

struct findByType : public std::unary_function<uint8_t, bool>
{
	uint8_t _type;

	findByType( uint8_t type ) : _type(type) { }

	bool operator( )( const BGPAttributePtr &obj ) const
	{
		return _type == obj->getAttributeTypeCode( );
	}
};

#endif	/* _BGPATTRIBUTE_H_ */

