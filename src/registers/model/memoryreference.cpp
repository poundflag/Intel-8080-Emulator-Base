#include "memoryreference.h"

MemoryReference::MemoryReference(BusController &busController,
                                 Register &hRegister, Register &lRegister)
    : busController(busController), hRegister(hRegister), lRegister(lRegister) {
}

void MemoryReference::setRegister(uint8_t value) {
  busController.writeByte(getAddress(), value);
}

uint8_t MemoryReference::getRegister() {
  return busController.readByte(getAddress());
}

uint16_t MemoryReference::getAddress() {
  return (hRegister.getRegister() << 8) | lRegister.getRegister();
}
