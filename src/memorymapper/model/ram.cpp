#include "ram.h"
#include <iostream>

Ram::Ram(int ramSize) {
    ram = new uint8_t[ramSize]; 
}

void Ram::write(int address, uint8_t value) {
    ram[address] = value;
}

uint8_t Ram::read(int address) { return ram[address]; }

Ram::~Ram() {
    delete[] ram;
}
