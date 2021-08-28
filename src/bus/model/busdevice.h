#include <iostream>

#ifndef __BUSDEVICE_H__
#define __BUSDEVICE_H__

class BusDevice {
    public:
        void write(int address, uint8_t value);
        uint8_t read(int address);
};
#endif // __BUSDEVICE_H__