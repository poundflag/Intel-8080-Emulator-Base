#include "model/iodevice.h"
#include <iostream>

#ifndef __IOCONTROLLER_H__
#define __IOCONTROLLER_H__

class IOController {
private:
  IODevice **ioDevices = new IODevice *[256];
  IODevice *findDevice(uint8_t portAddress);

public:
  IOController();
  void addIODevice(IODevice *ioDevice);
  uint8_t getDeviceValue(uint8_t portAddress);
  void setDeviceValue(uint8_t portAddress, uint8_t value);
  ~IOController();
};

#endif // __IOCONTROLLER_H__