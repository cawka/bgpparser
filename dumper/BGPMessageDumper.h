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
#ifndef __BGPMSGDUMPER_H_
#define __BGPMSGDUMPER_H_

#include "Dumper.h"
#include "BGPCommonHeader.h"
#include "BGPAttribute.h"
#include "MRTBgp4MPMessage.h"

#define _XFB_VERSION "0.2"

/* Common Dumper */
class BGPMessageDumper : public Dumper {
public:
  BGPMessageDumper(const BGPMessagePtr& msg);
  virtual ~BGPMessageDumper();

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

  bool
  isTableDump(bool is_tabledump)
  {
    this->is_tabledump = is_tabledump;
    return isTableDump();
  };

  bool
  isTableDump()
  {
    return this->is_tabledump;
  };

private:
  BGPMessageDumper()
  {
  }

protected:
  /* Time */
  time_t timestamp;

  /* Type */
  bool is_tabledump;

  /* Peering */
  IPAddress peer_addr;
  IPAddress local_addr;
  uint32_t peer_as;
  uint32_t local_as;
  uint16_t if_idx;
  uint16_t afi;

  /* BGP messsage */
  BGPMessagePtr bgp_msg;
};

typedef std::shared_ptr<BGPMessageDumper> BGPMessageDumperPtr;

#endif /* __BGPMSGDUMPER_H_ */

// vim: sw=4 ts=4 sts=4 expandtab
