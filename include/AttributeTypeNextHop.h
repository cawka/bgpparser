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

#ifndef _ATTRIBUTETYPENEXTHOP_H_
#define _ATTRIBUTETYPENEXTHOP_H_

#include "MRTStructure.h"
#include "AttributeType.h"

class AttributeTypeNextHop :
	public AttributeType
{
public:
	AttributeTypeNextHop(AttributeType &header, std::istream &input);
	virtual ~AttributeTypeNextHop(void);

	IPAddress getNextHopIPAddress(void) const { return nextHop; };
	void setNextHopIPAddress(IPAddress *nextHop) { memcpy(&(this->nextHop), nextHop, sizeof(IPAddress)); };

	uint16_t getNextHopAFI(void) const { return afi; };
	void setNextHopAFI(uint16_t afi) { this->afi = afi; };
	
	virtual void printMe();
	virtual void printMeCompact();

protected:
	IPAddress nextHop;
	uint16_t afi;

	static log4cxx::LoggerPtr Logger;
};

typedef boost::shared_ptr<AttributeTypeNextHop> AttributeTypeNextHopPtr;

#endif	/* _ATTRIBUTETYPENEXTHOP_H_ */

