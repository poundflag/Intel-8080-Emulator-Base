#include "stack.h"

Stack::Stack(std::shared_ptr<BusController> busController)
    : busController(move(busController)) {
  stackPointer = 0;
}

void Stack::incrementStackPointer() { stackPointer++; }

void Stack::decrementStackPointer() { stackPointer--; }

void Stack::pushByte(uint8_t value) {
  decrementStackPointer();
  busController->writeByte(stackPointer, value);
}

uint8_t Stack::popByte() {
  uint16_t result = busController->readByte(stackPointer);
  busController->writeByte(stackPointer, 0);
  incrementStackPointer();
  return result;
}

void Stack::pushWord(uint16_t value) {
  decrementStackPointer();
  decrementStackPointer();
  busController->writeWord(stackPointer, value);
}

uint16_t Stack::popWord() {
  uint16_t result = busController->readWord(stackPointer);
  busController->writeWord(stackPointer, 0);
  incrementStackPointer();
  incrementStackPointer();
  return result;
}

void Stack::setRegister(uint8_t value) { stackPointer = value; }

uint8_t Stack::getRegister() { return stackPointer; }
