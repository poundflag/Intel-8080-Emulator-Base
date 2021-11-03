#include "alu.h"

ALU::ALU(RegisterController &registerController)
    : registerController(registerController) {}

uint8_t ALU::performSub(uint8_t value1, uint8_t value2, uint8_t carryBit) {
  uint8_t value2Sub = (~value2) & 0xFF;

  // Add all resulting values, and subtract the value if the carry is set
  uint16_t result = value1 + value2Sub + (carryBit ? 0 : 1);
  uint8_t result8 = result & 0xFF;
  // Process only the ZSPA and the remaining later
  registerController.getFlagRegister().processFlags(
      FlagRegister::FlagRule::Partial, result8, 0, "+");
  bool acResult =
      ((value1 & 0xF) + (value2Sub & 0xF) + (carryBit ? 0 : 1)) & 0x10;
  registerController.getFlagRegister().setFlag(FlagRegister::AuxiliaryCarry,
                                               (acResult));
  // Determine the complement of the carry
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

uint8_t ALU::performAnd(uint8_t value1, uint8_t value2) {

  uint8_t result = value1 & value2;

  //  Or all bits down to a single result and check if it ands with 0x08
  // If this is the case the and operation will result in a half-carry bit
  bool acResult = ((value1 | value2) & 0x08) != 0;

  // registerController.get(Registers::A).setRegister(result);
  registerController.getFlagRegister().processFlags(FlagRegister::FlagRule::All,
                                                    result, 0, "+");

  // Any AND-operation won't result in a carry,
  // therefore we can just set it to 0
  registerController.getFlagRegister().setFlag(FlagRegister::Carry, false);
  registerController.getFlagRegister().setFlag(FlagRegister::AuxiliaryCarry,
                                               acResult);

  return result;
}

uint8_t ALU::performOr(uint8_t value1, uint8_t value2) {
  uint8_t result = value1 | value2;

  registerController.getFlagRegister().processFlags(FlagRegister::FlagRule::All,
                                                    result, 0, "+");
  return result;
}
