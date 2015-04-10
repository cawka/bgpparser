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

#ifndef _MRTBGP4MPMESSAGE_H_
#define _MRTBGP4MPMESSAGE_H_

#include "MRTCommonHeader.h"
#include "BGPCommonHeader.h"

#include "MRTStructure.h"

/* MRT type BGP4MP and subtype BGP4MP_MESSAGE */
class MRTBgp4MPMessage : public MRTCommonHeader {
public:
  MRTBgp4MPMessage(MRTCommonHeader& header, std::istream& is);
  virtual ~MRTBgp4MPMessage(void);

  inline uint32_t
  getPeerAS(void) const;
  inline uint32_t
  getLocalAS(void) const;
  inline uint16_t
  getInterfaceIndex(void) const;
  inline uint16_t
  getAddressFamily(void) const;
  inline IPAddress
  getPeerIP(void) const;
  inline IPAddress
  getLocalIP(void) const;

  BGPMessagePtr
  getPayload() const
  {
    return payload;
  };

  virtual void
  accept(Visitor& v)
  {
    v.visit(*this);
  }
  virtual void
  accept(GJVoidVisitor& v, boost::any param)
  {
    v.visit(*this, param);
  }
  virtual boost::any
  accept(GJNoArguVisitor& v)
  {
    return v.visit(*this);
  }
  virtual boost::any
  accept(GJVisitor& v, boost::any param)
  {
    return v.visit(*this, param);
  }

protected:
  MRTBgp4MPMessage(MRTCommonHeader& header)
    : MRTCommonHeader(header)
  {
    ;
  }
  void
  processIPs(std::istream& input);

protected:
  uint32_t peerAS;  /* consider making private */
  uint32_t localAS; /* consider making private */
  uint16_t interfaceIndex;
  uint16_t addressFamily;
  IPAddress peerIP;
  IPAddress localIP;
  bool isAS4;

  BGPMessagePtr payload;

private:
  MRTBgp4MPMessage(void); /* disable default constructor */

  static log4cxx::LoggerPtr Logger;
};

typedef boost::shared_ptr<MRTBgp4MPMessage> MRTBgp4MPMessagePtr;

///////////////////////////////////////////////////////////////////////////

uint32_t
MRTBgp4MPMessage::getPeerAS(void) const
{
  return peerAS;
}

uint32_t
MRTBgp4MPMessage::getLocalAS(void) const
{
  return localAS;
}

uint16_t
MRTBgp4MPMessage::getInterfaceIndex(void) const
{
  return interfaceIndex;
}

uint16_t
MRTBgp4MPMessage::getAddressFamily(void) const
{
  return addressFamily;
}

IPAddress
MRTBgp4MPMessage::getPeerIP(void) const
{
  return peerIP;
}

IPAddress
MRTBgp4MPMessage::getLocalIP(void) const
{
  return localIP;
}

#endif /* _MRTBGP4MPMESSAGE_H_ */
