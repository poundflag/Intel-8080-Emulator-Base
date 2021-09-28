#include "../model/busdevice.h"
#include <iostream>
#include <string>
#include <fstream>

#ifndef __RAMDEBUG_H__
#define __RAMDEBUG_H__

class RamDebug : public BusDevice {
private:
  int *ram;
  int determineSize(std::ifstream &file);
  void loadRomFile(std::string fullFilePath);

public:
  RamDebug(std::string fullFilePath);
  void write(uint16_t address, uint8_t value) override;
  uint8_t read(uint16_t address) override;
  ~RamDebug();
};

#endif // __RAMDEBUG_H__