#include "alu.h"

ALU::ALU(RegisterController &registerController)
    : registerController(registerController) {}

uint8_t ALU::performSub(uint8_t value1, uint8_t value2, uint8_t carryBit) {
  uint8_t value2Sub = (~value2) & 0xFF;

  // I don't know why this works, but it works! TODO find out why it works
  uint16_t result = value1 + value2Sub + (carryBit ? 0 : 1);
  uint8_t result8 = result & 0xFF;
  registerController.getFlagRegister().processFlags(
      FlagRegister::FlagRule::Partial, result8, 0, "+");
  registerController.getFlagRegister().setFlag(
      FlagRegister::AuxiliaryCarry,
      ((value1 & 0xF) + (value2Sub & 0xF) + (carryBit ? 0 : 1)) & 0x10);
  registerController.getFlagRegister().setFlag(FlagRegister::Carry,
                                               !(result & 0x100));
  return result8;
}

uint8_t ALU::performAdd(uint8_t value1, uint8_t value2, uint8_t carryBit) {

  uint16_t result = value1 + value2 + (carryBit ? 1 : 0);
  uint8_t result8 = result & 0xFF;
  registerController.getFlagRegister().processFlags(
      FlagRegister::FlagRule::Partial, result8, 0, "+");
  registerController.getFlagRegister().setFlag(
      FlagRegister::AuxiliaryCarry,
      ((value1 & 0xF) + (value2 & 0xF) + (carryBit ? 1 : 0)) & 0x10);
  registerController.getFlagRegister().setFlag(FlagRegister::Carry,
                                               result & 0x100);
  return result8;
}
