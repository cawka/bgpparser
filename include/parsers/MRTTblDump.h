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

#include "MRTStructure.h"
#include "BGPAttribute.h"
#include "MRTTblDumpV2RibHeader.h"

/* MRT type TABLE_DUMP */
class MRTTblDump :
	public MRTCommonHeader
{
public:
	MRTTblDump( MRTCommonHeader &header, std::istream &input );
	virtual ~MRTTblDump(void);

	inline uint16_t  getViewNumber( ) const;
	inline uint16_t  getSequenceNumber( ) const;
	inline IPAddress getPrefix( ) const;
	inline uint8_t   getPrefixLength( ) const;
	inline uint8_t   getStatus( ) const;
	inline time_t    getOriginatedTime( ) const;
	inline IPAddress getPeerIP( ) const;
	inline uint16_t  getPeerAS( ) const;
	inline uint16_t  getAttributeLength( ) const;
	
	virtual void printMe() { std::cout << "MRTTblDump"; };
	virtual void printMeCompact();

	const std::list<BGPAttributePtr> &getAttributes(void) const { return attributes; };

	virtual void accept( Visitor &v ) 							{ v.visit( *this ); }
	virtual void accept( GJVoidVisitor &v, boost::any param )   { v.visit( *this, param ); }
	virtual boost::any accept( GJNoArguVisitor &v ) 		    { return v.visit( *this ); }
	virtual boost::any accept( GJVisitor &v, boost::any param ) { return v.visit( *this, param ); }

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

	std::list<BGPAttributePtr> attributes;

private:
	static log4cxx::LoggerPtr Logger;
};

typedef boost::shared_ptr<MRTTblDump> MRTTblDumpPtr;

uint16_t MRTTblDump::getViewNumber(void) const {
	return viewNumber;
}

uint16_t MRTTblDump::getSequenceNumber(void) const {
	return sequenceNumber;
}

IPAddress MRTTblDump::getPrefix(void) const {
	return prefix;
}

uint8_t MRTTblDump::getPrefixLength(void) const {
	return prefixLength;
}

uint8_t MRTTblDump::getStatus(void) const {
	return status;
}

time_t MRTTblDump::getOriginatedTime(void) const {
	return originatedTime;
}

IPAddress MRTTblDump::getPeerIP(void) const {
	return peerIP;
}

uint16_t MRTTblDump::getPeerAS(void) const {
	return peerAS;
}

uint16_t MRTTblDump::getAttributeLength(void) const {
	return attributeLength;
}

#endif	/* _MRTTBLDUMP_H_ */

