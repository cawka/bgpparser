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

// Modified: Jonathan Park (jpark@cs.ucla.edu)
#include <bgpparser.h>

#include "AttributeTypeMPReachNLRI.h"

using namespace std;
namespace io = boost::iostreams;

log4cxx::LoggerPtr AttributeTypeMPReachNLRI::Logger =
  log4cxx::Logger::getLogger("bgpparser.AttributeTypeMPReachNLRI");

AttributeTypeMPReachNLRI::AttributeTypeMPReachNLRI(AttributeType& header, istream& input)
  : AttributeType(header)
  , afi(0)
  , safi(0)
  , nextHopAddressLength(0)
{
  memset(reinterpret_cast<char*>(&nextHopAddress), 0, sizeof(IPAddress));
  memset(reinterpret_cast<char*>(&nextHopAddressLocal), 0, sizeof(IPAddress));

  LOG4CXX_TRACE(Logger, "");

  bool error = false;

  // Test if there is only "NextHopLength" and "NextHopAddress" in this attribute (MRT TABLE_DUMP_V2
  // format)
  // http://tools.ietf.org/id/draft-ietf-grow-mrt-09.txt
  int testLength = input.peek();
  if (testLength < 0) {
    LOG4CXX_ERROR(Logger, "Cannot look forward (nothing left)");
    throw BGPError();
  }

  if (length == 1 + testLength) {
    error |= sizeof(uint8_t)
             != io::read(input, reinterpret_cast<char*>(&nextHopAddressLength), sizeof(uint8_t));

    afi = 0;
    safi = 0;
    memset(reinterpret_cast<char*>(&nextHopAddress), 0, sizeof(IPAddress));
    error |= nextHopAddressLength
             != io::read(input, reinterpret_cast<char*>(&nextHopAddress), nextHopAddressLength);

    if (error) {
      LOG4CXX_ERROR(Logger, "Parsing error (MRT TABLE_DUMP_V2 format)");
      throw BGPError();
    }
    return;
  }

  // Else parse every fields specified in RFC 4760
  error |= sizeof(uint16_t) != io::read(input, reinterpret_cast<char*>(&afi), sizeof(uint16_t));
  afi = ntohs(afi);
  LOG4CXX_TRACE(Logger, "  afi = " << (int)afi);

  if (error) {
    LOG4CXX_ERROR(Logger, "Parsing error (wrong AFI)");
    throw BGPError();
  }

  /* SAFI */
  error |= sizeof(uint8_t) != io::read(input, reinterpret_cast<char*>(&safi), sizeof(uint8_t));
  LOG4CXX_TRACE(Logger, "  safi = " << (int)safi);
  // SAFI not valid in MP_REACH_NLRI

  if (error) {
    LOG4CXX_ERROR(Logger, "Parsing error (wrong SAFI)");
    throw BGPError();
  }

  /* Next hop length and address */
  error |= sizeof(uint8_t)
           != io::read(input, reinterpret_cast<char*>(&nextHopAddressLength), sizeof(uint8_t));
  LOG4CXX_TRACE(Logger, "  nextHopAddressLength = " << (int)nextHopAddressLength);

  if (error) {
    LOG4CXX_ERROR(Logger, "Parsing error (wrong NextHop length)");
    throw BGPError();
  }

  if (nextHopAddressLength == 32) {
    /* this must be ipv6 with local address as well */
    error |=
      sizeof(nextHopAddress.ipv6) != io::read(input, reinterpret_cast<char*>(&nextHopAddress.ipv6),
                                              sizeof(nextHopAddress.ipv6));
    error |= sizeof(nextHopAddressLocal.ipv6)
             != io::read(input, reinterpret_cast<char*>(&nextHopAddressLocal.ipv6),
                         sizeof(nextHopAddressLocal.ipv6));
  }
  else if (nextHopAddressLength == 16) {
    /* this must be ipv6 */
    error |=
      sizeof(nextHopAddress.ipv6) != io::read(input, reinterpret_cast<char*>(&nextHopAddress.ipv6),
                                              sizeof(nextHopAddress.ipv6));
  }
  else if (nextHopAddressLength == 4) {
    /* this must be ipv4 */
    error |=
      sizeof(nextHopAddress.ipv4) != io::read(input, reinterpret_cast<char*>(&nextHopAddress.ipv4),
                                              sizeof(nextHopAddress.ipv4));
  }
  if (error) {
    LOG4CXX_ERROR(Logger, "Parsing error (wrong NextHop/NextHopLocal)");
    throw BGPError();
  }

  int left = length - /*afi*/ 2 - /*safi*/ 1 - /*nextHopAddressLength*/ 1 - nextHopAddressLength;

  /* JP: let's not worry about SNPA for now */
  uint8_t unSNPACnt = 0;
  uint8_t unSNPALen = 0;
  error |= sizeof(uint8_t) != io::read(input, reinterpret_cast<char*>(&unSNPACnt), sizeof(uint8_t));

  LOG4CXX_TRACE(Logger, "  unSNPACnt = " << (int)unSNPACnt);
  if (error) {
    LOG4CXX_ERROR(Logger, "Parsing error (wrong SNPACnt)");
    throw BGPError();
  }

  left--;

  for (int ic = 0; ic < unSNPACnt; ic++) {
    error |=
      sizeof(uint8_t) != io::read(input, reinterpret_cast<char*>(&unSNPALen), sizeof(uint8_t));
    error |= unSNPALen != io::seek(input, unSNPALen, std::ios_base::cur);

    left -= 1 + unSNPALen;
    if (error) {
      LOG4CXX_ERROR(Logger, "Parsing error (wrong SNPA)");
      throw BGPError();
    }
  }

  while (left > 0) {
    uint8_t prefixLength = 0;
    error |=
      sizeof(uint8_t) != io::read(input, reinterpret_cast<char*>(&prefixLength), sizeof(uint8_t));
    if (error) {
      LOG4CXX_ERROR(Logger, "message truncated! cannot read mbgp prefix.");
      throw BGPError();
    }
    if (prefixLength > sizeof(IPAddress) * 8) {
      LOG4CXX_ERROR(Logger, "abnormal prefix-length [" << (int)prefixLength
                                                       << "]. skip this record.");
      LOG4CXX_ERROR(Logger, "Length = " << (int)length << ", left = " << left);
      throw BGPError();
    }
    LOG4CXX_TRACE(Logger, "prefixLength = " << (int)prefixLength);

    NLRIReachablePtr route(new NLRIReachable(prefixLength, input));
    left -= 1 + route->getNumOctets();
    nlri.push_back(route);

    LOG4CXX_TRACE(Logger, "num_octets = " << (int)route->getNumOctets());
  }
}

AttributeTypeMPReachNLRI::~AttributeTypeMPReachNLRI(void)
{
}

void
AttributeTypeMPReachNLRI::addNLRI(NLRIReachablePtr& nlri)
{
  /**
   * If the actual header contained a full NLRI attribute, there is no need in extra stuff
   */
  if (this->nlri.size() == 0) {
    this->nlri.push_back(nlri);
  }
};
