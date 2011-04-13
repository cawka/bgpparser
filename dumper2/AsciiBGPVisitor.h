
#ifndef _ASCIIBGPVISITOR_H_
#define _ASCIIBGPVISITOR_H_

#include <visitors/GJVoidDepthFirst.h>
#include "ASSegmentVisitor.h"

class AsciiBGPVisitor : public GJVoidDepthFirst
{
public:
	virtual void visit( MRTTblDump &n, 					boost::any param );
	virtual void visit( MRTTblDumpV2RibHeader &n,		boost::any param );

	virtual void visit( BGPOpen &n, 					boost::any param );
	virtual void visit( BGPUpdate &n, 					boost::any param );

	virtual void visit( AttributeTypeMPReachNLRI &n, 	boost::any param );
	virtual void visit( AttributeTypeMPUnreachNLRI &n, 	boost::any param );

	virtual void visit( NLRIReachable &n, 				boost::any param );
	virtual void visit( NLRIUnReachable &n, 			boost::any param );

	//////////////////////////////////////////////////////////////////////
	virtual void visit( AttributeTypeOrigin &n, 		boost::any param );
	virtual void visit( AttributeTypeASPath &n, 		boost::any param );
	virtual void visit( AttributeTypeAS4Path &n, 		boost::any param );
	virtual void visit( AttributeTypeNextHop &n, 		boost::any param );
	virtual void visit( AttributeTypeMultiExitDisc &n, 	boost::any param );
	virtual void visit( AttributeTypeLocalPref &n, 		boost::any param );
	virtual void visit( AttributeTypeAtomicAggregate &n,boost::any param );
	virtual void visit( AttributeTypeAS4Aggregator &n,	boost::any param );
	virtual void visit( AttributeTypeAggregator &n, 	boost::any param );
	virtual void visit( AttributeTypeCommunities &n, 	boost::any param );

private:
	ASSegmentVisitor _asSegmentVisitor;
};

#endif
