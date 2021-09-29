#include "buscontroller.h"

ChipRegion &BusController::findChipRegion(uint16_t address) {
  for (auto &chip : chipRegions) {
    if (address >= chip.getStartAddress() && address <= chip.getEndAddress()) {
      return chip;
    }
  }
  throw std::runtime_error(
      "Intel 8080 Error: Trying to access a memory address out of reach");
  return chipRegions.at(0); // TODO Change later to a real exception
}

void BusController::addChipRegion(ChipRegion chipRegion) {
  chipRegions.push_back(chipRegion); // TODO Add a overlapping option later
}

void BusController::addChipRegion(int startAddress, int endAddress,
                                  BusDevice *chip) {
  addChipRegion(ChipRegion(startAddress, endAddress, chip));
}

uint16_t BusController::readWord(uint16_t address) {
  // ChipRegion chip = findChipRegion(address);
  uint8_t msb = readByte(address);
  uint8_t lsb = readByte(address + 1);
  uint16_t value = (lsb << 8) + msb;
  return value;
}

void BusController::writeWord(uint16_t address, uint16_t value) {
  ChipRegion &chip = findChipRegion(address);
  uint8_t msb = value & 0x00FF;
  uint8_t lsb = value >> 8;
  writeByte(address, msb);
  writeByte(address + 1, lsb);
}

uint8_t BusController::readByte(uint16_t address) {
  ChipRegion chip = findChipRegion(address);
  return chip.getChip()->read(address);
}

void BusController::writeByte(uint16_t address, uint8_t value) {
  ChipRegion &chip = findChipRegion(address);
  chip.getChip()->write(address, value);
}

BusController::~BusController() {
  for (auto &chip : chipRegions) {
    delete chip.getChip();
  }
}
