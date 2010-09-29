
#include <visitors/DepthFirstVisitor.h>

#include "AsciiBGPVisitor.h"

#include <sstream>

class AsciiVisitor : public DepthFirstVisitor
{
	AsciiBGPVisitor _bgpVisitor;

public:
	virtual void visit( MRTBgp4MPMessage &n 				)
	{
//		n.getTimestamp( )
//		n.getType( )
//		n.getSubType( )
//		n.getLength( )

//		n.getPeerAS()
//		n.getLocalAS()
//		n.getInterfaceIndex()
//		n.getAddressFamily();
//		n.getPeerIP()
//		n.getLocalIP()

	    n.getPayload()->accept( _bgpVisitor, Info(
	    		"BGP4MP",
	    		n.getTimestamp(),
	    		"A",
	    		FORMAT_IP_ADDRESS( n.getPeerIP(), n.getAddressFamily() ),
	    		n.getPeerAS()
	    		) );
	}

	virtual void visit( MRTBgp4MPStateChange &n 			)
	{
//		n.getTimestamp( )
//		n.getType( )
//		n.getSubType( )
//		n.getLength( )

//		n.getPeerAS( )
//		n.getLocalAS( )
//		n.getInterfaceIndex( )
//		n.getAddressFamily( )
//		n.getPeerIP( )
//		n.getLocalIP( )
//		n.getOldState( )
//		n.getNewState( )
	}

	virtual void visit( MRTTblDump &n 					)
	{
//		n.getTimestamp( )
//		n.getType( )
//		n.getSubType( )
//		n.getLength( )

//		n.getViewNumber( )
//		n.getSequenceNumber( )
//		n.getPrefix( )
//		n.getPrefixLength( )
//		n.getStatus( )
//		n.getOriginatedTime( )
//		n.getPeerIP( )
//		n.getPeerAS( )
//		n.getAttributeLength( )
	}

	virtual void visit( MRTTblDumpV2RibHeader &n 			)
	{
//		n.getTimestamp( )
//		n.getType( )
//		n.getSubType( )
//		n.getLength( )

//		n.getSequenceNumber( )
//		n.getPrefixLength( )
//		n.getPrefix( )
//		n.getEntryCount( )
//		n.getAFI( )
//		n.getSAFI( )

//		n.getPeerIndexTbl()

		BOOST_FOREACH( const TblDumpV2RibEntryPtr &entry, n.getRibEntries() )
		{
			entry->accept( *this );
		}
	}
};
