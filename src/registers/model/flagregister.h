#include "register.h"
#include <iostream>

#ifndef __FLAGREGISTER_H__
#define __FLAGREGISTER_H__

class FlagRegister : public Register {
private:
  void determineCarry(uint16_t value);
  void determineParity(uint8_t value);
  void determineAuxiliaryCarry(uint8_t value1, uint8_t value2);
  void determineZero(uint8_t value);
  void determineSigned(uint8_t value);

public:

  enum class FlagRule { All, Partial, CarryOnly };

  enum Flag { Carry = 0, AuxiliaryCarry = 4, Parity = 2, Zero = 6, Signed = 7 };
  
  FlagRegister();
  void processFlags(FlagRule flagRule, uint8_t value1, uint16_t value2,
                    std::string operation);
  bool getFlag(Flag flag);
  void setFlag(uint8_t position, bool bit);
  void setFlag(Flag flag, bool bit);
};

#endif // __FLAGREGISTER_H__