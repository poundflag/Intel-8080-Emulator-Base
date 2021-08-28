#include "ram.h"
#include <iostream>

Ram::Ram(int ramSize) {
    ram = new uint8_t[ramSize](); 
}

void Ram::write(uint16_t address, uint8_t value) {
    ram[address] = value;
}

uint8_t Ram::read(uint16_t address) { return ram[address]; }

Ram::~Ram() {
    delete[] ram;
}
