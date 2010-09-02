
#include <bgpparser.h>

#include "OptionalParameter.h"
#include "OptionalParameterCapabilities.h"
#include "Exceptions.h"

#include <boost/iostreams/read.hpp>
#include <boost/iostreams/skip.hpp>
namespace io = boost::iostreams;

log4cxx::LoggerPtr OptionalParameter::Logger = log4cxx::Logger::getLogger( "bgpparser.OptionalParameter" );

OptionalParameter::OptionalParameter( std::istream &input )
{
	LOG4CXX_TRACE( Logger, "" );

	bool error=false;

	error|= sizeof(uint8_t)!=
		io::read( input, reinterpret_cast<char*>(&type), sizeof(uint8_t) );
	error|= sizeof(uint8_t)!=
		io::read( input, reinterpret_cast<char*>(&length), sizeof(uint8_t) );

	if( error )
	{
		LOG4CXX_ERROR( Logger, "Parsing error" );
		throw BGPError( );
	}
}

OptionalParameter::~OptionalParameter( )
{
}

OptionalParameterPtr OptionalParameter::newOptionalParameter( std::istream &input )
{
	OptionalParameter header( input );
	OptionalParameterPtr param;

	switch( header.type )
	{
	case OptionalParameter::CAPABILITIES:
		param = OptionalParameterPtr( new OptionalParameterCapabilities(header,input) );
		break;
	default:
		io::detail::skip( input, header.getLength(), boost::mpl::false_() );
		break;
	}

	return param;
}

