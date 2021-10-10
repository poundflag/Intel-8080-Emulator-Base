#include <iostream>

#ifndef __IODEVICE_H__
#define __IODEVICE_H__

class IODevice {
private:
  // LATER INPUT OUTPUT ENUM
  uint8_t portAddress;

protected:
  uint8_t value;

public:
  IODevice(uint8_t portAddress);
  virtual void setValue(uint8_t value);
  virtual uint8_t getValue();
  uint8_t getPortAddress();
};

#endif // __IODEVICE_H__