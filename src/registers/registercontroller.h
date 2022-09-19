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
  uint8_t registers[Registers::MemoryReference] = {0};
  BusController &busController;
  FlagRegister flagRegister = FlagRegister();
  Stack stackRegister = Stack(busController);

public:
  RegisterController(BusController &busController);

  uint8_t getRegister(Registers::Register registerIndex);
  void setRegister(Registers::Register registerIndex, uint8_t value);

  uint16_t getRegisterPair(RegisterPair registerPair);
  void setRegisterPair(RegisterPair registerPair, uint16_t value);

  FlagRegister &getFlagRegister();
  Stack &getStack();
  ~RegisterController();
};
#endif // __REGISTERCONTROLLER_H__
