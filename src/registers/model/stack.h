#include "../../bus/buscontroller.h"
#include "register.h"

#ifndef __STACK_H__
#define __STACK_H__

class Stack : public Register {
private:
  uint16_t stackPointer;
  BusController &busController;
  void incrementStackPointer();
  void decrementStackPointer();

public:
  Stack(BusController &busController); // TODO Implement derived methods
  void pushByte(uint8_t value);
  uint8_t popByte();
  void pushWord(uint16_t value);
  uint16_t popWord();
  void
  setStackPointer(uint16_t value); // Sadly the base method cannot be overriden,
                                   // because the datatypes are no covariants
  uint16_t getStackPointer();
};

#endif // __STACK_H__