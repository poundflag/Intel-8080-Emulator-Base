#include "../bus/buscontroller.h"
#include "model/register.h"
#include "model/flagregister.h"
#include "model/stack.h"
#include "registers.h"

#ifndef __REGISTERCONTROLLER_H__
#define __REGISTERCONTROLLER_H__

class RegisterController {
private:
  Register *registers = new Register[10];
  std::shared_ptr<BusController> busController;
  FlagRegister flagRegister;
  Stack stackRegister = Stack(busController);

public:
  RegisterController(std::shared_ptr<BusController> busController);
  Register &get(Registers::Register reg); // TODO Add getter and setter for this
  FlagRegister &getFlagRegister();
  Stack &getStack();
  uint16_t getRegisterPair(RegisterPair registerPair);
  void setRegisterPair(RegisterPair registerPair, uint16_t immediate);
  ~RegisterController();
};
#endif // __REGISTERCONTROLLER_H__
