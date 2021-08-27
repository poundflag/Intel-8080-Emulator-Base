#include <iostream>

#ifndef __MEMORYCHIP_H__
#define __MEMORYCHIP_H__

class MemoryChip {
    public:
        void write(int address, uint8_t value);
        uint8_t read(int address);
};
#endif // __MEMORYCHIP_H__