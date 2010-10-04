
#include <bgpparser.h>

#include "ASSegmentVisitor.h"

boost::any ASSegmentVisitor::visit( AttributeTypeASPathSegment &n, boost::any param )
{
	std::string head = "";
	std::string tail = "";
	std::string sep  = " ";

	switch( n.getPathSegmentType() )
	{
		case AttributeTypeASPathSegment::AS_SEQUENCE:          head = "";   tail="";   sep=" ";  break;
		case AttributeTypeASPathSegment::AS_SET:               head = "{";  tail="}",  sep=",";  break;
		case AttributeTypeASPathSegment::AS_CONFED_SEQUENCE:   head = "(";  tail=")";  sep=" ";  break;
		case AttributeTypeASPathSegment::AS_CONFED_SET:        head = "[";  tail="]";  sep=",";  break;
	}

	std::ostream &os=*boost::any_cast<std::ostream*>(param);

	os << head;
	bool isFirst=true;
	BOOST_FOREACH( uint32_t asn, n.getPathSegmentValue() )
	{
		if( isFirst )
			isFirst=false;
		else
			os << sep;

		os << asn;
	}
	os << tail;

	return param;
}
