#include "instructions.h"
#include "registers/registers.h"
#include "registers/registercontroller.h"
#include <iostream>

using namespace std;

int main() {
  Instructions instructions;
  shared_ptr<BusController> busController;
  RegisterController registerController = RegisterController(busController);
  registerController.get(Registers::B).setRegister(15);
  instructions.MOV(registerController.get(Registers::A),
                   registerController.get(Registers::B));
  cout << "Hello World and hello "
       << (int)registerController.get(Registers::A).getRegister() << endl;
}