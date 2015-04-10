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

// BGP Message Dumper
#ifndef __BGPSTATECHANGEDUMPER_H_
#define __BGPSTATECHANGEDUMPER_H_

#include "Dumper.h"
#include "MRTBgp4MPStateChange.h"

#define _XFB_VERSION "0.2"

/* Common Dumper */
class BGPStateChangeDumper : public Dumper {
public:
  BGPStateChangeDumper();
  virtual ~BGPStateChangeDumper();

  xmlNodePtr
  genXml();
  std::string
  genAscii();

  void
  setPeering(IPAddress peer_addr, IPAddress local_addr, uint32_t peer_as, uint32_t local_as,
             uint16_t if_idx, uint16_t afi)
  {
    this->peer_addr = peer_addr;
    this->local_addr = local_addr;
    this->peer_as = peer_as;
    this->local_as = local_as;
    this->if_idx = if_idx;
    this->afi = afi;
  };

  void
  setTimestamp(time_t timestamp)
  {
    this->timestamp = timestamp;
  };

  void
  setState(uint16_t oldState, uint16_t newState)
  {
    this->oldState = oldState;
    this->newState = newState;
  };

  void
  setAFI(uint16_t afi)
  {
    this->afi = afi;
  };

protected:
  /* Time */
  time_t timestamp;

  /* Peering */
  IPAddress peer_addr;
  IPAddress local_addr;
  uint32_t peer_as;
  uint32_t local_as;
  uint16_t if_idx;
  uint16_t afi;

  /* State */
  uint16_t oldState;
  uint16_t newState;

private:
  static log4cxx::LoggerPtr Logger;
};

typedef std::shared_ptr<BGPStateChangeDumper> BGPStateChangeDumperPtr;

#endif /* __BGPSTATECHANGEDUMPER_H_ */

// vim: sw=4 ts=4 sts=4 expandtab
