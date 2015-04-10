
#include <bgpparser.h>

#include "AsciiVisitor.h"
#include "Params.h"

#include <sstream>

void
AsciiVisitor::visit(MRTBgp4MPMessage& n)
{
  n.getPayload()->accept(_bgpVisitor,
                         InfoPtr(new Info("BGP4MP", n.getTimestamp(), "A", n.getPeerIP(),
                                          n.getAddressFamily(), n.getPeerAS())));
}

void
AsciiVisitor::visit(MRTBgp4MPStateChange& n)
{
  std::cout << "BGP4MP"
            << "|" << (int)n.getTimestamp() << "|"
            << "STATE"
            << "|" << FORMAT_IP_ADDRESS(n.getPeerIP(), n.getAddressFamily()) << "|" << n.getPeerAS()
            << "|" << (int)n.getOldState() << "|" << (int)n.getNewState() << "\n";
}

void
AsciiVisitor::visit(MRTTblDump& n)
{
  n.accept(_bgpVisitor, InfoPtr(new Info("TABLE_DUMP", n.getTimestamp(), "B", n.getPeerIP(),
                                         n.getSubType(), n.getPeerAS())));
}

void
AsciiVisitor::visit(MRTTblDumpV2RibHeader& n)
{
  n.accept(_bgpVisitor, InfoPtr(new Info("TABLE_DUMP2", n.getTimestamp(), "B", IPAddress(), 0, 0)));
}
