
#ifndef _ASCII_ATTRS_VISITOR_H_
#define _ASCII_ATTRS_VISITOR_H_

#include <visitors/GJDepthFirst.h>

class ASSegmentVisitor : public GJDepthFirst
{
	virtual boost::any& visit( AttributeTypeASPathSegment &n, boost::any &param )
	{
		string head = "";
		string tail = "";
		string sep  = " ";

		switch( n.getPathSegmentType() )
		{
			case AttributeTypeASPathSegment::AS_SEQUENCE:          head = "";   tail="";   sep=" ";  break;
			case AttributeTypeASPathSegment::AS_SET:               head = "{";  tail="}",  sep=",";  break;
			case AttributeTypeASPathSegment::AS_CONFED_SEQUENCE:   head = "(";  tail=")";  sep=" ";  break;
			case AttributeTypeASPathSegment::AS_CONFED_SET:        head = "[";  tail="]";  sep=",";  break;
		}

	    ostringstream os;

		os << head;
		bool isFirst=true;
		BOOST_FOREACH( uint32_t asn, as_seg->getPathSegmentValue() )
		{
			if( isFirst )
				isFirst=false;
			else
				os << sep;

			os << asn;
		}
		os << tail;
	}
};

#endif
