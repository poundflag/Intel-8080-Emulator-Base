#include "../bus/buscontroller.h"
#include "model/register.h"
#include "registers.h"

#ifndef __REGISTERCONTROLLER_H__
#define __REGISTERCONTROLLER_H__

class RegisterController {
private:
  Register *registers = new Register[10];
  std::shared_ptr<BusController> busController;

public:
  RegisterController(std::shared_ptr<BusController> busController);
  Register &get(Registers::Register reg);
  ~RegisterController();
};
#endif // __REGISTERCONTROLLER_H__
