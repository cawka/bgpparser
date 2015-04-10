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
#include "AttributeTypeASPath.h"
#include "AttributeTypeAS4Path.h"

using namespace std;
using namespace boost;

#include "AttributeTypeDumper.h"

#include <string>
#include <sstream>
#include <libxml/tree.h>
#include <boost/foreach.hpp>

extern "C" {
#include "xmlinternal.h"
}

AttributeTypeASPathDumper::AttributeTypeASPathDumper(const AttributeTypePtr& attr)
  : AttributeTypeDumper(attr)
{
}

AttributeTypeASPathDumper::~AttributeTypeASPathDumper()
{
}

xmlNodePtr
AttributeTypeASPathDumper::genXml()
{
  xmlNodePtr node = xmlNewNode(NULL, BAD_CAST(char*)type_str.c_str()); /* AS_PATH or AS4_PATH */

  const list<AttributeTypeASPathSegmentPtr>& pathSegments =
    (type_code == AttributeType::AS_PATH)
      ? dynamic_pointer_cast<AttributeTypeASPath>(attr_type)->getPathSegmentsComplete()
      : /*type_code==AttributeType::NEW_AS_PATH*/
      dynamic_pointer_cast<AttributeTypeAS4Path>(attr_type)->getPathSegments();

  BOOST_FOREACH (AttributeTypeASPathSegmentPtr as_seg, pathSegments) {
    xmlNodePtr as_seg_node = xmlNewNode(NULL, BAD_CAST "AS_SEG");
    xmlAddChild(node, as_seg_node);

    string type;
    uint8_t pathSegmentType = as_seg->getPathSegmentType();
    switch (pathSegmentType) {
    case AttributeTypeASPathSegment::AS_SEQUENCE:
      type = "AS_SEQUENCE";
      break;
    case AttributeTypeASPathSegment::AS_SET:
      type = "AS_SET";
      break;
    //[TO_DO] conf_set / conf_sequence
    case AttributeTypeASPathSegment::AS_CONFED_SEQUENCE:
      type = "AS_CONFED_SEQUENCE";
      break;
    case AttributeTypeASPathSegment::AS_CONFED_SET:
      type = "AS_CONFED_SET";
      break;
    default:
      type = "OTHER";
      break;
    }
    xmlNewPropString(as_seg_node, "type", type.c_str());
    xmlNewPropInt(as_seg_node, "length", as_seg->getPathSegmentLength());

    const list<uint32_t>& pathSegmentValue = as_seg->getPathSegmentValue();
    BOOST_FOREACH (uint32_t asn, pathSegmentValue) {
      xmlNewChildInt(as_seg_node, "AS", asn);
    }
  }

  if (type_code == AttributeType::AS_PATH) {
    string path = genAscii(true);
    xmlNewPropString(node, "complete", (char*)path.c_str());
  }
  return node;
}

string
AttributeTypeASPathDumper::genAscii()
{
  return genAscii(false);
}

string
AttributeTypeASPathDumper::genAscii(bool showComplete)
{
  ostringstream node;

  const list<AttributeTypeASPathSegmentPtr>& pathSegments =
    dynamic_pointer_cast<AttributeTypeASPath>(attr_type)->getPathSegmentsComplete();

  string head = "";
  string tail = "";
  string sep = " ";

  bool first_seg = true;
  BOOST_FOREACH (AttributeTypeASPathSegmentPtr as_seg, pathSegments) {
    if (first_seg)
      first_seg = false;
    else
      node << " ";

    uint8_t pathSegmentType = as_seg->getPathSegmentType();

    if (!showComplete) {
      switch (pathSegmentType) {
      case AttributeTypeASPathSegment::AS_CONFED_SEQUENCE:
        continue;
        break; // CONF is not supported in ASCII
      case AttributeTypeASPathSegment::AS_CONFED_SET:
        continue;
        break; // CONF is not supported in ASCII
      default:
        break; //
      }
    }
    switch (pathSegmentType) {
    case AttributeTypeASPathSegment::AS_SEQUENCE:
      head = "";
      tail = "";
      sep = " ";
      break;
    case AttributeTypeASPathSegment::AS_SET:
      head = "{";
      tail = "}", sep = ",";
      break;
    //[TO_DO] conf_set / conf_sequence
    case AttributeTypeASPathSegment::AS_CONFED_SEQUENCE:
      head = "(";
      tail = ")";
      sep = " ";
      break; //[TODO]
    case AttributeTypeASPathSegment::AS_CONFED_SET:
      head = "[";
      tail = "]";
      sep = ",";
      break; //[TODO]
    default:
      head = "";
      tail = "";
      sep = " ";
      break;
    }

    node << head;
    bool isFirst = true;
    BOOST_FOREACH (uint32_t asn, as_seg->getPathSegmentValue()) {
      if (isFirst)
        isFirst = false;
      else
        node << sep;
      node << asn;
    }
    node << tail;
  }

  return node.str();
}

// vim: sw=4 ts=4 sts=4 expandtab
