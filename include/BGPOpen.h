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

/* BGPOpen */
#ifndef __BGPOpen_H__
#define __BGPOpen_H__

#include "BGPCommonHeader.h"

class BGPOpen : public BGPCommonHeader
{
public:
	BGPOpen( BGPCommonHeader &header, std::istream &input );
	virtual ~BGPOpen();
	
	virtual BGP_MESSAGE_TYPE Type()    { return OPEN; }
	virtual std::string      TypeStr() { return "OPEN"; };

	bool    isAS4Supported( ) const { return isAS4; }

	virtual void printMe();
	virtual void printMeCompact();

protected:
	uint8_t version;
	uint16_t myAS;
	uint16_t holdTime;
	uint32_t bgpId;
	uint8_t optParmLen;
	
	bool isAS4;

private:
	static log4cxx::LoggerPtr Logger;
};

typedef boost::shared_ptr<BGPOpen> BGPOpenPtr;

#endif /* __BGPOpen_H__ */
