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

#ifndef _ATTRIBUTETYPEMPREACHNLRI_H_
#define _ATTRIBUTETYPEMPREACHNLRI_H_

#include "AttributeType.h"
#include "BGPStructure.h"

#include <list>

class AttributeTypeMPReachNLRI :
	public AttributeType
{
public:
	AttributeTypeMPReachNLRI(void);
	AttributeTypeMPReachNLRI(const AttributeTypeMPReachNLRI &);
	/* TODO: define this */
	AttributeTypeMPReachNLRI(uint16_t len, uint8_t* msg, bool isCompact=false);
	virtual ~AttributeTypeMPReachNLRI(void);

	void setAFI(uint16_t afi) { this->afi = afi; };
	uint16_t getAFI(void) const { return afi; };
	void setSAFI(uint8_t safi) { this->safi = safi; };
	uint8_t getSAFI(void) const { return safi; };
	void setNextHopAddressLength(uint8_t nextHopAddressLength) { this->nextHopAddressLength = nextHopAddressLength; };
	uint8_t getNextHopAddressLength(void) const { return nextHopAddressLength; };
	void setNextHopAddress(IPAddress *nextHopAddress) { memcpy(&(this->nextHopAddress), nextHopAddress, sizeof(IPAddress)); };
	IPAddress getNextHopAddress(void) const { return nextHopAddress; };
	void setNextHopAddressLocal(IPAddress *nextHopAddressLocal) { memcpy(&(this->nextHopAddressLocal), nextHopAddressLocal, sizeof(IPAddress)); };
	IPAddress getNextHopAddressLocal(void) const { return nextHopAddressLocal; };

	void addNLRI(NLRIReachable &nlri) { this->nlri->push_back(nlri); };
	list<NLRIReachable> *getNLRI(void) const { return nlri; };
	void addSNPA(NLRIReachable &snpa) { this->snpa->push_back(snpa); };
	list<NLRIReachable> *getSNPA(void) const { return snpa; };
	
	virtual void printMe();
	virtual void printMeCompact();
	virtual AttributeType* clone();
	
	void setCorrupt(bool c) { corrupt = c; };
	bool getCorrupt(void) { return corrupt; };

protected:
	uint16_t  afi;
	uint8_t   safi;
	uint8_t   nextHopAddressLength;
	IPAddress nextHopAddress;
	IPAddress nextHopAddressLocal;
	
	//uint8_t snpaLength; // Number of snpa (not octets!)
	list<NLRIReachable> *snpa;
	list<NLRIReachable> *nlri;
	
	bool corrupt;

	static LoggerPtr Logger;
};

#endif	/* _ATTRIBUTETYPEMPREACHNLRI_H_ */
