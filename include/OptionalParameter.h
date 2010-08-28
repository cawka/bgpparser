
#ifndef _OPTIONALPARAMETER_H_
#define _OPTIONALPARAMETER_H_

class OptionalParameter;
typedef boost::shared_ptr<OptionalParameter> OptionalParameterPtr;

class OptionalParameter
{
public:
	enum
	{
//		RESERVED=0,
//		AUTHENTIFICATION=1
		CAPABILITIES=2
	};

	uint8_t getType( ) const   { return type; }
	uint8_t getLength( ) const { return length; }
	virtual ~OptionalParameter( );

	static OptionalParameterPtr newOptionalParameter( std::istream &is );

protected:
	OptionalParameter( std::istream &input );

private:
	OptionalParameter( ) { ; }

private:
	uint8_t	type;
	uint8_t length;

	static log4cxx::LoggerPtr Logger;
};

#endif
