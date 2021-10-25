#include "flagregister.h"

void FlagRegister::determineCarry(uint16_t value) {
  setFlag(Flag::Carry, (value & 0b100000000) == 0b100000000);
}

// Checks the number of ones in binary, and determines whether they are even
// or not
void FlagRegister::determineParity(uint8_t value) {
  /*value = (value >> 4) ^ (value & 0xF);
  value = (value >> 2) ^ (value & 0b11);
  value = (value >> 1) ^ (value & 0b1);
  setFlag(Flag::Parity, value == 0);*/
  setFlag(Flag::Parity, parity_table[value]);
}

void FlagRegister::determineAuxiliaryCarry(uint8_t value1, uint8_t value2,
                                           std::string operation) {
  uint8_t temp = 0;
  // c->hf = !((result & 0xF) == 0xF);
  if (operation == "+") {
    temp = (value1 & 0x0F) + (value2 & 0x0F);
    setFlag(Flag::AuxiliaryCarry, temp > 0x0F);
  } else {
    /*temp = (value1 & 0x0F) - (value2 & 0x0F);
    setFlag(Flag::AuxiliaryCarry, temp != 0x00);*/
    value2 = ~value2; // Pretty clever algortihm!
    uint16_t result = value1 + value2 + (1 - getFlag(FlagRegister::Carry));
    uint16_t newCarry = result ^ value1 ^ value2;
    setFlag(Flag::AuxiliaryCarry, (newCarry & (1 << 4)) != 0);
  }
  // setFlag(Flag::AuxiliaryCarry, temp > 0x0F);
}

void FlagRegister::determineZero(uint8_t value) {
  setFlag(Flag::Zero, value == 0);
}

void FlagRegister::determineSigned(uint8_t value) {
  setFlag(Flag::Signed, (value & 0b10000000) == 0b10000000);
}

void FlagRegister::determineCarry16Bit(uint32_t value) {
  setFlag(Flag::Carry, (value & 0x10000) == 0x10000);
}

FlagRegister::FlagRegister() { value = 0b00000010; }

void FlagRegister::processFlags(FlagRule flagRule, uint16_t value1,
                                uint16_t value2, std::string operation) {
  uint16_t result = 0;
  if (operation == "+") {
    result = value1 + value2;
  } else {
    // value2 = (~value2) & 0xFF;
    result = value1 - value2;
  }
  switch (flagRule) { // TODO Finish the case statements
  case FlagRule::DAD: // TODO Remove
    determineCarry16Bit(value1 + value2);
    break;
  case FlagRule::CarryOnly:
    determineCarry(result);
    break;
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
  value |= (1 << 1); // TODO Change later please
  value &= ~(1 << 3);
  value &= ~(1 << 5);
  return value;
}