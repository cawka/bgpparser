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

// MRT TABLE_DUMP_V2 Dumper
#ifndef __MRTBGP4MPMSGDUMPER__    
#define __MRTBGP4MPMSGDUMPER__    

#include "Dumper.h"
#include "MRTBgp4MPMessage.h"

using namespace std;

/* Common Dumper */
class MRTBgp4MPMessageDumper : public Dumper
{
public:
	MRTBgp4MPMessageDumper();
	virtual ~MRTBgp4MPMessageDumper();
	
	// Factory method for creating a dumper
	static class MRTBgp4MPMessageDumper* newDumper(MRTBgp4MPMessage*);

	xmlNodePtr genXml();
	string     genAscii();

    void setMRTBgp4MPMessagese(MRTBgp4MPMessage* bgp4mp_msg)
    {
        this->bgp4mp_msg = bgp4mp_msg;
    }


protected:
    /* Pointer to BGP4MP_MESSAGE */
    MRTBgp4MPMessage *bgp4mp_msg;
};

#endif /* __MRTBGP4MPMSGDUMPER__ */

// vim: sw=4 ts=4 sts=4 expandtab
