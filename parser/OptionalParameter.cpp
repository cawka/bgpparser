
#include <bgpparser.h>

#include "OptionalParameter.h"
#include "OptionalParameterCapabilities.h"

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
	OptionalParameterPtr header( new OptionalParameter(input) );
	OptionalParameterPtr param;

	switch( header->type )
	{
	case OptionalParameter::CAPABILITIES:
		param = OptionalParameterPtr( new OptionalParameterCapabilities(*header,input) );
		break;
	default:
		io::detail::skip( input, header->getLength(), boost::mpl::false_() );
		param = header;
		break;
	}

	return param;
}

