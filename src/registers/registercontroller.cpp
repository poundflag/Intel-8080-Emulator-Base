#include "registercontroller.h"

RegisterController::RegisterController(BusController &busController)
    : busController(busController) {

  for (int i = 0; i <= Registers::L; i++) {
    registers[i] = new Register();
  }

  registers[Registers::M] = new MemoryReference(
      busController, *registers[Registers::H], *registers[Registers::L]);
}

// Get a register
Register &RegisterController::getRegister(Registers::Register reg) {
  return *registers[reg];
}

FlagRegister &RegisterController::getFlagRegister() { return flagRegister; }

Stack &RegisterController::getStack() { return stackRegister; }

uint16_t RegisterController::getRegisterPair(RegisterPair registerPair) {
  switch (registerPair) {
  case B:
    return (registers[Registers::B]->getRegister() << 8) |
           registers[Registers::C]->getRegister();
    break;
  case D:
    return (registers[Registers::D]->getRegister() << 8) |
           registers[Registers::E]->getRegister();
    break;
  case H:
    return (registers[Registers::H]->getRegister() << 8) |
           registers[Registers::L]->getRegister();
    break;
  case PSW:
    return (registers[Registers::A]->getRegister() << 8) |
           getFlagRegister().getRegister();
    break;
  case SP:
    return stackRegister.getStackPointer();
    break;
  };
  return 0;
}

void RegisterController::setRegisterPair(RegisterPair registerPair,
                                         uint16_t immediate) {
  uint8_t higherByte = immediate & 0xFF;
  uint8_t lowerByte = immediate >> 8;
  switch (registerPair) {
  case B:
    registers[Registers::B]->setRegister(lowerByte);
    registers[Registers::C]->setRegister(higherByte);
    break;
  case D:
    registers[Registers::D]->setRegister(lowerByte);
    registers[Registers::E]->setRegister(higherByte);
    break;
  case H:
    registers[Registers::H]->setRegister(lowerByte);
    registers[Registers::L]->setRegister(higherByte);
    break;
  case PSW:
    registers[Registers::A]->setRegister(lowerByte);
    getFlagRegister().setRegister(higherByte);
    break;
  case SP:
    stackRegister.setStackPointer(immediate);
    break;
  };
}

RegisterController::~RegisterController() {
  for (int i = 0; i <= Registers::M; i++) {
    delete registers[i];
  }
}
