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

#include <bgpparser.h>
using namespace std;
using namespace boost;

#include "AttributeTypeOrigin.h"
#include "AttributeTypeDumper.h"

#include <string>
#include <sstream>
#include <libxml/tree.h>

extern "C" {
#include "xmlinternal.h"
}

AttributeTypeOriginDumper::AttributeTypeOriginDumper(const AttributeTypePtr& attr)
  : AttributeTypeDumper(attr)
{
  attr_type = attr;
}

AttributeTypeOriginDumper::~AttributeTypeOriginDumper()
{
}

xmlNodePtr
AttributeTypeOriginDumper::genXml()
{
  AttributeTypeOriginPtr attr_origin = dynamic_pointer_cast<AttributeTypeOrigin>(attr_type);

  string origin_str = "";
  switch (attr_origin->getOrigin()) {
  case AttributeTypeOrigin::IGP:
    origin_str = "IGP";
    break;
  case AttributeTypeOrigin::EGP:
    origin_str = "EGP";
    break;
  default:
    origin_str = "INCOMPLETE";
    break;
  }

  xmlNodePtr node = xmlNewNodeString("ORIGIN", origin_str.c_str());
  xmlNewPropInt(node, "value", attr_origin->getOrigin());

  return node;
}

string
AttributeTypeOriginDumper::genAscii()
{
  AttributeTypeOriginPtr attr_origin = dynamic_pointer_cast<AttributeTypeOrigin>(attr_type);

  string node = "";
  string origin_str = "";
  switch (attr_origin->getOrigin()) {
  case AttributeTypeOrigin::IGP:
    origin_str = "IGP";
    break;
  case AttributeTypeOrigin::EGP:
    origin_str = "EGP";
    break;
  default:
    origin_str = "INCOMPLETE";
    break;
  }
  node += origin_str;

  return node;
}

// vim: sw=4 ts=4 sts=4 expandtab
