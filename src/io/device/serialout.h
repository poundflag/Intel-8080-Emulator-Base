#include "../model/iodevice.h"

#ifndef __SERIALOUT_H__
#define __SERIALOUT_H__

class SerialOut : public IODevice {
public:
  SerialOut(uint8_t portNumber);
  void setValue(uint8_t value) override;
};

#endif // __SERIALOUT_H__