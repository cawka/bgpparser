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

#include <libxml/tree.h>
#include "MRTBgp4MPStateChange.h"
#include "MRTBgp4MPStateChangeDumper.h"
#include "BGPStateChangeDumper.h"

MRTBgp4MPStateChangeDumper::MRTBgp4MPStateChangeDumper()
{}

MRTBgp4MPStateChangeDumper::~MRTBgp4MPStateChangeDumper()
{}

MRTBgp4MPStateChangeDumper* MRTBgp4MPStateChangeDumper::newDumper(MRTBgp4MPStateChange* bgp4mp_sc)
{
    MRTBgp4MPStateChangeDumper *dumper = new MRTBgp4MPStateChangeDumper();
    dumper->setMRTBgp4MPStateChange(bgp4mp_sc); 
    return dumper;
}

xmlNodePtr MRTBgp4MPStateChangeDumper::genXml()
{
    xmlNodePtr node = NULL;

    BGPStateChangeDumper *bgpsc_dumper = new BGPStateChangeDumper();
    bgpsc_dumper->setTimestamp(bgp4mp_state_change->getTimestamp());
    bgpsc_dumper->setPeering(
                                bgp4mp_state_change->getPeerIP(),
                                bgp4mp_state_change->getLocalIP(),
                                bgp4mp_state_change->getPeerAS(),
                                bgp4mp_state_change->getLocalAS(),
                                bgp4mp_state_change->getInterfaceIndex(),
                                bgp4mp_state_change->getAddressFamily()
                             );
    bgpsc_dumper->setState(bgp4mp_state_change->getOldState(), bgp4mp_state_change->getNewState());
    bgpsc_dumper->setAFI(bgp4mp_state_change->getAddressFamily());
    node = bgpsc_dumper->genXml();
    delete bgpsc_dumper;

    return node;
}

// vim: sw=4 ts=4 sts=4 expandtab
