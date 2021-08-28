#include "buscontroller.h"

ChipRegion& BusController::findChipRegion(uint16_t address) {
  for (auto &chip : chipRegions) {
    if (address >= chip.getStartAddress() && address <= chip.getEndAddress()) {
      return chip;
    }
  }

  return chipRegions.at(0); // TODO Change later to a real exception
}

void BusController::addChipRegion(ChipRegion chipRegion) {
  chipRegions.push_back(chipRegion); // TODO Add a overlapping option later
}

uint16_t BusController::readWord(uint16_t address) {
  ChipRegion chip = findChipRegion(address);
  uint16_t value =
      chip.getChip()->read(address + 1) + (chip.getChip()->read(address) >> 8);
  return value;
}

void BusController::writeWord(uint16_t address, uint16_t value) {
  ChipRegion chip = findChipRegion(address);
  chip.getChip()->write(address, value << 8);
  chip.getChip()->write(address + 1, value & 8);
}

uint8_t BusController::readByte(uint16_t address) {
  ChipRegion chip = findChipRegion(address);
  return chip.getChip()->read(address);
}

void BusController::writeByte(uint16_t address, uint8_t value) {
  ChipRegion& chip = findChipRegion(address);
  chip.getChip()->write(address, value);
}
