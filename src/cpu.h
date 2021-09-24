#include "registers/registercontroller.h";
#include "bus/buscontroller.h"

#ifndef __CPU_H__
#define __CPU_H__

class Cpu {
    private:
    void instructionDecoder();
    RegisterController registerController;
    BusController busController;
    public:
    Cpu();
    ~Cpu();
    void cycle();
};

#endif // __CPU_H__