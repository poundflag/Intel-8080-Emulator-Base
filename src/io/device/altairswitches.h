#include "../model/iodevice.h"

#ifndef __ALTAIRSWITCHES_H__
#define __ALTAIRSWITCHES_H__

class AltairSwitches : public IODevice {

public:
  AltairSwitches(uint8_t portAddress[], int size);
  void setValue(uint8_t value, uint8_t portAddress) override;
  uint8_t getValue(uint8_t portAddress) override;
};

#endif // __ALTAIRSWITCHES_H__