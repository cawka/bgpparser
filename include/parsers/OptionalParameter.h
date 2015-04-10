
#ifndef _OPTIONALPARAMETER_H_
#define _OPTIONALPARAMETER_H_

class OptionalParameter;
typedef std::shared_ptr<OptionalParameter> OptionalParameterPtr;

class OptionalParameter : public Node {
public:
  enum {
    //		RESERVED=0,
    //		AUTHENTIFICATION=1
    CAPABILITIES = 2
  };

  uint8_t
  getType() const
  {
    return type;
  }
  uint8_t
  getLength() const
  {
    return length;
  }
  virtual ~OptionalParameter();

  static OptionalParameterPtr
  newOptionalParameter(std::istream& is);

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

protected:
  OptionalParameter(std::istream& input);

private:
  OptionalParameter()
  {
    ;
  }

private:
  uint8_t type;
  uint8_t length;

  static log4cxx::LoggerPtr Logger;
};

#endif
