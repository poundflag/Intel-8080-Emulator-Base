#include <iostream>

#ifndef __BUSDEVICE_H__
#define __BUSDEVICE_H__

class BusDevice {
    public:
        virtual void write(uint16_t address, uint8_t value);
        virtual uint8_t read(uint16_t address);
};
#endif // __BUSDEVICE_H__