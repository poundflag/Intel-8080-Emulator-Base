#include "busdevice.h"

#ifndef __CHIPREGION_H__
#define __CHIPREGION_H__

class ChipRegion {
private:
  BusDevice *chip;
  int startAddress;
  int endAddress;

public:
  ChipRegion();
  ChipRegion(int startAddress, int endAddress, BusDevice *chip);
  BusDevice &getChip();
  int getStartAddress();
  int getEndAddress();
  ~ChipRegion();
};

#endif // __CHIPREGION_H__