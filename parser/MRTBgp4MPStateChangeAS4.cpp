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

#include "MRTBgp4MPStateChangeAS4.h"

using namespace std;
namespace io = boost::iostreams;

log4cxx::LoggerPtr MRTBgp4MPStateChangeAS4::Logger =
  log4cxx::Logger::getLogger("bgpparser.MRTBgp4MPStateChangeAS4");

MRTBgp4MPStateChangeAS4::MRTBgp4MPStateChangeAS4(MRTCommonHeader& header, istream& input)
  : MRTBgp4MPStateChange(header)
{
  MRTBgp4MPStateChangeAS4Packet pkt;
  bool error =
    sizeof(MRTBgp4MPStateChangeAS4Packet)
    != io::read(input, reinterpret_cast<char*>(&pkt), sizeof(MRTBgp4MPStateChangeAS4Packet));

  if (error) {
    LOG4CXX_ERROR(Logger, "Parsing error");
    throw BGPError();
  }

  peerAS = ntohl(pkt.peerAS);
  localAS = ntohl(pkt.localAS);
  interfaceIndex = ntohs(pkt.interfaceIndex);
  addressFamily = ntohs(pkt.addressFamily);

  processIPs(input);
  processStates(input);
}
