#include "registercontroller.h"
#include "model/flagregister.h"
#include "model/memoryreference.h"
#include "model/stack.h"
#include "registers.h"

RegisterController::RegisterController(
    std::shared_ptr<BusController> busController)
    : busController(move(busController)) {

  for (int i = 0; i < Registers::L; i++) { // TODO Necessary??
    registers[i] = Register();
  }

  // registers[Registers::M] = new MemoryReference(busController);
  registers[Registers::F] =  FlagRegister();
  registers[Registers::SP] =  Stack(busController);
}

Register &RegisterController::get(Registers::Register reg) {
  return registers[reg];
}

RegisterController::~RegisterController() { delete[] registers; }
