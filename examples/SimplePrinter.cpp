

#include <bgpparser.h>
#include <boost/format.hpp>
#include <boost/asio/ip/address.hpp>

#include "SimplePrinter.h"

using namespace std;

void SimplePrinter::printTimestamp( const MRTMessage &mrt )
{
    /* Format and print the time, "ddd yyyy-mm-dd hh:mm:ss zzz" */
	char rchTime[80];
	time_t      tmTime=mrt.getTimestamp();
    struct tm   *ts;
    ts = localtime(&tmTime);
//	ts = gmtime( &tmTime );
    strftime( rchTime, sizeof(rchTime), "%a %Y-%m-%d %H:%M:%S %Z", ts );
//	strftime( rchTime, sizeof(rchTime), "%m/%d/%y %H:%M:%S", ts );

    cout << "\n" << "TIME: " << rchTime << "\n";
}

void SimplePrinter::visit( MRTBgp4MPMessage &n )
{
	printTimestamp( n );
	cout << "TYPE: BGP4MP/BGP4MP_MESSAGE" << "\n";

	cout << "FROM: " << FORMAT_IP_ADDRESS( n.getPeerIP(),  n.getAddressFamily() ) << " AS" << n.getPeerAS()  << "\n";
	cout << "TO: "   << FORMAT_IP_ADDRESS( n.getLocalIP(), n.getAddressFamily() ) << " AS" << n.getLocalAS() << "\n";

	n.getPayload()->accept( *this );
}

void SimplePrinter::visit( MRTBgp4MPStateChange &n 			)
{
	printTimestamp( n );
	cout << "TYPE: BGP4MP/BGP4MP_STATE_CHANGE" << "\n";
	cout << "FROM: " << FORMAT_IP_ADDRESS( n.getPeerIP(),  n.getAddressFamily() ) << " AS" << n.getPeerAS()  << "\n";
	cout << "TO: "   << FORMAT_IP_ADDRESS( n.getLocalIP(), n.getAddressFamily() ) << " AS" << n.getLocalAS() << "\n";

	cout << "PEER_ASN: " << n.getPeerAS() << endl;
	cout << "OLD_STATE: " << n.getOldState() << endl;
	cout << "NEW_STATE: " << n.getNewState() << endl;
}

void SimplePrinter::visit( MRTBgp4MPMessageAS4 &n 				)
{
	printTimestamp( n );
	cout << "TYPE: BGP4MP/BGP4MP_MESSAGE_AS4" << "\n";
	cout << "FROM: " << FORMAT_IP_ADDRESS( n.getPeerIP(),  n.getAddressFamily() ) << " AS" << n.getPeerAS()  << "\n";
	cout << "TO: "   << FORMAT_IP_ADDRESS( n.getLocalIP(), n.getAddressFamily() ) << " AS" << n.getLocalAS() << "\n";

	n.getPayload()->accept( *this );
}

void SimplePrinter::visit( MRTBgp4MPStateChangeAS4 &n 			)
{
	printTimestamp( n );
	cout << "TYPE: BGP4MP/BGP4MP_STATE_CHANGE_AS4" << "\n";
	cout << "FROM: " << FORMAT_IP_ADDRESS( n.getPeerIP(),  n.getAddressFamily() ) << " AS" << n.getPeerAS()  << "\n";
	cout << "TO: "   << FORMAT_IP_ADDRESS( n.getLocalIP(), n.getAddressFamily() ) << " AS" << n.getLocalAS() << "\n";

	cout << "PEER_ASN: " << n.getPeerAS() << endl;
	cout << "OLD_STATE: " << n.getOldState() << endl;
	cout << "NEW_STATE: " << n.getNewState() << endl;
}

void SimplePrinter::visit( MRTTblDump &n 					)
{
	printTimestamp( n );
	cout << "TYPE: TABLE_DUMP/";
	switch( n.getSubType() )
	{
	case AFI_IPv4:
		cout << "AFI_IPv4";
		break;
	case AFI_IPv6:
		cout << "AFI_IPv6";
		break;
	}
	cout << "\n";

	cout << "FROM: " << FORMAT_IP_ADDRESS( n.getPeerIP(),  n.getSubType() ) << " AS" << n.getPeerAS()  << "\n";

	cout << n.getViewNumber() << "|" << n.getSequenceNumber() << "|"
		 << FORMAT_IP_ADDRESS( n.getPrefix(), n.getSubType() ) << "/" << (int)n.getPrefixLength() << "|"
		 << n.getStatus() << "|"
		 << FORMAT_IP_ADDRESS( n.getPeerIP(), n.getSubType() ) << "|"
		 << n.getPeerAS();

	list<BGPAttributePtr>::const_iterator it;
	for (it = n.getAttributes().begin(); it != n.getAttributes().end(); it++) {
		cout << "|";
		(*it)->accept( *this );
	}

	cout << "\n";
}

void SimplePrinter::visit( MRTTblDumpV2RibHeader &n )
{
	cout << "PREFIX: "
		<< FORMAT_IP_ADDRESS( n.getPrefix(), n.getAFI() ) << "/" << (int)n.getPrefixLength() << endl;
	cout << "SEQUENCE: " << n.getSequenceNumber();
}

void SimplePrinter::visit( MRTTblDumpV2PeerIndexTbl &n  )
{
	printTimestamp( n );
	cout << "TYPE: TABLE_DUMP_V2/PEER_INDEX_TABLE" << "\n";
}

void SimplePrinter::visit( MRTTblDumpV2RibGeneric &n 		)
{
	printTimestamp( n );
	cout << "TYPE: TABLE_DUMP_V2/RIB_GENERIC" << "\n";
	static_cast<MRTTblDumpV2RibHeader>( n ).accept( *this );
}
void SimplePrinter::visit( MRTTblDumpV2RibIPv4Multicast &n 	)
{
	printTimestamp( n );
	cout << "TYPE: TABLE_DUMP_V2/RIB_IPV4_UNICAST" << "\n";
	static_cast<MRTTblDumpV2RibHeader>( n ).accept( *this );
}
void SimplePrinter::visit( MRTTblDumpV2RibIPv4Unicast &n 	)
{
	printTimestamp( n );
	cout << "TYPE: TABLE_DUMP_V2/RIB_IPV4_MULTICAST" << "\n";
	static_cast<MRTTblDumpV2RibHeader>( n ).accept( *this );
}
void SimplePrinter::visit( MRTTblDumpV2RibIPv6Multicast &n 	)
{
	printTimestamp( n );
	cout << "TYPE: TABLE_DUMP_V2/RIB_IPV6_UNICAST" << "\n";
	static_cast<MRTTblDumpV2RibHeader>( n ).accept( *this );
}
void SimplePrinter::visit( MRTTblDumpV2RibIPv6Unicast &n 	)
{
	printTimestamp( n );
	cout << "TYPE: TABLE_DUMP_V2/RIB_IPV6_MULTICAST" << "\n";
	static_cast<MRTTblDumpV2RibHeader>( n ).accept( *this );
}


void SimplePrinter::visit( BGPKeepAlive &n 					)
{
	cout << "BGP MESSAGE TYPE: KEEPALIVE" << "\n";
}

void SimplePrinter::visit( BGPNotification &n 				)
{
	cout << "BGP MESSAGE TYPE: NOTIFICATION" << "\n";
}

void SimplePrinter::visit( BGPOpen &n 						)
{
	cout << "BGP MESSAGE TYPE: OPEN" << "\n";
}
void SimplePrinter::visit( BGPRouteRefresh &n				)
{
	cout << "BGP MESSAGE TYPE: ROUTE_REFRESH" << "\n";
}
void SimplePrinter::visit( BGPUpdate &n 						)
{
	cout << "BGP MESSAGE TYPE: UPDATE" << "\n";

	if( n.getWithdrawnRoutes().size() > 0 )
	{
		cout << "WITHDRAWN:" << endl;
		for( std::list<RoutePtr>::const_iterator routeIter = n.getWithdrawnRoutes().begin( );
				routeIter != n.getWithdrawnRoutes().end( ); routeIter++ )
		{
			cout << "  ";
			(*routeIter)->accept( _prefixPrinter, boost::any_cast<uint16_t>((uint16_t)AFI_IPv4) );
			cout << endl;
		}
	}

	for( std::list<BGPAttributePtr>::const_iterator attrIter = n.getPathAttributes().begin( );
			attrIter != n.getPathAttributes().end( ); attrIter++ )
	{
		(*attrIter)->accept( *this );
	}

	if( n.getNlriRoutes().size( ) > 0 )
	{
		cout << "ANNOUNCE" << endl;
		for( std::list<RoutePtr>::const_iterator routeIter = n.getNlriRoutes().begin( );
				routeIter != n.getNlriRoutes().end( ); routeIter++ )
		{
			cout << "  ";
			(*routeIter)->accept( _prefixPrinter, boost::any_cast<uint16_t>((uint16_t)AFI_IPv4) );
			cout << endl;
		}
	}
}


void SimplePrinter::visit( AttributeTypeMPReachNLRI &n )
{
	if( n.getAFI()==0 ) return;

	cout << "ANNOUNCED:" << endl;
	BOOST_FOREACH( const NLRIReachablePtr &route, n.getNLRI() )
	{
		cout << "  ";
		route->accept( _prefixPrinter, boost::any_cast<uint16_t>(n.getAFI( )) );
		cout << endl;
	}
}

void SimplePrinter::visit( AttributeTypeMPUnreachNLRI &n )
{
	if( n.getAFI()==0 ) return;

	cout << "WITHDRAWN:" << endl;
	BOOST_FOREACH( const NLRIUnReachablePtr &route, n.getNLRI() )
	{
		cout << "  ";
		route->accept( _prefixPrinter, boost::any_cast<uint16_t>(n.getAFI( )) );
		cout << endl;
	}
}

void SimplePrefixPrinter::visit( Route &n, boost::any param )
{
	uint16_t afi=boost::any_cast<uint16_t>(param);

	std::cout
		<< FORMAT_IP_ADDRESS( n.getPrefix(), afi ) << "/" << (int)n.getLength( );
}

void SimplePrefixPrinter::visit( NLRIReachable &n, 	boost::any param )
{
	static_cast<Route>( n ).accept( *this, param );
}

void SimplePrefixPrinter::visit( NLRIUnReachable &n, boost::any param )
{
	static_cast<Route>( n ).accept( *this, param );
}

void SimplePrinter::visit( AttributeType &n )
{
	cout << "UNKNOWN_ATTRIBUTE";
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////// ORIGIN //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void SimplePrinter::visit( AttributeTypeOrigin &n )
{
	cout << "ORIGIN: ";

	switch( n.getOrigin( ) )
	{
	case AttributeTypeOrigin::IGP:
		cout << "IGP";
		break;
	case AttributeTypeOrigin::EGP:
		cout << "EGP";
		break;
	case AttributeTypeOrigin::INCOMPLETE:
	default:
		cout << "INCOMPLETE";
		break;
	}
	cout << endl;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////// AS PATH /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void SimplePrinter::visit( AttributeTypeASPath &n )
{
	std::ostringstream os;

	bool isFirst=true;
	BOOST_FOREACH( const AttributeTypeASPathSegmentPtr &segment, /**/n.getPathSegmentsComplete()/**/ )
	{
		if( isFirst )
			isFirst=false;
		else
			os << " ";

		segment->accept( _asSegmentVisitor, static_cast<std::ostream*>(&os) );
	}

	cout << "ASPATH: " << os.str( ) << endl;
}

void SimplePrinter::visit( AttributeTypeAS4Path &n )
{
	std::ostringstream os;

	bool isFirst=true;
	BOOST_FOREACH( const AttributeTypeASPathSegmentPtr &segment, /**/n.getPathSegments()/**/ )
	{
		if( isFirst )
			isFirst=false;
		else
			os << " ";

		segment->accept( _asSegmentVisitor, static_cast<std::ostream*>(&os) );
	}

	cout << "AS4_PATH: " << os.str( ) << endl;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////// NEXT HOP ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void SimplePrinter::visit( AttributeTypeNextHop &n )
{
	cout << "NEXT_HOP: " << FORMAT_IP_ADDRESS( n.getNextHopIPAddress(), n.getNextHopAFI() ) << endl;
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// MultiExitDisc /////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void SimplePrinter::visit( AttributeTypeMultiExitDisc &n )
{
	cout << "MULTI_EXIT_DISC: " << n.getMultiExitDiscValue( ) << endl;
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// LOCAL PREF ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void SimplePrinter::visit( AttributeTypeLocalPref &n )
{
	cout << "LOCAL_PREF: " << n.getLocalPrefValue( ) << endl;
}

//////////////////////////////////////////////////////////////////////////////
/////////////////////////// ATOMIC AGGREGATE ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void SimplePrinter::visit( AttributeTypeAtomicAggregate &n )
{
	cout << "ATOMIC_AGGREGATE" << endl;
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// AGGREGATOR ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void SimplePrinter::visit( AttributeTypeAggregator &n )
{
	cout << "AGGREGATOR: "
	     << "AS"<<(int)n.getAggregatorLastASComplete( ) << " "
	     << n.getAggregatorBGPSpeakerIPAddress().ipv4 << endl;
}

void SimplePrinter::visit( AttributeTypeAS4Aggregator &n )
{
	cout << "AS4_AGGREGATOR: " 
	     << "AS"<<(int)n.getAggregatorLastAS( ) << " "
	     << n.getAggregatorBGPSpeakerIPAddress().ipv4 << endl;
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// COMMUNITIES ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void SimplePrinter::visit( AttributeTypeCommunities &n )
{
	std::ostringstream os;
	std::string sep  = "";

	BOOST_FOREACH( const CommunityValue &value, n.getCommunityValues() )
	{
		if( value.ASnum == 0xFFFF && value.info == 0xFF01 )
		{
			os << sep << "no-export";
		}
		else
		{
			os << sep << (int)value.ASnum << ":" << (int)value.info;
		}
		sep = " ";
	}

	cout << "COMMUNITIES: " << os.str( ) << endl;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////// EXT COMMUNITIES /////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void SimplePrinter::visit( AttributeTypeExtCommunities &n		)
{
	std::ostringstream os;
	std::string sep  = "";

	BOOST_FOREACH( const ExtCommunityValue &value, n.getExtCommunityValues() )
	{
		os << sep << (int)value.typeHigh << ":" << (int)value.typeLow << ":";
		os << boost::format("%02x%02x%02x%02x%02x%02x") %
					(int)value.rchValue[0] % (int)value.rchValue[1] % (int)value.rchValue[2] %
					(int)value.rchValue[3] % (int)value.rchValue[4] % (int)value.rchValue[5];
		sep = " ";
	}

	cout << "EXT_COMMUNITIES: " << os.str( ) << endl;
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// CLUSTER LIST //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void SimplePrinter::visit( AttributeTypeClusterList &n			)
{
	cout << "CLUSTER_LIST: ";

	std::string sep  = "";
	BOOST_FOREACH( uint32_t cluster, n.getClusterList() )
	{
		cout << sep << boost::asio::ip::address_v4( ntohl(cluster) );
		sep = " ";
	}
	cout << endl;
}

//////////////////////////////////////////////////////////////////////////////
///////////////////////////// ORIGINATOR ID //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void SimplePrinter::visit( AttributeTypeOriginatorID &n		)
{
	cout << "ORIGINATOR-ID: " << boost::asio::ip::address_v4( ntohl(n.getOrigin()) ) << endl;
}
