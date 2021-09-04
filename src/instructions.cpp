#include "instructions.h"

void Instructions::MOV(Register &destination, Register &source) {
  // MOV D,S   01DDDSSS - Move register to register
  destination.setRegister(source.getRegister());
}

void Instructions::MVI(Register destination, uint8_t immediate) {
  // MVI D,#   00DDD110 db - Move immediate to register
  destination.setRegister(immediate);
}
