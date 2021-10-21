#include "iocontroller.h"

IOController::IOController() {}

IODevice *IOController::findDevice(uint8_t portAddress) {
  int a = deviceIndex;
  for (int i = 0; i < a; i++) {
    IODevice *currentDevice = this->ioDevices[i];
    uint8_t *currentPortAddresses = currentDevice->getPortAddresses();
    int b = currentDevice->getArraySize();
    for (uint8_t j = 0; j < b; j++) {
      if (currentPortAddresses[j] == portAddress) {
        return currentDevice;
      }
    }
  }
  return ioDevices[portAddress];
}

void IOController::addIODevice(IODevice *ioDevice) {
  ioDevices[deviceIndex] = ioDevice;
  deviceIndex++;
}

uint8_t IOController::getDeviceValue(uint8_t portAddress) {
  // return 0;
  return findDevice(portAddress)->getValue(portAddress);
}

void IOController::setDeviceValue(uint8_t portAddress, uint8_t value) {
  return findDevice(portAddress)->setValue(value, portAddress);
}

IOController::~IOController() { delete[] ioDevices; }
