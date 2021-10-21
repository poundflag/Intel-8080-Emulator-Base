#include "../model/iodevice.h"

#ifndef __SERIALOUT_H__
#define __SERIALOUT_H__

class SerialOut : public IODevice {
public:
  SerialOut(uint8_t portAddress[], int size);
  void setValue(uint8_t value, uint8_t portAddress) override;
};

#endif // __SERIALOUT_H__