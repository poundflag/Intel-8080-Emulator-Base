#include "base/memoryChip.h"
#include <iostream>

#ifndef __RAM_H__
#define __RAM_H__

class Ram : public MemoryChip {
private:
  uint8_t *ram;

public:
  Ram(int ramSize);
        void write(int address, uint8_t value);
        uint8_t read(int address);
  ~Ram();
};

#endif // __RAM_H__