
#ifndef _ASCII_ATTRS_VISITOR_H_
#define _ASCII_ATTRS_VISITOR_H_

#include <visitors/GJDepthFirst.h>

class ASSegmentVisitor : public GJDepthFirst {
  virtual boost::any
  visit(AttributeTypeASPathSegment& n, boost::any param);
};

#endif
