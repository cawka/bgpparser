
#ifndef _OPTIONALPARAMETERCAPABILITIES_H_
#define _OPTIONALPARAMETERCAPABILITIES_H_

#include "OptionalParameter.h"

/**
 *
 */

class OptionalParameterCapabilities : public OptionalParameter {
public:
  OptionalParameterCapabilities(OptionalParameter& header, std::istream& input);
  virtual ~OptionalParameterCapabilities();

  enum {
    CAPABILITY_AS4 = 65 //[RFC4893]
  };

  uint8_t
  getCapCode() const
  {
    return capCode;
  }
  uint8_t
  getCapLength() const
  {
    return capLength;
  }
  const std::shared_ptr<uint8_t>&
  getCapValue() const
  {
    return capValue;
  }

  virtual void
  accept(Visitor& v)
  {
    v.visit(*this);
  }
  virtual void
  accept(GJVoidVisitor& v, boost::any param)
  {
    v.visit(*this, param);
  }
  virtual boost::any
  accept(GJNoArguVisitor& v)
  {
    return v.visit(*this);
  }
  virtual boost::any
  accept(GJVisitor& v, boost::any param)
  {
    return v.visit(*this, param);
  }

private:
  uint8_t capCode;
  uint8_t capLength;
  std::shared_ptr<uint8_t> capValue;

  static log4cxx::LoggerPtr Logger;
};

typedef std::shared_ptr<OptionalParameterCapabilities> OptionalParameterCapabilitiesPtr;

#endif
