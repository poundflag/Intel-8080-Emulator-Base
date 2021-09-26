#include "chipregion.h"

ChipRegion::ChipRegion(int startAddress, int endAddress, BusDevice *chip) {
  this->startAddress = startAddress;
  this->endAddress = endAddress;
  this->chip = chip;
}

BusDevice *ChipRegion::getChip() { return chip; }

int ChipRegion::getStartAddress() { return startAddress; }

int ChipRegion::getEndAddress() { return endAddress; }
