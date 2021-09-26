#include "../../bus/buscontroller.h"
#include "register.h"
#include <iostream>

#ifndef __MEMORYREFERENCE_H__
#define __MEMORYREFERENCE_H__

class MemoryReference : public Register {
private:
  uint16_t getAddress();
  BusController &busController;
  Register &hRegister;
  Register &lRegister;

public:
  MemoryReference(BusController &busController, Register &hRegister,
                  Register &lRegister);
  void setRegister(uint8_t value);
  uint8_t getRegister();
};

#endif // __MEMORYREFERENCE_H__