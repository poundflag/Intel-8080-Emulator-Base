#include "../registers/registercontroller.h"
#include <iostream>

#ifndef __ALU_H__
#define __ALU_H__

class ALU {
private:
  RegisterController &registerController;

  // Base Methods stolen from:
  // https://github.com/GunshipPenguin/lib8080/blob/master/src/i8080.c Line: 280
public:
  ALU(RegisterController &registerController);
  uint8_t performSub(uint8_t value1, uint8_t value2, uint8_t carryBit);
  uint8_t performAdd(uint8_t value1, uint8_t value2, uint8_t carryBit);
  uint8_t performAnd(uint8_t value1, uint8_t value2);
  uint8_t performOr(uint8_t value1, uint8_t value2);
};

#endif // __ALU_H__