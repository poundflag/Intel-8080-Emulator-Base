#include "ramdebug.h"
#include <fstream>
#include <iostream>
#include <string>

RamDebug::RamDebug(std::string fullFilePath) { loadRomFile(fullFilePath); }

void RamDebug::loadRomFile(std::string fullFilePath) {

  std::ifstream myfile(fullFilePath, std::ios::binary | std::ios::ate);
  // Initialize new array with the size of file
  ram = new int[determineSize(myfile)];
  int i = 0;
  if (myfile.is_open()) {

    // While there are still bytes left to read
    while (myfile.good()) {
      // Add the byte to the ram array
      ram[i] = myfile.get();
      i++;
    }
    myfile.close();
  }

  else {
    std::cout << "Unable to open file";
  }

  for (int a = 0; a < 9; a++) {
    std::cout << std::hex << int(ram[a]) << " ";
  }
}

int RamDebug::determineSize(std::ifstream &file) {
  // If the file is open
  if (file.is_open()) {
    // Get the size of file
    int fileSize = file.tellg();
    // Reset its cursor position
    file.seekg(0);
    return fileSize;
  }
  return -1;
}

void RamDebug::write(uint16_t address, uint8_t value) { ram[address] = value; }

uint8_t RamDebug::read(uint16_t address) { return ram[address]; }

RamDebug::~RamDebug() { delete ram; }
