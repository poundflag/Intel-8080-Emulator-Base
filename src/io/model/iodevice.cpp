#include "iodevice.h"

IODevice::IODevice(uint8_t portAddress) {
  this->portAddress = portAddress;
  value = 0;
}

void IODevice::setValue(uint8_t value) {
  this->value = value;
}

uint8_t IODevice::getValue() { return value; }

uint8_t IODevice::getPortAddress() { return portAddress; }