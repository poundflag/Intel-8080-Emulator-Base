#include "instructions.h"

#ifndef __INSTRUCTIONDECODER_H__
#define __INSTRUCTIONDECODER_H__

class InstructionDecoder {
private:
  BusController &busController;
  Instructions &instructions;
  RegisterController &registerController;
  uint16_t getNextWord();
  uint8_t getNextByte();

public:
  InstructionDecoder(Instructions &instructions, BusController &busController,
                     RegisterController &registerController);
  bool instructionDecoder(uint8_t opcode);
};

#endif // __INSTRUCTIONDECODER_H__