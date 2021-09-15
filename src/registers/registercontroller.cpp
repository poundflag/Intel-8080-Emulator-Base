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
  registers[Registers::F] = flagRegister;
  registers[Registers::SP] = stackRegister;
}

Register &RegisterController::get(Registers::Register reg) {
  return registers[reg];
}

FlagRegister &RegisterController::getFlagRegister() { return flagRegister; }

Stack &RegisterController::getStack() { return stackRegister; }

uint16_t RegisterController::getRegisterPair(RegisterPair registerPair) {
  switch (registerPair) {
  B:
    return (registers[Registers::B].getRegister() << 8) |
           registers[Registers::C].getRegister();
    break;
  D:
    return (registers[Registers::D].getRegister() << 8) |
           registers[Registers::E].getRegister();
    break;
  H:
    return (registers[Registers::H].getRegister() << 8) |
           registers[Registers::L].getRegister();
    break;
  PSW:
    return (registers[Registers::A].getRegister() << 8) |
           registers[Registers::F].getRegister();
    break;
  };
  return 0;
}

void RegisterController::setRegisterPair(RegisterPair registerPair,
                                         uint16_t immediate) {}

RegisterController::~RegisterController() { delete[] registers; }
