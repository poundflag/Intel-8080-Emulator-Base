#include "register.h"
#include <iostream>

#ifndef __FLAGREGISTER_H__
#define __FLAGREGISTER_H__

class FlagRegister : public Register {
private:
  int parity_table[256] = {
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1,
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1,
      1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1,
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
      0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1,
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
      1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1};

  void determineCarry(uint16_t value);
  void determineParity(uint8_t value);
  void determineAuxiliaryCarry(uint8_t value1, uint8_t value2,
                               std::string operation);
  void determineZero(uint8_t value);
  void determineSigned(uint8_t value);

public:
  enum class FlagRule { All, Partial, CarryOnly };

  enum class Condition { // Move to designated class TODO
    Carry,
    NotCarry,
    Zero,
    NotZero,
    ParityOdd,
    ParityEven,
    Minus,
    Positive
  };

  enum Flag { Carry = 0, AuxiliaryCarry = 4, Parity = 2, Zero = 6, Signed = 7 };

  FlagRegister();
  void processFlags(FlagRule flagRule, uint16_t value1, uint16_t value2,
                    std::string operation);
  bool getFlag(Flag flag);
  void setFlag(Flag flag, bool bit);
  uint8_t getRegister() override;
};

#endif // __FLAGREGISTER_H__