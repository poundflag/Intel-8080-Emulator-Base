#include "../../bus/buscontroller.h"
#include "register.h"

#ifndef __STACK_H__
#define __STACK_H__

class Stack : public Register {
private:
  uint16_t stackPointer;
  std::shared_ptr<BusController> busController;
  void incrementStackPointer();
  void decrementStackPointer();

public:
  Stack(std::shared_ptr<BusController> busController); // TODO Implement derived methods
  void pushByte(uint8_t value);
  uint8_t popByte();
  void pushWord(uint16_t value);
  uint16_t popWord();
  void setRegister(uint8_t value) override;
  uint8_t getRegister() override;
};

#endif // __STACK_H__