#include "../bus/buscontroller.h"
#include "model/flagregister.h"
#include "model/memoryreference.h"
#include "model/register.h"
#include "model/stack.h"
#include "registers.h"

#ifndef __REGISTERCONTROLLER_H__
#define __REGISTERCONTROLLER_H__

// Manage the registers of the i8080
class RegisterController {
private:
  Register *registers[8];
  BusController &busController;
  FlagRegister flagRegister;
  Stack stackRegister = Stack(busController);

public:
  RegisterController(BusController &busController);
  Register &get(Registers::Register reg);
  FlagRegister &getFlagRegister();
  Stack &getStack();
  uint16_t getRegisterPair(RegisterPair registerPair);
  void setRegisterPair(RegisterPair registerPair, uint16_t immediate);
  ~RegisterController();
};
#endif // __REGISTERCONTROLLER_H__
