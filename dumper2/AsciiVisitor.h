
#ifndef _ASCIIVISITOR_H_
#define _ASCIIVISITOR_H_

#include <visitors/DepthFirstVisitor.h>
#include "AsciiBGPVisitor.h"

class AsciiVisitor : public DepthFirstVisitor
{

public:
	virtual void visit( MRTBgp4MPMessage &n 				);

	virtual void visit( MRTBgp4MPStateChange &n 			);

	virtual void visit( MRTTblDump &n 					);

	virtual void visit( MRTTblDumpV2RibHeader &n 			);

private:
	AsciiBGPVisitor _bgpVisitor;
};

#endif
