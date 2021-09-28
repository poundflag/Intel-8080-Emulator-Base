#include "bus/buscontroller.h"
#include "instructions.h"
#include "registers/registercontroller.h"

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
  void step();
  void step(int steps);
  void cycle();
  BusController &getBusController();
  ~Cpu();
};

#endif // __CPU_H__