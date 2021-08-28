#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include "model/chipregion.h"

#ifndef __BUSCONTROLLER_H__
#define __BUSCONTROLLER_H__

// Inspired from https://github.com/LowLevelJavaScript/16-Bit-Virtual-Machine/blob/master/episode-5/memory-mapper.js
class BusController {
    private:
    std::vector<ChipRegion> chipRegions;
    ChipRegion findChipRegion(int address);
    public:
    BusController();
    void addChipRegion(ChipRegion chipRegion);

    // Getter and setter for 8 bit and 16 bit numbers please



};

#endif // __BUSCONTROLLER_H__