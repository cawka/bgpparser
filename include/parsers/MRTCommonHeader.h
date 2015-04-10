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

class MRTTblDumpV2PeerIndexTbl;
typedef std::shared_ptr<MRTTblDumpV2PeerIndexTbl> MRTTblDumpV2PeerIndexTblPtr;

/* the MRTCommonHeader is a MRTMessage */
typedef class MRTCommonHeader MRTMessage;
typedef class std::shared_ptr<MRTCommonHeader> MRTMessagePtr;

/* Common header defined by all MRT packets */
class MRTCommonHeader : public Node {
public:
  virtual ~MRTCommonHeader(void);

  inline time_t
  getTimestamp() const;
  inline uint16_t
  getType() const;
  inline uint16_t
  getSubType() const;
  inline uint32_t
  getLength() const;

  const MRTCommonHeaderPacket&
  getHeader() const
  {
    return pkt;
  }
  const std::shared_ptr<char>&
  getData() const
  {
    return data;
  }

  /* static interface... uint8_t pointer will be updated to new location in file after call */
  /* !!! For MRTTblDumpV2 formats, parameter should specify MRTTblDumpV2PeerIndexTblPtr !!! */
  static MRTMessagePtr
  newMessage(std::istream& input, MRTTblDumpV2PeerIndexTblPtr& indexTbl);

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
  MRTCommonHeader(std::istream& input);

private:
  MRTCommonHeader(); /* disable default constructor */

protected:
  time_t timestamp; /* timestamp of MRT message */
  uint16_t type;    /* type of message in MRT payload */
  uint16_t subtype; /* subtype of message in MRT payload */
  uint32_t length;  /* length of payload in MRT */

  MRTCommonHeaderPacket pkt;    /* original MRT packet header */
  std::shared_ptr<char> data; /* raw data of MRT packet */

private:
  static log4cxx::LoggerPtr Logger;
};

time_t
MRTCommonHeader::getTimestamp(void) const
{
  return timestamp;
}

uint16_t
MRTCommonHeader::getType(void) const
{
  return type;
}

uint16_t
MRTCommonHeader::getSubType(void) const
{
  return subtype;
}

uint32_t
MRTCommonHeader::getLength(void) const
{
  return length;
}

#endif /* _MRTCOMMONHEADER_H_ */
