
#include "../bus/buscontroller.h"
#include "model/register.h"

#ifndef __REGISTERCONTROLLER_H__
#define __REGISTERCONTROLLER_H__

class RegisterController {
private:
  Register *registers[10];
  BusController &busController;

public:
  RegisterController(BusController &busController);
  ~RegisterController();
};
#endif // __REGISTERCONTROLLER_H__

enum Registers {
  A = 0,
  B,
  C,
  D,
  E,
  H,
  L,
  M,
  PSW,
  SP
}; // TODO Rename the class to RegisterService??