#include "chipregion.h"

ChipRegion::ChipRegion(int startAddress, int endAddress) {
    this -> startAddress = startAddress;
    this -> endAddress = endAddress;
}

MemoryChip ChipRegion::getChip() {
    return chip;
}

int ChipRegion::getStartAddress() {
    return startAddress;
}

int ChipRegion::endStartAddress() {
    return endAddress;
}
