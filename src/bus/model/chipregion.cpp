#include "chipregion.h"

ChipRegion::ChipRegion(int startAddress, int endAddress) {
    this -> startAddress = startAddress;
    this -> endAddress = endAddress;
}

BusDevice ChipRegion::getChip() {
    return chip;
}

int ChipRegion::getStartAddress() {
    return startAddress;
}

int ChipRegion::endStartAddress() {
    return endAddress;
}
