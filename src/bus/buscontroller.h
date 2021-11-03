#include "model/chipregion.h"
#include <iostream>
#include <vector>

#ifndef __BUSCONTROLLER_H__
#define __BUSCONTROLLER_H__

class BusController {
private:
  std::vector<ChipRegion>
      chipRegions;
  ChipRegion &findChipRegion(uint16_t address);

  // Nullpointer for an unsuccessful chip return
  ChipRegion nullChip = ChipRegion(0, 0, nullptr);

public:
  void addChipRegion(ChipRegion chipRegion);
  void addChipRegion(int startAddress, int endAddress, BusDevice *chip);

  uint16_t readWord(uint16_t address);
  void writeWord(uint16_t address, uint16_t value);
  uint8_t readByte(uint16_t address);
  void writeByte(uint16_t address, uint8_t value);
  ~BusController();
};

#endif // __BUSCONTROLLER_H__