#include "cpu.h"

Cpu::Cpu() {
  registerController = new RegisterController(busController);
  alu = new ALU(*registerController);
  instructions =
      new Instructions(busController, *registerController, ioController, *alu);
  instructionDecoder =
      new InstructionDecoder(*instructions, busController, *registerController);
}

// Continously run the cpu
void Cpu::run() {
    registerController->setRegister(Registers::InstructionRegister, busController.readByte(registerController->getProgramCounter()));
    while (true) {
        bool haltState = cycle();
        if (haltState) {
            break;
        }
  }
}

// Step the cpu in n steps
void Cpu::step(int steps) {
  registerController->fetchNextInstruction();
  for (int i = 0; i < steps; i++) {
    bool haltState = cycle();
    if (haltState) {
      break;
    }
  }
}

// Run a single instruction cycle
// Fetch opcode -> (fetch other bytes) -> decode -> execute -> store
// If a HALT has been received the method returns true
bool Cpu::cycle() {
  // uint8_t opcode = busController.readByte(getProgramCounter());
  uint8_t opcode = registerController->getRegister(Registers::InstructionRegister);
  return instructionDecoder->instructionDecoder(opcode);
}

BusController &Cpu::getBusController() { return busController; }

RegisterController &Cpu::getRegisterController() { return *registerController; }

IOController &Cpu::getIOController() { return ioController; }

uint16_t Cpu::getProgramCounter() { return registerController->getProgramCounter(); }

void Cpu::setProgramCounter(uint16_t programCounter) {
  registerController->getProgramCounter() = programCounter;
}

Cpu::~Cpu() {
  delete instructions;
  delete registerController;
  delete alu;
  delete instructionDecoder;
}
