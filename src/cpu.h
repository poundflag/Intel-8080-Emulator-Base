#include "bus/buscontroller.h"
#include "core/alu.h"
#include "instruction/instructiondecoder.h"
#include "io/iocontroller.h"
#include "registers/registercontroller.h"

#ifndef __CPU_H__
#define __CPU_H__

// The core class of the i8080
class Cpu {
private:
  BusController busController = BusController();
  IOController ioController = IOController();
  RegisterController *registerController;
  Instructions *instructions;
  InstructionDecoder *instructionDecoder;
  ALU *alu;

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