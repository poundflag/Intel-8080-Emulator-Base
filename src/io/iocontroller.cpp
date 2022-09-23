#include "iocontroller.h"

IOController::IOController() {}

IODevice *IOController::findDevice(uint8_t portAddress) {
  // Loop through each device
  for (int i = 0; i < MAX_DEVICE_SIZE; i++) {
    IODevice *currentDevice = this->ioDevices[i];
    // Check if the device is null
    if (currentDevice != nullptr) {
      uint8_t *currentPortAddresses = currentDevice->getPortAddresses();
      // Look up every port of the device and check if it matches
      for (uint8_t j = 0; j < currentDevice->getArraySize(); j++) {
        if (currentPortAddresses[j] == portAddress) {
          return currentDevice;
        }
      }
    }
  }
  return ioDevices[portAddress];
}

void IOController::addIODevice(IODevice *ioDevice) {
  // Loop through each index and search an emtpy place
  for (int i = 0; i < MAX_DEVICE_SIZE; i++) {
    if (ioDevices[i] == nullptr) {
      ioDevices[i] = ioDevice;
      break;
    }
  }
}

uint8_t IOController::getDeviceValue(uint8_t portAddress) {
  return findDevice(portAddress)->getValue(portAddress);
}

void IOController::setDeviceValue(uint8_t portAddress, uint8_t value) {
  return findDevice(portAddress)->setValue(value, portAddress);
}

IOController::~IOController() {
  /*for (auto ioDevice : ioDevices) {
    delete ioDevice;
  }*/
}
