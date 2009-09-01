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

#ifndef _MRTTBLDUMP_H_
#define _MRTTBLDUMP_H_


#include "MRTCommonHeader.h"

#ifdef WIN32
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#include <netinet/in.h>
#endif	/* WIN32 */

#include "MRTStructure.h"
#include "BGPAttribute.h"
#include "MRTTblDumpV2RibHeader.h"

#include <list>
using namespace std;

/* MRT type TABLE_DUMP */
class MRTTblDump :
	public MRTCommonHeader
{
public:
	MRTTblDump(uint8_t **);
	virtual ~MRTTblDump(void);

	uint16_t getViewNumber(void) const;
	uint16_t getSequenceNumber(void) const;
	IPAddress getPrefix(void) const;
	uint8_t getPrefixLength(void) const;
	uint8_t getStatus(void) const;
	time_t getOriginatedTime(void) const;
	IPAddress getPeerIP(void) const;
	uint16_t getPeerAS(void) const;
	uint16_t getAttributeLength(void) const;
	
	virtual void printMe() { cout << "MRTTblDump"; };
	virtual void printMeCompact();

	list<BGPAttribute> *getAttributes(void) const { return attributes; };
protected:
	uint16_t viewNumber;
	uint16_t sequenceNumber;
	IPAddress prefix;
	uint8_t prefixLength;
	uint8_t status;
	time_t	originatedTime;
	IPAddress peerIP;
	uint16_t peerAS;
	uint16_t attributeLength;

	list<BGPAttribute> *attributes;
private:
	MRTTblDump(void);
};

#endif	/* _MRTTBLDUMP_H_ */

