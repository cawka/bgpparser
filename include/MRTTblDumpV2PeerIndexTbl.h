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

#ifndef _MRTTBLDUMPV2PEERINDEXTBL_H_
#define _MRTTBLDUMPV2PEERINDEXTBL_H_

#include "MRTCommonHeader.h"

#ifdef WIN32
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#include <netinet/in.h>
#endif	/* WIN32 */

#include "MRTStructure.h"

#include <list>

/* MRT type TABLE_DUMP_V2 and subtype PEER_INDEX_TABLE */
class MRTTblDumpV2PeerIndexTbl :
	public MRTCommonHeader
{
//friend class MRTTblDumpV2RibIPv4Unicast;

public:
	MRTTblDumpV2PeerIndexTbl(uint8_t **);
	virtual ~MRTTblDumpV2PeerIndexTbl(void);

	uint32_t getCollectorBGPId(void) const;
	uint16_t getViewNameLength(void) const;
	const uint8_t *getViewName(void) const;
	uint16_t getPeerCount(void) const;
	list<struct _MRTTblDumpV2PeerIndexTblPeerEntry> *getPeerEntries(void);

protected:
	uint32_t collectorBGPId;
	uint16_t viewNameLength;
	uint8_t *viewName;
	uint16_t peerCount;
	list<struct _MRTTblDumpV2PeerIndexTblPeerEntry> *peerEntries;

private:
	MRTTblDumpV2PeerIndexTbl(void);
};

#endif	/* _MRTTBLDUMPV2PEERINDEXTBL_H_ */

