#include "busdevice.h"

#ifndef __CHIPREGION_H__
#define __CHIPREGION_H__

class ChipRegion {
private:
  BusDevice chip;
  int startAddress;
  int endAddress;

public:
  ChipRegion(int startAddress, int endAddress);
  BusDevice getChip();
  int getStartAddress();
  int endStartAddress();
};

#endif // __CHIPREGION_H__