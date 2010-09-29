
#include <bgpparser.h>
#include <visitors/GJVoidDepthFirst.h>

#include "AttrsVisitor.h"

#include "Params.h"


class AsciiBGPVisitor : public AttrsVisitor
{
	virtual void visit( BGPOpen &n, boost::any &param )
	{
		// prevent visiting optional headers
	}

	virtual void visit( BGPUpdate &n, boost::any &param )
	{
		BOOST_FOREACH( const BGPAttributePtr &attr, n.getPathAttributes() )
		{
			attr->accept( *this, param );
		}

		param.afi=AFI_IPv4;

		BOOST_FOREACH( const RoutePtr &route, n.getWithdrawnRoutes() )
		{
			route->accept( *this, param );
		}

		BOOST_FOREACH( const RoutePtr &route, n.getNlriRoutes() )
		{
			route->accept( *this, param );
		}
	}

	virtual void visit( AttributeTypeMPReachNLRI &n, boost::any &param )
	{
		param.afi      = n.getAFI( );
		param.next_hop = FORMAT_IP_ADDRESS( n.getNextHopAddress(), n.getAFI() );

		BOOST_FOREACH( const NLRIReachablePtr& route, n.getNLRI() )
		{
			route->accept( *this, param );
		}

		BOOST_FOREACH( const NLRIReachablePtr& route, n.getSNPA() )
		{
			route->accept( *this, param );
		}
	}
	virtual void visit( AttributeTypeMPUnreachNLRI &n, boost::any &param )
	{
		param.afi = n.getAFI( );

		BOOST_FOREACH( const NLRIUnReachablePtr &route, n.getNLRI() )
		{
			route->accept( *this, param );
		}
	}

	virtual void visit( Route &n, boost::any &param )
	{
//		n.getLength( )
//		n.getPrefix( )
//		n.getNumOctets( )
//		+ helper methods available, see Route
	}

	virtual void visit( NLRIReachable &n, boost::any &param )
	{
//		n.getLength( )
//		n.getPrefix( )
//		n.getNumOctets( )
//		+ helper methods available, see Route
	}
	virtual void visit( NLRIUnReachable &n, boost::any &param )
	{
		std::cout
			<< param.mrt_type << "|" << param.timestamp << "|"
			<< "W" + "|" + param.peer_addr + "|" + param.peer_as + "|" + n.toString( params.afi )
			<< std::endl;
	}
};
