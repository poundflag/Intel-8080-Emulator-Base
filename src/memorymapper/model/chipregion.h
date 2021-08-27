#include "base/memoryChip.h"

#ifndef __CHIPREGION_H__
#define __CHIPREGION_H__

class ChipRegion {
private:
  MemoryChip chip;
  int startAddress;
  int endAddress;

public:
  ChipRegion(int startAddress, int endAddress);
  MemoryChip getChip();
  int getStartAddress();
  int endStartAddress();
};

#endif // __CHIPREGION_H__