#include "iodevice.h"

IODevice::IODevice(uint8_t portAddress[], int size) {

  arraySize = size;

  portAddresses = new uint8_t[arraySize];
  for (int i = 0; i < arraySize; i++) {
    this->portAddresses[i] = portAddress[i];
  }
  value = 0;
}

void IODevice::setValue(uint8_t value, uint8_t portAddress) {
  this->value = value;
}

uint8_t IODevice::getValue(uint8_t portAddress) { return value; }

uint8_t *IODevice::getPortAddresses() { return portAddresses; }

int IODevice::getArraySize() { return arraySize; }

IODevice::~IODevice() { delete[] portAddresses; }