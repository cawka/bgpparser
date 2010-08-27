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

#ifndef _MRTCOMMONHEADER_H_
#define _MRTCOMMONHEADER_H_

#include "MRTStructure.h"
#include <iostream>

/* the MRTCommonHeader is a MRTMessage */
typedef class MRTCommonHeader	MRTMessage;
typedef class boost::shared_ptr<MRTCommonHeader> MRTMessagePtr;

/* Common header defined by all MRT packets */
class MRTCommonHeader
{
public:
	MRTCommonHeader( std::istream &input );
	MRTCommonHeader( const uint8_t **input );
	virtual ~MRTCommonHeader(void);

	time_t getTimestamp(void) const;
	uint16_t getType(void) const;
	uint16_t getSubType(void) const;
	uint32_t getLength(void) const;

	/* static interface... uint8_t pointer will be updated to new location in file after call */
	static MRTMessagePtr newMessage( std::istream &input );

private:
	MRTCommonHeader( );	/* disable default constructor */

protected:
	time_t timestamp;		/* timestamp of MRT message */
	uint16_t type;	/* type of message in MRT payload */
	uint16_t subtype;	/* subtype of message in MRT payload */
	uint32_t length;	/* length of payload in MRT */

private:
	static log4cxx::LoggerPtr Logger;
};

#endif	/* _MRTCOMMONHEADER_H_ */
