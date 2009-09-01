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

#ifndef _MRTTBLDUMPV2RIBHEADER_H_
#define _MRTTBLDUMPV2RIBHEADER_H_

#include "MRTCommonHeader.h"

#ifdef WIN32
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#include <netinet/in.h>
#endif	/* WIN32 */

#include "MRTStructure.h"
#include "MRTTblDumpV2PeerIndexTbl.h"
#include "TblDumpV2RibEntry.h"
#include "AttributeType.h"
#include "AttributeTypeOrigin.h"
#include "AttributeTypeASPath.h"
#include "AttributeTypeNextHop.h"
#include "AttributeTypeMultiExitDisc.h"
#include "AttributeTypeLocalPref.h"
#include "AttributeTypeAtomicAggregate.h"
#include "AttributeTypeAggregator.h"
#include "AttributeTypeCommunities.h"
#include "AttributeTypeMPReachNLRI.h"
#include "AttributeTypeMPUnreachNLRI.h"

#include <list>
using namespace std;

class MRTTblDumpV2RibHeader :
	public MRTCommonHeader
{
public:
	MRTTblDumpV2RibHeader(void);
	MRTTblDumpV2RibHeader(const uint8_t **);
	virtual ~MRTTblDumpV2RibHeader(void);

	uint32_t getSequenceNumber(void) const;
	uint8_t getPrefixLength(void) const;
	IPAddress getPrefix(void) const;
	uint16_t getEntryCount(void) const;
	uint16_t getAFI(void) const;
	uint16_t getSAFI(void) const;
	//list<struct _MRTTblDumpV2RibEntry> *getRibEntries(void) const;
	list<TblDumpV2RibEntry> *getRibEntries(void) const;

	//static void parseRibEntry(list<struct _MRTTblDumpV2RibEntry> *, uint16_t, uint8_t **);
	static void parseRibEntry(list<TblDumpV2RibEntry> *, uint16_t, uint8_t **);
	static void processAttributes(list<BGPAttribute> *, uint8_t *, const uint8_t * const, bool);

	//[TODO] use copy constructor to create a new PeerIndexTbl object, instead of just use the external object
	static void setPeerIndexTbl(MRTTblDumpV2PeerIndexTbl* peerIndexTbl) { MRTTblDumpV2RibHeader::_peerIndexTbl = peerIndexTbl; }
	static MRTTblDumpV2PeerIndexTbl* getPeerIndexTbl() { return MRTTblDumpV2RibHeader::_peerIndexTbl; }

	virtual void printMe() { cout << "MRTTblDumpV2RibHeader"; };
	virtual void printMe(MRTTblDumpV2PeerIndexTbl*) { cout << "MRTTblDumpV2RibHeader with Peer Index Table"; };
	virtual void printMeCompact() { cout << "MRTTblDumpV2RibHeader"; };
	virtual void printMeCompact(MRTTblDumpV2PeerIndexTbl *) { cout << "MRTTblDumpV2RibHeader"; };

protected:
	uint32_t sequenceNumber;
	uint8_t prefixLength;
	IPAddress prefix;
	uint16_t entryCount;
	uint16_t afi;
	uint16_t safi;
	//list<struct _MRTTblDumpV2RibEntry> *ribEntries;
	list<TblDumpV2RibEntry> *ribs;
	static MRTTblDumpV2PeerIndexTbl* _peerIndexTbl;
};

#endif	/* _MRTTBLDUMPV2RIBHEADER_H_ */
