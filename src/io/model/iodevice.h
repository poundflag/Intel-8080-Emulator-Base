#include <iostream>

#ifndef __IODEVICE_H__
#define __IODEVICE_H__

class IODevice {
private:
  // LATER INPUT OUTPUT ENUM
  uint8_t *portAddresses;
  // Pointer for the current Array-Size of "portAddresses"
  int arraySize = 0;

protected:
  uint8_t value;

public:
  IODevice(uint8_t portAddresses[], int size);
  virtual void setValue(uint8_t value,uint8_t portAddress);
  virtual uint8_t getValue(uint8_t portAddress);
  uint8_t *getPortAddresses();
  int getArraySize();
  ~IODevice();
};

#endif // __IODEVICE_H__