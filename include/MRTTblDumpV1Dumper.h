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

// MRT TABLE_DUMP_V1 Dumper
#ifndef __MRTTBLDUMPV1DUMPER_H_
#define __MRTTBLDUMPV1DUMPER_H_

#include <list>
#include "Dumper.h"
#include "BGPCommonHeader.h"
#include "BGPAttribute.h"
#include "MRTTblDump.h"

using namespace std;

#define _XFB_VERSION "0.2" 

/* Common Dumper */
class MRTTblDumpV1Dumper : public Dumper
{
public:
	MRTTblDumpV1Dumper();
	virtual ~MRTTblDumpV1Dumper();
	
	// Factory method for creating a BGP dumper
	//static class MRTTblDumpV1Dumper* newDumper(BGPMessage*);
	xmlNodePtr genXml();
	string     genAscii();

    void setTblDumpMsg(MRTTblDump* tblDumpMsg)
    {
        this->tblDumpMsg = tblDumpMsg;
    }

protected:
    /* TABLE_DUMPV1 */
	MRTTblDump*    tblDumpMsg;
};

#endif /* __MRTTBLDUMPV2DUMPER_H_ */

// vim: sw=4 ts=4 sts=4 expandtab
