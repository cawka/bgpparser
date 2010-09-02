
#include <bgpparser.h>

#include "OptionalParameter.h"
#include "OptionalParameterCapabilities.h"

#include <boost/iostreams/read.hpp>
#include <boost/iostreams/skip.hpp>
namespace io = boost::iostreams;

log4cxx::LoggerPtr OptionalParameter::Logger = log4cxx::Logger::getLogger( "bgpparser.OptionalParameter" );

OptionalParameter::OptionalParameter( std::istream &input )
{
	type  =input.get( );
	length=input.get( );
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

