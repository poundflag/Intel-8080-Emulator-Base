#include "../model/busdevice.h"
#include <iostream>

#ifndef __RAM_H__
#define __RAM_H__

class Ram : public BusDevice {
private:
  uint8_t *ram;

public:
  Ram(int ramSize);
  virtual void write(uint16_t address, uint8_t value) override;
  virtual uint8_t read(uint16_t address) override;
  ~Ram();
};

#endif // __RAM_H__