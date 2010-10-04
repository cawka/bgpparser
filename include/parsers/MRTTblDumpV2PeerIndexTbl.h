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
#include "MRTStructure.h"

/* MRT type TABLE_DUMP_V2 and subtype PEER_INDEX_TABLE */
class MRTTblDumpV2PeerIndexTbl :
	public MRTCommonHeader
{
//friend class MRTTblDumpV2RibIPv4Unicast;

public:
	MRTTblDumpV2PeerIndexTbl( MRTCommonHeader &header, std::istream &input );
	virtual ~MRTTblDumpV2PeerIndexTbl(void);

	inline uint32_t getCollectorBGPId( ) const;
	inline uint16_t getViewNameLength( ) const;
	inline uint16_t getPeerCount( ) const;

	const std::string &getViewName( ) const { return viewName; }

	const std::vector<MRTTblDumpV2PeerIndexTblPeerEntryPtr> &getPeerEntries(void) { return peerEntries; }
	inline const MRTTblDumpV2PeerIndexTblPeerEntryPtr getPeer( uint16_t peerIndex );

	virtual void accept( Visitor &v ) 							{ v.visit( *this ); }
	virtual void accept( GJVoidVisitor &v, boost::any param )   { v.visit( *this, param ); }
	virtual boost::any accept( GJNoArguVisitor &v ) 		    { return v.visit( *this ); }
	virtual boost::any accept( GJVisitor &v, boost::any param ) { return v.visit( *this, param ); }

protected:
	uint32_t collectorBGPId;
	uint16_t viewNameLength;
	std::string viewName;
	uint16_t peerCount;
	std::vector<MRTTblDumpV2PeerIndexTblPeerEntryPtr> peerEntries;

private:
	static log4cxx::LoggerPtr Logger;
};

typedef boost::shared_ptr<MRTTblDumpV2PeerIndexTbl> MRTTblDumpV2PeerIndexTblPtr;

const MRTTblDumpV2PeerIndexTblPeerEntryPtr MRTTblDumpV2PeerIndexTbl::getPeer( uint16_t peerIndex )
{
	if( peerIndex>peerEntries.size() ) throw BGPTextError( "Corrupted PeerIndexTbl" );
	return peerEntries[peerIndex];
}


uint32_t MRTTblDumpV2PeerIndexTbl::getCollectorBGPId(void) const {
	return collectorBGPId;
}

uint16_t MRTTblDumpV2PeerIndexTbl::getViewNameLength(void) const {
	return viewNameLength;
}

uint16_t MRTTblDumpV2PeerIndexTbl::getPeerCount(void) const {
	return peerCount;
}


#endif	/* _MRTTBLDUMPV2PEERINDEXTBL_H_ */

