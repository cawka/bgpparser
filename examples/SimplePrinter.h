
#ifndef _SIMPLEPRINTER_H_
#define _SIMPLEPRINTER_H_

#include <visitors/DepthFirstVisitor.h>

class SimplePrinter : public DepthFirstVisitor
{

public:
	virtual void visit( MRTBgp4MPMessage &n 				);
	virtual void visit( MRTBgp4MPStateChange &n 			);
	virtual void visit( MRTBgp4MPMessageAS4 &n 				);
	virtual void visit( MRTBgp4MPStateChangeAS4 &n 			);

	virtual void visit( MRTTblDump &n 						);

	virtual void visit( MRTTblDumpV2RibHeader &n 			);
	virtual void visit( MRTTblDumpV2PeerIndexTbl &n 		);
	virtual void visit( MRTTblDumpV2RibGeneric &n 			);
	virtual void visit( MRTTblDumpV2RibIPv4Multicast &n 	);
	virtual void visit( MRTTblDumpV2RibIPv4Unicast &n 		);
	virtual void visit( MRTTblDumpV2RibIPv6Multicast &n 	);
	virtual void visit( MRTTblDumpV2RibIPv6Unicast &n 		);

	virtual void visit( BGPKeepAlive &n 					);

	virtual void visit( BGPNotification &n 					);
	virtual void visit( BGPOpen &n 							);
	virtual void visit( BGPRouteRefresh &n					);
	virtual void visit( BGPUpdate &n 						);

private:
	void printTimestamp( const MRTMessage &mrt );
};

#endif //_SIMPLEPRINTER_H_
