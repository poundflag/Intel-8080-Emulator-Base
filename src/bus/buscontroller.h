#include "model/chipregion.h"
#include <iostream>
#include <vector>

#ifndef __BUSCONTROLLER_H__
#define __BUSCONTROLLER_H__

// Inspired from
// https://github.com/LowLevelJavaScript/16-Bit-Virtual-Machine/blob/master/episode-5/memory-mapper.js
class BusController {
private:
  std::vector<ChipRegion> chipRegions;
  ChipRegion& findChipRegion(uint16_t address); // Return a reference??

public:
  void addChipRegion(ChipRegion chipRegion);

  uint16_t readWord(uint16_t address);
  void writeWord(uint16_t address, uint16_t value);
  uint8_t readByte(uint16_t address);
  void writeByte(uint16_t address, uint8_t value);
};

#endif // __BUSCONTROLLER_H__