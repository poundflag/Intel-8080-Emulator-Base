#include "buscontroller.h"

BusController::BusController() {}

// Find the object of a given address
ChipRegion *BusController::findChipRegion(uint16_t address) {
  for (auto &chip : chipRegions) {
    if (chip != nullptr && address >= chip->getStartAddress() &&
        address <= chip->getEndAddress()) {
      return chip;
    }
  }

  return nullptr;
}

void BusController::addChipRegion(int startAddress, int endAddress,
                                  BusDevice *chip) {
  for (int i = 0; i < MAX_CHIPS; i++) {
    if (chipRegions[i] == nullptr) {
      chipRegions[i] = new ChipRegion(startAddress, endAddress, chip);
      break;
    }
  }
}

uint16_t BusController::readWord(uint16_t address) {
  uint8_t msb = readByte(address);
  uint8_t lsb = readByte(address + 1);
  uint16_t value = (lsb << 8) + msb;
  return value;
}

void BusController::writeWord(uint16_t address, uint16_t value) {
  uint8_t msb = value & 0x00FF;
  uint8_t lsb = value >> 8;
  writeByte(address, msb);
  writeByte(address + 1, lsb);
}

uint8_t BusController::readByte(uint16_t address) {
  ChipRegion *chip = findChipRegion(address);
  if (chip != nullptr) {
    return chip->getChip().read(address - chip->getStartAddress());
  }
  return 0;
}

void BusController::writeByte(uint16_t address, uint8_t value) {
  ChipRegion *chip = findChipRegion(address);
  if (chip != nullptr) {
    chip->getChip().write(address - chip->getStartAddress(), value);
  }
}

BusController::~BusController() {
  for (auto chip : chipRegions) {
    delete chip;
  }
}
