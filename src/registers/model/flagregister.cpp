#include "flagregister.h"

FlagRegister::FlagRegister() { value = 0b00000010; }

void FlagRegister::determineCarry(uint16_t value) {
  setFlag(Flag::Carry, (value & 0x100) == 0x100);
}

// Checks the number of ones in binary, and determines whether they are even
// or not
void FlagRegister::determineParity(uint8_t value) {
  setFlag(Flag::Parity, parity_table[value]);
}

void FlagRegister::determineAuxiliaryCarry(uint8_t value1, uint8_t value2,
                                           std::string operation) {
  uint8_t temp = 0;
  if (operation == "+") {
    temp = (value1 & 0x0F) + (value2 & 0x0F);
    setFlag(Flag::AuxiliaryCarry, temp > 0x0F);
  } else {
    // Pretty clever algortihm!
    value2 = ~value2;
    uint16_t result = value1 + value2 + (1 - getFlag(FlagRegister::Carry));
    uint16_t newCarry = result ^ value1 ^ value2;
    setFlag(Flag::AuxiliaryCarry, (newCarry & (1 << 4)) != 0);
  }
}

void FlagRegister::determineZero(uint8_t value) {
  setFlag(Flag::Zero, value == 0);
}

void FlagRegister::determineSigned(uint8_t value) {
  setFlag(Flag::Signed, (value & 0x80) == 0x80);
}

void FlagRegister::processFlags(FlagRule flagRule, uint16_t value1,
                                uint16_t value2, std::string operation) {
  uint16_t result = 0;
  if (operation == "+") {
    result = value1 + value2;
  } else {
    result = value1 - value2;
  }

  switch (flagRule) {
  case FlagRule::All:
    determineCarry(result);
    determineZero(result);
    determineSigned(result);
    determineParity(result);
    determineAuxiliaryCarry(value1, value2, operation);
    break;
  case FlagRule::Partial:
    determineZero(result);
    determineSigned(result);
    determineParity(result);
    determineAuxiliaryCarry(value1, value2, operation);
    break;
  case FlagRule::CarryOnly:
    determineCarry(result);
    break;

  default:
    break;
  }
}

bool FlagRegister::getFlag(Flag flag) { return ((value >> flag) & 1) == 1; }

void FlagRegister::setFlag(Flag flag, bool bit) {
  if (bit) {
    value |= (1 << flag);
  } else {
    value &= ~(1 << flag);
  }
}

uint8_t FlagRegister::getRegister() {
  // All positions, which should be zero are set to zero
  value |= (1 << 1);
  value &= ~(1 << 3);
  value &= ~(1 << 5);
  return value;
}