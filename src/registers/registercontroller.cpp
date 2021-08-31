#include "registercontroller.h"
#include "model/flagregister.h"
#include "model/memoryreference.h"
#include "model/stack.h"

RegisterController::RegisterController(BusController &busController)
    : busController(busController) {

  for (int i = 0; i < Registers::L; i++) {
    registers[i] = new Register();
  }

  registers[Registers::M] = new MemoryReference(busController);
  registers[Registers::PSW] = new FlagRegister();
  registers[Registers::SP] = new Stack(busController);
}

RegisterController::~RegisterController() { delete[] registers; }
