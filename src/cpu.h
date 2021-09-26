#include "registers/registercontroller.h";
#include "bus/buscontroller.h"
#include "instructions.h"

#ifndef __CPU_H__
#define __CPU_H__

class Cpu {
    private:
    RegisterController *registerController;
    BusController *busController;
    Instructions *instructions;
    uint16_t programCounter = 0;
    void instructionDecoder(uint8_t opcode);
    uint16_t getNextWord();
    uint8_t getNextByte();
    public:
    Cpu();
    void cycle();
    ~Cpu();
};

#endif // __CPU_H__