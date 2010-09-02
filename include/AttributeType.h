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
#ifndef _ATTRIBUTETYPE_H_
#define _ATTRIBUTETYPE_H_

#include "BGPStructure.h"
#include <string>

class AttributeType;
typedef boost::shared_ptr<AttributeType> AttributeTypePtr;

class AttributeType
{
public:
	virtual ~AttributeType(void);
	
	// Factory method for creating new attribute
	static AttributeTypePtr newAttribute(uint8_t type, uint16_t len, std::istream &input, bool isAS4);
	static std::string getTypeStr(uint8_t attrType);

	uint16_t getLength() { return length; }
	const boost::shared_ptr<char> &getData() const { return data; }
//	uint8_t hasError() { return error; }
//	void setValue(uint8_t* value) { this->value = value; };
	bool getAS4() { return isAS4; }
	void setAS4(bool isAS4) { this->isAS4 = isAS4; };
	virtual void printMe() { /*cout << "AttributeType Default";*/ };
	virtual void printMeCompact() { /*cout << "AttributeType Default";*/ };
//	virtual AttributeType* clone() { /*cout << "Cloning AttributeType" << endl;*/ return new AttributeType(length, value, isAS4); }

	enum AttributeTypeCode
	{
		ORIGIN = 1,
		AS_PATH,
		NEXT_HOP,
		MULTI_EXIT_DISC,
		LOCAL_PREF,				/* 5 */
		ATOMIC_AGGREGATE,
		AGGREGATOR,
		COMMUNITIES,
		ORIGINATOR_ID,
		CLUSTER_LIST,			/* 10 */
		DPA,
		ADVERTISER,
		RCID_PATH,
		MP_REACH_NLRI,
		MP_UNREACH_NLRI,	/* 15 */
		EXT_COMMUNITIES,
		NEW_AS_PATH,
		NEW_AGGREGATOR
	};
	
private:
	AttributeType(uint16_t len, std::istream &input, bool isAS4=false);
	AttributeType( ) { }

protected:
	uint16_t length;
    bool isAS4;

	boost::shared_ptr<char> data;

	static log4cxx::LoggerPtr Logger;
};

#endif	/* _ATTRIBUTETYPE_H_ */

// vim: sw=4 ts=4 sts=4 expandtab
