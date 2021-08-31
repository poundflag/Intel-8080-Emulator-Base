#include "flagregister.h"

void FlagRegister::determineCarry(uint16_t value) {
  // TODO Look into DAD before implementing it fully
  setFlag(Flag::Carry, (value & 0b100000000) == 0b100000000);
}

void FlagRegister::determineParity(uint8_t value) {
  // Checks the number of ones in binary, and determines whether they are even
  // or not
  value = (value >> 4) ^ (value & 0xF);
  value = (value >> 2) ^ (value & 0b11);
  value = (value >> 1) ^ (value & 0b1);
  setFlag(Flag::Parity, value == 0);
}

void FlagRegister::determineAuxiliaryCarry(uint8_t value1, uint8_t value2) {
  // TODO Get operator
  uint8_t temp = (value1 & 0x0F) + (value2 & 0x0F);
  setFlag(Flag::AuxiliaryCarry, temp > 0x0F);

  /*
      let a = value_1 & 0x0F; // lower nibble
      let b = value_2 & 0x0F; // lower nibble
      let c = if operator == "+" { a + b } else { a.wrapping_sub(b) };
      // (a & 0x0f) + (n & 0x0f) > 0x0f
      self.auxiliary_carry_bit = c > 0x0f; // there must be borrow if minuend is
     less than subtrahend
  */
}

void FlagRegister::determineZero(uint8_t value) {
  setFlag(Flag::Zero, value == 0);
}

void FlagRegister::determineSigned(uint8_t value) {
  setFlag(Flag::Signed, (value & 0b10000000) == 0b10000000);
}

FlagRegister::FlagRegister() { value = 0b00000010; }

void FlagRegister::processFlags(FlagRule flagRule, uint8_t value1,
                                uint16_t value2, std::string operation) {
  uint16_t result = 0;
  if (operation == "+") {
    result = value1 + value2;
  } else {
    result = value1 - value2;
  }
  switch (flagRule) {
  case FlagRule::CarryOnly:
    determineCarry(result);
    break;
  case FlagRule::All:
    determineCarry(result);
    determineZero(result);
    determineSigned(result);
    determineParity(result);


  default:
    break;
  }
}

bool FlagRegister::getFlag(Flag flag) { return ((value >> flag) & 1) == 1; }

void FlagRegister::setFlag(uint8_t position, bool bit) {
  uint8_t flagValue = bit ? 1 : 0;
  value = value | (bit << position);
}

void FlagRegister::setFlag(Flag flag, bool bit) {
  uint8_t flagValue = bit ? 1 : 0;
  value = value | (bit << flag);
}