#include "bus/buscontroller.h"
#include "instructions.h"
#include "registers/registercontroller.h"
#include "io/iocontroller.h"

#ifndef __CPU_H__
#define __CPU_H__

// The core class of the i8080
class Cpu {
private:
  RegisterController *registerController;
  BusController *busController;
  Instructions *instructions;
  IOController *ioController;
  uint16_t programCounter = 0;
  bool instructionDecoder(uint8_t opcode);
  uint16_t getNextWord();
  uint8_t getNextByte();

public:
  Cpu();
  void run();
  void step(int steps);
  bool cycle();
  BusController &getBusController();
  RegisterController &getRegisterController();
  IOController &getIOController();
  uint16_t getProgramCounter();
  void setProgramCounter(uint16_t programCounter);
  ~Cpu();
};

#endif // __CPU_H__