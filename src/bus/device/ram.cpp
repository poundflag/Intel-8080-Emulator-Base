#include "ram.h"
#include <iostream>

Ram::Ram(uint16_t ramSize) { ram = new uint8_t[ramSize + 1](); }

void Ram::write(uint16_t address, uint8_t value) {
  // std::cout <<"Write " << (int)address << " " << (int)value << std::endl;
  ram[address] = value;
}

uint8_t Ram::read(uint16_t address) {
  /*for (int i = 0; i < 28; i++) {
      std::cout << (int)ram[i] << " ";
  }*/
  // std::cout << "READ " << (int) address << std::endl;
  return ram[address];
}

Ram::~Ram() { 
  delete[] ram; 
  }
