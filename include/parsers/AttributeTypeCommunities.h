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

// Author: Paul Wang
// Author: Jason Ryder
#ifndef _ATTRIBUTETYPECOMMUNITIES_H_
#define _ATTRIBUTETYPECOMMUNITIES_H_

#include "AttributeType.h"

#define COMMUNITY_NO_EXPORT 0xFFFFFF01
#define COMMUNITY_NO_ADVERTISE 0xFFFFFF02
#define COMMUNITY_NO_EXPORT_SUBCONFED 0xFFFFFF03
#define COMMUNITY_LOCAL_AS 0xFFFFFF03

struct CommunityValue {
  uint16_t ASnum;
  uint16_t info;
};

class AttributeTypeCommunities : public AttributeType {
public:
  AttributeTypeCommunities(AttributeType& header, std::istream& input);
  virtual ~AttributeTypeCommunities(void);

  const std::list<CommunityValue>&
  getCommunityValues(void) const
  {
    return communityValues;
  };
  //	void setCommunityValue(CommunityValue communityVal) {
  //this->communityValues->push_back(communityVal); };

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

private:
  std::list<CommunityValue> communityValues;

  static log4cxx::LoggerPtr Logger;
};

typedef std::shared_ptr<AttributeTypeCommunities> AttributeTypeCommunitiesPtr;

#endif /* _ATTRIBUTETYPECOMMUNITIES_H_ */
