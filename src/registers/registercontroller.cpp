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
  case B:
    return (registers[Registers::B].getRegister() << 8) |
           registers[Registers::C].getRegister();
    break;
  case D:
    return (registers[Registers::D].getRegister() << 8) |
           registers[Registers::E].getRegister();
    break;
  case H:
    return (registers[Registers::H].getRegister() << 8) |
           registers[Registers::L].getRegister();
    break;
  case PSW:
    return (registers[Registers::A].getRegister() << 8) |
           getFlagRegister().getRegister();
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
    registers[Registers::B].setRegister(lowerByte);
    registers[Registers::C].setRegister(higherByte);
    break;
  case D:
    registers[Registers::D].setRegister(lowerByte);
    registers[Registers::E].setRegister(higherByte);
    break;
  case H:
    registers[Registers::H].setRegister(lowerByte);
    registers[Registers::L].setRegister(higherByte);
    break;
  case PSW:
    registers[Registers::A].setRegister(lowerByte);
    getFlagRegister().setRegister(higherByte);
    break;
  };
}

RegisterController::~RegisterController() { delete[] registers; }
