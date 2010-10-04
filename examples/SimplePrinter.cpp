

#include <bgpparser.h>

#include "SimplePrinter.h"

using namespace std;

void SimplePrinter::printTimestamp( const MRTMessage &mrt )
{
    /* Format and print the time, "ddd yyyy-mm-dd hh:mm:ss zzz" */
	char rchTime[80];
	time_t      tmTime=mrt.getTimestamp();
    struct tm   *ts;
    ts = localtime(&tmTime);
    strftime( rchTime, sizeof(rchTime), "%a %Y-%m-%d %H:%M:%S %Z", ts );

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

	n.printMe( );
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

	n.printMe( );
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
	n.printMeCompact( );
	cout << "\n";
}

void SimplePrinter::visit( MRTTblDumpV2RibHeader &n )
{
	n.printMe( );
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

	n.printMe();
}
