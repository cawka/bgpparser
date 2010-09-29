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

#ifndef _MRTBGP4MPSTATECHANGE_H_
#define _MRTBGP4MPSTATECHANGE_H_

#include "MRTCommonHeader.h"

#ifdef WIN32
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif	/* WIN32 */

#include "MRTStructure.h"

class MRTBgp4MPStateChange :
	public MRTCommonHeader
{
public:
	MRTBgp4MPStateChange( MRTCommonHeader &header, std::istream &input );
	virtual ~MRTBgp4MPStateChange(void);

	inline uint32_t getPeerAS( ) const;
	inline uint32_t getLocalAS( ) const;
	inline uint16_t getInterfaceIndex( ) const;
	inline uint16_t getAddressFamily( ) const;
	inline IPAddress getPeerIP( ) const;
	inline IPAddress getLocalIP( ) const;
	inline uint16_t getOldState( ) const;
	inline uint16_t getNewState( ) const;

	virtual void printMe(void);
	virtual void printMeCompact(void);

	virtual void accept( Visitor &v ) 							{ v.visit( *this ); }
	virtual void accept( GJVoidVisitor &v, boost::any param )   { v.visit( *this, param ); }
	virtual boost::any accept( GJNoArguVisitor &v ) 		    { return v.visit( *this ); }
	virtual boost::any accept( GJVisitor &v, boost::any param ) { return v.visit( *this, param ); }

protected:
	MRTBgp4MPStateChange( MRTCommonHeader &header) : MRTCommonHeader( header ) { ; }
	void processIPs( std::istream &input );
	void processStates( std::istream &input );

protected:
	uint32_t peerAS;
	uint32_t localAS;
	uint16_t interfaceIndex;
	uint16_t addressFamily;
	IPAddress peerIP;
	IPAddress localIP;
	uint16_t oldState;
	uint16_t newState;
	bool isAS4;

private:
	static log4cxx::LoggerPtr Logger;
};

typedef boost::shared_ptr<MRTBgp4MPStateChange> MRTBgp4MPStateChangePtr;

#endif	/* _MRTBGP4MPSTATECHANGE_H_ */

