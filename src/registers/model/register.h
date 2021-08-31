#include <iostream>

#ifndef __REGISTER_H__
#define __REGISTER_H__

class Register {
protected:
  uint8_t value;

public:
  Register();
  virtual void setRegister(uint8_t value);
  virtual uint8_t getRegister();
};

#endif // __REGISTER_H__