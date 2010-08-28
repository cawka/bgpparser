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

#ifndef _TBLDUMPV2RIBENTRY_H_
#define _TBLDUMPV2RIBENTRY_H_

#ifdef WIN32
#define _USE_32BIT_TIME_T
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#include <netinet/in.h>
#endif	/* WIN32 */

#include "MRTTblDumpV2PeerIndexTbl.h"
#include "BGPAttribute.h"

#include <list>
using namespace std;

class TblDumpV2RibEntry
{
public:
	TblDumpV2RibEntry( std::istream &input );
	virtual ~TblDumpV2RibEntry(void);

	uint16_t getPeerIndex(void) const { return peerIndex; };
	void setPeerIndex(uint16_t peerIndex) { this->peerIndex = peerIndex; };
	time_t getOriginatedTime(void) const { return originatedTime; };
	void setOriginatedTime(time_t originatedTime) { this->originatedTime = originatedTime; };

	/* consider removing attribute length getter/setters */
	uint16_t getAttributeLength(void) const { return attributeLength; };
	void setAttributeLength(uint16_t attributeLength) { this->attributeLength = attributeLength; };

	const std::list<BGPAttributePtr> &getAttributes(void) const { return attributes; };
	
	virtual void printMe();
	virtual void printMe( const MRTTblDumpV2PeerIndexTblPtr &peerIndexTbl );
	virtual void printMeCompact();
	virtual void printMeCompact( const MRTTblDumpV2PeerIndexTblPtr &peerIndexTbl );

protected:
	uint16_t peerIndex; // This is an index into the PEER_INDEX_TABLE MRT record
	uint32_t originatedTime;
	uint16_t attributeLength;		/* consider removing attribute length field */
	
	/* add list of BGPAttributes */
	std::list<BGPAttributePtr> attributes;

private:
	static log4cxx::LoggerPtr Logger;
};

typedef boost::shared_ptr<TblDumpV2RibEntry> TblDumpV2RibEntryPtr;

#endif	/* _TBLDUMPV2RIBENTRY_H_ */
