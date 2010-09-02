
#ifndef _OPTIONALPARAMETERCAPABILITIES_H_
#define _OPTIONALPARAMETERCAPABILITIES_H_

#include "OptionalParameter.h"

/**
 *
 */

class OptionalParameterCapabilities : public OptionalParameter
{
public:
	OptionalParameterCapabilities( OptionalParameter &header, std::istream &input );
	virtual ~OptionalParameterCapabilities( );

	enum
	{
		CAPABILITY_AS4=65 //[RFC4893]
	};

private:
	uint8_t capCode;
	uint8_t capLength;
	boost::shared_ptr<uint8_t> capValue;

	static log4cxx::LoggerPtr Logger;
};

typedef boost::shared_ptr<OptionalParameterCapabilities> OptionalParameterCapabilitiesPtr;

#endif
