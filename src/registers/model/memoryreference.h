#include "../../bus/buscontroller.h"
#include "register.h"
#include <iostream>

#ifndef __MEMORYREFERENCE_H__
#define __MEMORYREFERENCE_H__

class MemoryReference : public Register {
private:
  BusController &busController;
  Register &hRegister;
  Register &lRegister;
  uint16_t getAddress();

public:
  MemoryReference(BusController &busController, Register &hRegister,
                  Register &lRegister);
  void setRegister(uint8_t value); // OVerride the method later TODO 
  uint8_t getRegister();
};

#endif // __MEMORYREFERENCE_H__