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

// Attribute Dumper
#ifndef __ATTDUMPER_H_
#define __ATTDUMPER_H_

#include <string>
#include "Dumper.h"
#include "AttributeType.h"

using namespace std;

class AttributeTypeDumper : public Dumper /* AKA BGPMessage */
{
public:
	AttributeTypeDumper(AttributeType*);
	virtual ~AttributeTypeDumper();
	
	// Factory method for creating a attribute dumper instance.
	static class AttributeTypeDumper* newDumper(uint8_t attrType, string attrTypeStr, AttributeType*);
	virtual xmlNodePtr genXml();
	virtual string     genAscii();

    string  getTypeStr(void)        {return type_str;      };
	void    setTypeStr(string ts)   {this->type_str = ts;  };
    uint8_t getTypeCode(void)       {return type_code;     };
	void    setTypeCode(uint8_t tc) {this->type_code = tc; };
protected:
    AttributeType* attr_type;
    string  type_str;     
    uint8_t type_code;     
};

class AttributeTypeOriginDumper : public AttributeTypeDumper
{
public:
	AttributeTypeOriginDumper(AttributeType*);
	virtual ~AttributeTypeOriginDumper();
	virtual xmlNodePtr genXml();
	virtual string     genAscii();
};

class AttributeTypeNextHopDumper : public AttributeTypeDumper
{
public:
	AttributeTypeNextHopDumper(AttributeType*);
	virtual ~AttributeTypeNextHopDumper();
	virtual xmlNodePtr genXml();
	virtual string     genAscii();
};

class AttributeTypeLocalPrefDumper : public AttributeTypeDumper
{
public:
	AttributeTypeLocalPrefDumper(AttributeType*);
	virtual ~AttributeTypeLocalPrefDumper();
	virtual xmlNodePtr genXml();
	virtual string     genAscii();
};

class AttributeTypeMultiExitDiscDumper : public AttributeTypeDumper
{
public:
	AttributeTypeMultiExitDiscDumper(AttributeType*);
	virtual ~AttributeTypeMultiExitDiscDumper();
	virtual xmlNodePtr genXml();
	virtual string     genAscii();
};

class AttributeTypeCommunitiesDumper : public AttributeTypeDumper
{
public:
	AttributeTypeCommunitiesDumper(AttributeType*);
	virtual ~AttributeTypeCommunitiesDumper();
	virtual xmlNodePtr genXml();
	virtual string     genAscii();
};

class AttributeTypeAggregatorDumper : public AttributeTypeDumper
{
public:
	AttributeTypeAggregatorDumper(AttributeType*);
	virtual ~AttributeTypeAggregatorDumper();
	virtual xmlNodePtr genXml();
	virtual string     genAscii();
};

class AttributeTypeAtomicAggregateDumper : public AttributeTypeDumper
{
public:
	AttributeTypeAtomicAggregateDumper(AttributeType*);
	virtual ~AttributeTypeAtomicAggregateDumper();
	virtual xmlNodePtr genXml();
	virtual string     genAscii();
};

class AttributeTypeASPathDumper : public AttributeTypeDumper
{
public:
	AttributeTypeASPathDumper(AttributeType*);
	virtual ~AttributeTypeASPathDumper();
	virtual xmlNodePtr genXml();
	virtual string     genAscii();
};

class AttributeTypeMPReachNLRIDumper : public AttributeTypeDumper
{
public:
	AttributeTypeMPReachNLRIDumper(AttributeType*);
	virtual ~AttributeTypeMPReachNLRIDumper();
	virtual xmlNodePtr genXml();
	//virtual string     genAscii();
    xmlNodePtr genPrefixNode(Route rt, int afi, int safi);
};

class AttributeTypeMPUnreachNLRIDumper : public AttributeTypeDumper
{
public:
	AttributeTypeMPUnreachNLRIDumper(AttributeType*);
	virtual ~AttributeTypeMPUnreachNLRIDumper();
	virtual xmlNodePtr genXml();
	//virtual string     genAscii();
    xmlNodePtr genPrefixNode(Route rt, int afi, int safi);
};

#endif /* __ATTDUMPER_H_ */

// vim: sw=4 ts=4 sts=4 expandtab
