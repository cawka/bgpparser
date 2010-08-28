
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
	uint8_t code;
	uint8_t length;
	boost::shared_ptr<uint8_t> value;
};

typedef boost::shared_ptr<OptionalParameterCapabilities> OptionalParameterCapabilitiesPtr;

#endif
