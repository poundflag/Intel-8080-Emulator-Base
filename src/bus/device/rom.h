#include "../model/busdevice.h"

#ifndef __ROM_H__
#define __ROM_H__

class Rom : public BusDevice {
private:
public:
  // void loadRomFile(string fullFilePath);
  void write(uint16_t address, uint8_t value) override;
  uint8_t read(uint16_t address) override;
};
#endif // __ROM_H__