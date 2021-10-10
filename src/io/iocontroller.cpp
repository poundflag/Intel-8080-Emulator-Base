#include "iocontroller.h"

IOController::IOController() {
}

IODevice *IOController::findDevice(uint8_t portAddress) {
  return ioDevices[portAddress];
}

void IOController::addIODevice(IODevice *ioDevice) {
  ioDevices[ioDevice->getPortAddress()] = ioDevice;
}

uint8_t IOController::getDeviceValue(uint8_t portAddress) {
  return findDevice(portAddress)->getValue();
}

void IOController::setDeviceValue(uint8_t portAddress, uint8_t value) {
  return findDevice(portAddress)->setValue(value);
}

IOController::~IOController() {
  delete[] ioDevices;
}
