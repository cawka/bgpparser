
#include <bgpparser.h>

#include "OptionalParameterCapabilities.h"

using namespace std;
namespace io = boost::iostreams;

log4cxx::LoggerPtr OptionalParameterCapabilities::Logger =
  log4cxx::Logger::getLogger("bgpparser.OptionalParameterCapabilities");

OptionalParameterCapabilities::~OptionalParameterCapabilities()
{
}

OptionalParameterCapabilities::OptionalParameterCapabilities(OptionalParameter& header,
                                                             std::istream& input)
  : OptionalParameter(header)
{
  LOG4CXX_TRACE(Logger, "");

  bool error = false;

  error |= sizeof(uint8_t) != io::read(input, reinterpret_cast<char*>(&capCode), sizeof(uint8_t));
  error |= sizeof(uint8_t) != io::read(input, reinterpret_cast<char*>(&capLength), sizeof(uint8_t));

  if (capLength > 0) {
    capValue = std::shared_ptr<uint8_t>(new uint8_t[capLength]);
    error |= capLength != io::read(input, reinterpret_cast<char*>(capValue.get()), capLength);
  }

  if (error) {
    LOG4CXX_ERROR(Logger, "Parsing error");
    throw BGPError();
  }
}
