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

#ifndef _MRTTBLDUMPV2RIBHEADER_H_
#define _MRTTBLDUMPV2RIBHEADER_H_

#include "MRTCommonHeader.h"
#include "MRTTblDumpV2PeerIndexTbl.h"
#include "TblDumpV2RibEntry.h"

class MRTTblDumpV2RibHeader : public MRTCommonHeader {
public:
  virtual ~MRTTblDumpV2RibHeader(void);

  inline uint32_t
  getSequenceNumber() const;
  inline uint8_t
  getPrefixLength() const;
  inline IPAddress
  getPrefix() const;
  inline uint16_t
  getEntryCount() const;
  inline uint16_t
  getAFI() const;
  inline uint16_t
  getSAFI() const;

  const std::list<TblDumpV2RibEntryPtr>&
  getRibEntries(void) const
  {
    return ribs;
  };

  static void
  processAttributes(std::list<BGPAttributePtr>&, std::istream& input, int len, bool isAS4);

  inline const MRTTblDumpV2PeerIndexTblPeerEntryPtr
  getPeer(const TblDumpV2RibEntry& entry);

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
  MRTTblDumpV2RibHeader(MRTTblDumpV2PeerIndexTblPtr& peertbl, MRTCommonHeader& header,
                        std::istream& input);
  void
  init(std::istream& input);

protected:
  uint32_t sequenceNumber;
  uint8_t prefixLength;
  IPAddress prefix;
  uint16_t entryCount;
  uint16_t afi;
  uint16_t safi;

  std::list<TblDumpV2RibEntryPtr> ribs;
  MRTTblDumpV2PeerIndexTblPtr _peerIndexTbl;

  static log4cxx::LoggerPtr Logger;
};

typedef boost::shared_ptr<MRTTblDumpV2RibHeader> MRTTblDumpV2RibHeaderPtr;

uint32_t
MRTTblDumpV2RibHeader::getSequenceNumber(void) const
{
  return sequenceNumber;
}

uint8_t
MRTTblDumpV2RibHeader::getPrefixLength(void) const
{
  return prefixLength;
}

IPAddress
MRTTblDumpV2RibHeader::getPrefix(void) const
{
  return prefix;
}

uint16_t
MRTTblDumpV2RibHeader::getEntryCount(void) const
{
  return entryCount;
}

uint16_t
MRTTblDumpV2RibHeader::getAFI(void) const
{
  return afi;
}

uint16_t
MRTTblDumpV2RibHeader::getSAFI(void) const
{
  return safi;
}

const MRTTblDumpV2PeerIndexTblPeerEntryPtr
MRTTblDumpV2RibHeader::getPeer(const TblDumpV2RibEntry& entry)
{
  return _peerIndexTbl->getPeer(entry.getPeerIndex());
}

#endif /* _MRTTBLDUMPV2RIBHEADER_H_ */
