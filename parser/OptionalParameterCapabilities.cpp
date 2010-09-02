
#include <bgpparser.h>

#include "OptionalParameterCapabilities.h"
using namespace std;

#include <boost/iostreams/read.hpp>
namespace io = boost::iostreams;

OptionalParameterCapabilities::~OptionalParameterCapabilities( )
{

}

OptionalParameterCapabilities::OptionalParameterCapabilities( OptionalParameter &header, std::istream &input )
: OptionalParameter( header )
{
	code  =input.get( );
	length=input.get( );

	if( length>0 )
	{
		value=boost::shared_ptr<uint8_t>( new uint8_t[length] );
		io::read( input, reinterpret_cast<char*>(value.get()), length );
	}
}
