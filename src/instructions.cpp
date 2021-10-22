#include "instructions.h"
#include "registers/model/flagregister.h"

Instructions::Instructions(BusController &busController,
                           RegisterController &registerController,
                           IOController &ioController)
    : busController(busController), registerController(registerController),
      ioController(ioController) {}

// MOV D,S   01DDDSSS - Move register to register
void Instructions::MOV(Registers::Register destination,
                       Registers::Register source) {
  registerController.get(destination)
      .setRegister(registerController.get(source).getRegister());
}

// MVI D,#   00DDD110 db - Move immediate to register
void Instructions::MVI(Registers::Register destination, uint8_t immediate) {
  registerController.get(destination).setRegister(immediate);
}

// LXI RP,#  00RP0001 lb hb - Load register pair immediate
void Instructions::LXI(RegisterPair registerPair, uint16_t immediate) {
  registerController.setRegisterPair(registerPair, immediate);
}

// LDA a     00111010 lb hb - Load A from memory
void Instructions::LDA(uint16_t address) {
  uint8_t memoryValue = busController.readByte(address);
  registerController.get(Registers::A).setRegister(memoryValue);
}

// STA a     00110010 lb hb - Store A to memory
void Instructions::STA(uint16_t address) {
  busController.writeByte(address,
                          registerController.get(Registers::A).getRegister());
}

// LHLD a    00101010 lb hb    -       Load H:L from memory
void Instructions::LHLD(uint16_t address) {
  registerController.setRegisterPair(RegisterPair::H,
                                     busController.readWord(address));
}

// SHLD a    00100010 lb hb    -       Store H:L to memory
void Instructions::SHLD(uint16_t address) {
  busController.writeWord(address,
                          registerController.getRegisterPair(RegisterPair::H));
}

// LDAX RP   00RP1010 *1       -       Load indirect through BC or DE
void Instructions::LDAX(RegisterPair indirectAddress) {
  uint16_t address = registerController.getRegisterPair(indirectAddress);
  registerController.get(Registers::A)
      .setRegister(busController.readByte(address));
}

// STAX RP   00RP0010 *1       -       Store indirect through BC or DE
void Instructions::STAX(RegisterPair indirectAddress) {
  uint16_t address = registerController.getRegisterPair(indirectAddress);
  busController.writeByte(address,
                          registerController.get(Registers::A).getRegister());
}

// XCHG      11101011          -       Exchange DE and HL content
void Instructions::XCHG() {
  uint16_t temp = registerController.getRegisterPair(RegisterPair::D);
  registerController.setRegisterPair(
      RegisterPair::D, registerController.getRegisterPair(RegisterPair::H));
  registerController.setRegisterPair(RegisterPair::H, temp);
}

// ADD S     10000SSS          ZSPCA   Add register to A
void Instructions::ADD(Registers::Register source) {
  uint8_t aValue = registerController.get(Registers::A).getRegister();
  uint8_t sourceValue = registerController.get(source).getRegister();

  // Add it in a 16 bit type to avoid an overflow, and therefore the wrong value
  uint16_t result = aValue + sourceValue;
  registerController.get(Registers::A).setRegister((uint8_t)result);
  registerController.getFlagRegister().processFlags(FlagRegister::FlagRule::All,
                                                    aValue, sourceValue, "+");
}

// ADI #     11000110 db       ZSCPA   Add immediate to A
void Instructions::ADI(uint8_t immediate) {
  uint8_t aValue = registerController.get(Registers::A).getRegister();

  // Add it in a 16 bit type to avoid an overflow, and therefore the wrong value
  uint16_t result = aValue + immediate;
  registerController.get(Registers::A).setRegister((uint8_t)result);
  registerController.getFlagRegister().processFlags(FlagRegister::FlagRule::All,
                                                    aValue, immediate, "+");
}

// ADC S     10001SSS          ZSCPA   Add register to A with carry
void Instructions::ADC(Registers::Register source) {
  uint8_t aValue = registerController.get(Registers::A).getRegister();
  uint8_t sourceValue = registerController.get(source).getRegister();

  // Add it in a 16 bit type to avoid an overflow, and therefore the wrong value
  uint16_t result = aValue + sourceValue;

  // If the carry flag is on
  if (registerController.getFlagRegister().getFlag(FlagRegister::Flag::Carry)) {
    sourceValue += 1;
    result += 1;
  }

  registerController.get(Registers::A).setRegister((uint8_t)result);
  registerController.getFlagRegister().processFlags(FlagRegister::FlagRule::All,
                                                    aValue, sourceValue, "+");
}

// ACI #     11001110 db       ZSCPA   Add immediate to A with carry
void Instructions::ACI(uint8_t immediate) {

  uint8_t aValue = registerController.get(Registers::A).getRegister();

  // Add it in a 16 bit type to avoid an overflow, and therefore the wrong value
  uint16_t result = aValue + immediate;

  // If the carry flag is on
  if (registerController.getFlagRegister().getFlag(FlagRegister::Flag::Carry)) {
    immediate += 1;
    result += 1;
  }

  registerController.get(Registers::A).setRegister((uint8_t)result);
  registerController.getFlagRegister().processFlags(FlagRegister::FlagRule::All,
                                                    aValue, immediate, "+");
}

// SUB S     10010SSS          ZSCPA   Subtract register from A
void Instructions::SUB(Registers::Register source) {
  uint8_t aValue = registerController.get(Registers::A).getRegister();
  uint8_t sourceValue = registerController.get(source).getRegister();

  uint8_t result = aValue - sourceValue;
  registerController.get(Registers::A).setRegister(result);
  registerController.getFlagRegister().processFlags(FlagRegister::FlagRule::All,
                                                    aValue, sourceValue, "-");
}

// SUI #     11010110 db       ZSCPA   Subtract immediate from A
void Instructions::SUI(uint8_t immediate) {
  uint8_t aValue = registerController.get(Registers::A).getRegister();

  uint8_t result = aValue - immediate;
  registerController.get(Registers::A).setRegister(result);
  registerController.getFlagRegister().processFlags(FlagRegister::FlagRule::All,
                                                    aValue, immediate, "-");
}

// SBB S     10011SSS          ZSCPA   Subtract register from A with borrow
void Instructions::SBB(Registers::Register source) {
  uint8_t aValue = registerController.get(Registers::A).getRegister();
  uint8_t sourceValue = registerController.get(source).getRegister();

  // Add it in a 16 bit type to avoid an overflow, and therefore the wrong value
  uint16_t result = aValue - sourceValue;

  // If the carry flag is on
  if (registerController.getFlagRegister().getFlag(FlagRegister::Flag::Carry)) {
    sourceValue -= 1;
    result -= 1;
  }

  registerController.get(Registers::A).setRegister((uint8_t)result);
  registerController.getFlagRegister().processFlags(FlagRegister::FlagRule::All,
                                                    aValue, sourceValue, "-");
}

// SBI #     11011110 db       ZSCPA   Subtract immediate from A with borrow
void Instructions::SBI(uint8_t immediate) {
  uint8_t aValue = registerController.get(Registers::A).getRegister();

  // Add it in a 16 bit type to avoid an overflow, and therefore the wrong value
  uint16_t result = aValue - immediate;

  // If the carry flag is on
  if (registerController.getFlagRegister().getFlag(FlagRegister::Flag::Carry)) {
    immediate -= 1;
    result -= 1;
  }

  registerController.get(Registers::A).setRegister((uint8_t)result);
  registerController.getFlagRegister().processFlags(FlagRegister::FlagRule::All,
                                                    aValue, immediate, "-");
}

// INR D     00DDD100          ZSPA    Increment register
void Instructions::INR(Registers::Register destination) {
  uint8_t temp = registerController.get(destination).getRegister();
  registerController.get(destination).setRegister(temp + 1);
  registerController.getFlagRegister().processFlags(
      FlagRegister::FlagRule::Partial, temp, 1, "+");
}

// DCR D     00DDD101          ZSPA    Decrement register
void Instructions::DCR(Registers::Register destination) {
  uint8_t temp = registerController.get(destination).getRegister();
  registerController.get(destination).setRegister(temp - 1);
  registerController.getFlagRegister().processFlags(
      FlagRegister::FlagRule::Partial, temp, 1, "-");
}

// INX RP    00RP0011          -       Increment register pair
void Instructions::INX(RegisterPair destination) {
  uint16_t temp = registerController.getRegisterPair(destination);
  registerController.setRegisterPair(destination, temp + 1);
}

// DCX RP    00RP1011          -       Decrement register pair
void Instructions::DCX(RegisterPair destination) {
  uint16_t temp = registerController.getRegisterPair(destination);
  registerController.setRegisterPair(destination, temp - 1);
}

// DAD RP    00RP1001          C       Add register pair to HL (16 bit add)
void Instructions::DAD(RegisterPair source) {
  uint16_t sourceValue = registerController.getRegisterPair(source);
  uint16_t hValue = registerController.getRegisterPair(RegisterPair::H);
  registerController.setRegisterPair(RegisterPair::H, hValue + sourceValue);
  registerController.getFlagRegister().processFlags(FlagRegister::FlagRule::DAD,
                                                    sourceValue, hValue, "+");
}

// DAA       00100111          ZSPCA   Decimal Adjust accumulator
void Instructions::DAA() {
  // Stolen from
  // https://github.com/GunshipPenguin/lib8080/blob/master/src/i8080.c
  // Line: 405 | I don't really know, what this mess does
  int value = registerController.get(Registers::A).getRegister();

  uint8_t tempValue = 0;

  // If the least significant four bits of the accumulator represents a number
  // greater than 9, or if the Auxiliary Carry bit is equal to one, the
  // accumulator is incremented by six. Otherwise, no incrementing occurs.
  if (((value & 0xF) > 9) || registerController.getFlagRegister().getFlag(
                                 FlagRegister::Flag::AuxiliaryCarry)) {
    tempValue |= 0x06;
  }

  bool carry =
      registerController.getFlagRegister().getFlag(FlagRegister::Flag::Carry);
  // If the most significant four bits of the accumulator now represent a number
  // greater than 9, or if the normal carry bit is equal to one, the most
  // significant four bits of the accumulator are incremented by six.
  if (((value & 0xF0) > 0x90) ||
      (((value & 0xF0) >= 0x90) && ((value & 0xF) > 9)) ||
      registerController.getFlagRegister().getFlag(FlagRegister::Flag::Carry)) {
    tempValue |= 0x60;
    carry = true;
  }


  registerController.getFlagRegister().processFlags(FlagRegister::FlagRule::All,
                                                    value, tempValue, "+");

  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry,
                                               carry);

  registerController.get(Registers::A).setRegister(value + tempValue);
}

// ANA S     10100SSS          ZSCPA   AND register with A
void Instructions::ANA(Registers::Register source) {
  uint8_t sourceValue = registerController.get(source).getRegister();
  uint8_t aValue = registerController.get(Registers::A).getRegister();
  registerController.get(Registers::A).setRegister(aValue & sourceValue);
  registerController.getFlagRegister().processFlags(
      FlagRegister::FlagRule::All, aValue & sourceValue, 0, "+");
}

// ANI #     11100110 db       ZSPCA   AND immediate with A
void Instructions::ANI(uint8_t immediate) {
  uint8_t aValue = registerController.get(Registers::A).getRegister();
  registerController.get(Registers::A).setRegister(aValue & immediate);
  registerController.getFlagRegister().processFlags(FlagRegister::FlagRule::All,
                                                    aValue & immediate, 0, "+");
}

// ORA S     10110SSS          ZSPCA   OR  register with A
void Instructions::ORA(Registers::Register source) {
  uint8_t sourceValue = registerController.get(source).getRegister();
  uint8_t aValue = registerController.get(Registers::A).getRegister();
  registerController.get(Registers::A).setRegister(aValue | sourceValue);
  registerController.getFlagRegister().processFlags(
      FlagRegister::FlagRule::All, aValue | sourceValue, 0, "+");
}

// ORI #     11110110          ZSPCA   OR  immediate with A
void Instructions::ORI(uint8_t immediate) {
  uint8_t aValue = registerController.get(Registers::A).getRegister();
  registerController.get(Registers::A).setRegister(aValue | immediate);
  registerController.getFlagRegister().processFlags(FlagRegister::FlagRule::All,
                                                    aValue | immediate, 0, "+");
}

// XRA S     10101SSS          ZSPCA   ExclusiveOR register with A
void Instructions::XRA(Registers::Register source) {
  uint8_t sourceValue = registerController.get(source).getRegister();
  uint8_t aValue = registerController.get(Registers::A).getRegister();
  registerController.get(Registers::A).setRegister(aValue ^ sourceValue);
  registerController.getFlagRegister().processFlags(
      FlagRegister::FlagRule::All, aValue ^ sourceValue, 0, "+");
}

// XRI #     11101110 db       ZSPCA   ExclusiveOR immediate with A
void Instructions::XRI(uint8_t immediate) {
  uint8_t aValue = registerController.get(Registers::A).getRegister();
  registerController.get(Registers::A).setRegister(aValue ^ immediate);
  registerController.getFlagRegister().processFlags(FlagRegister::FlagRule::All,
                                                    aValue ^ immediate, 0, "+");
}

// CMP S     10111SSS          ZSPCA   Compare register with A
void Instructions::CMP(Registers::Register source) {
  uint8_t intermediate = registerController.get(Registers::A).getRegister();
  SUB(source); // TODO FIX
  registerController.get(Registers::A).setRegister(intermediate);
}

// CPI #     11111110          ZSPCA   Compare immediate with A
void Instructions::CPI(uint8_t immediate) {
  uint8_t intermediate = registerController.get(Registers::A).getRegister();
  SUI(immediate);
  registerController.get(Registers::A).setRegister(intermediate);
}

// RLC       00000111          C       Rotate A left
void Instructions::RLC() {
  uint16_t value = registerController.get(Registers::A).getRegister();
  value = value << 1 | ((value & 0x80) >> 7);
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry,
                                               (value & 0x100) == 0x100);

  registerController.get(Registers::A).setRegister(value);
}

// RRC       00001111          C       Rotate A right
void Instructions::RRC() {

  uint16_t value = registerController.get(Registers::A).getRegister();

  // Get the first bit in the value, and check if it is == 1
  bool carry = (value & 0x01) == 0x01;

  // If that's the case, then shift the value, and add the carry to the last bit
  if (carry) {
    value = 0x80 | value >> 1;
  } else {
    value = value >> 1;
  }
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry,
                                               carry);
  registerController.get(Registers::A).setRegister(value);
}

// RAL       00010111          C       Rotate A left through carry
void Instructions::RAL() {

  uint16_t value = registerController.get(Registers::A).getRegister();

  // Get the last value and check is it is == 1
  bool carry = (value & 0x80) == 0x80;

  // If the carry in the flag register is on
  // take the value and add one in the first position
  if (registerController.getFlagRegister().getFlag(FlagRegister::Flag::Carry)) {
    value = value << 1 | 1;
  } else {
    value = value << 1 | 0;
  }
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry,
                                               carry);
  registerController.get(Registers::A).setRegister(value);
}

// RAR       00011111          C       Rotate A right through carry
void Instructions::RAR() {

  uint16_t value = registerController.get(Registers::A).getRegister();

  // Get the first value and check is it is == 1
  bool carry = (value & 0x1) == 0x1;

  // If the carry in the flag register is on
  // take the value and add one in the last position
  if (registerController.getFlagRegister().getFlag(FlagRegister::Flag::Carry)) {
    value = 0x80 | value >> 1;
  } else {
    value = value >> 1;
  }
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry,
                                               carry);
  registerController.get(Registers::A).setRegister(value);
}

// CMA       00101111          -       Compliment A
void Instructions::CMA() {
  uint8_t value = registerController.get(Registers::A).getRegister();
  registerController.get(Registers::A).setRegister(~value);
}

// CMC       00111111          C       Compliment Carry flag
void Instructions::CMC() {
  bool carry =
      registerController.getFlagRegister().getFlag(FlagRegister::Flag::Carry);
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry,
                                               !carry);
}

// STC       00110111          C       Set Carry flag
void Instructions::STC() {
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, true);
}

// JMP a     11000011 lb hb    -       Unconditional jump
void Instructions::JMP(uint16_t &source, uint16_t address) { source = address; }

// Checks the condition for all the condi. jmp, ret and call instr.
bool Instructions::conditionSuccessful(FlagRegister::Condition condition) {
  FlagRegister flagRegister = registerController.getFlagRegister();
  switch (condition) {
  case FlagRegister::Condition::Carry:
    return flagRegister.getFlag(FlagRegister::Flag::Carry) == 1;
    break;
  case FlagRegister::Condition::NotCarry:
    return flagRegister.getFlag(FlagRegister::Flag::Carry) == 0;
    break;
  case FlagRegister::Condition::Minus:
    return flagRegister.getFlag(FlagRegister::Flag::Signed) == 1;
    break;
  case FlagRegister::Condition::Positive:
    return flagRegister.getFlag(FlagRegister::Flag::Signed) == 0;
    break;
  case FlagRegister::Condition::ParityEven:
    return flagRegister.getFlag(FlagRegister::Flag::Parity) == 1;
    break;
  case FlagRegister::Condition::ParityOdd:
    return flagRegister.getFlag(FlagRegister::Flag::Parity) == 0;
    break;
  case FlagRegister::Condition::Zero:
    return flagRegister.getFlag(FlagRegister::Flag::Zero) == 1;
    break;
  case FlagRegister::Condition::NotZero:
    return flagRegister.getFlag(FlagRegister::Flag::Zero) == 0;
    break;
  }
}

// Jccc a    11CCC010 lb hb    -       Conditional jump
bool Instructions::JMPCondition(uint16_t &source, uint16_t address,
                                FlagRegister::Condition condition) {
  bool success = conditionSuccessful(condition);
  if (success) {
    JMP(source, address);
  }
  return success;
}

// CALL a    11001101 lb hb    -       Unconditional subroutine call
void Instructions::CALL(uint16_t &source, uint16_t address) {
  registerController.getStack().pushWord(source + 1);
  source = address;
}

// Cccc a    11CCC100 lb hb    -       Conditional subroutine call
bool Instructions::CALLCondition(uint16_t &source, uint16_t address,
                                 FlagRegister::Condition condition) {
  bool success = conditionSuccessful(condition);
  if (success) {
    CALL(source, address);
  }
  return success;
}

// RET       11001001          -       Unconditional return from subroutine
void Instructions::RET(uint16_t &source) {
  source = registerController.getStack().popWord();
}

// Rccc      11CCC000          -       Conditional return from subroutine
bool Instructions::RETCondition(uint16_t &source,
                                FlagRegister::Condition condition) {
  bool success = conditionSuccessful(condition);
  if (success) {
    RET(source);
  }
  return success;
}

// RST n     11NNN111          -       Restart (Call n*8)
void Instructions::RST(uint16_t &source, uint8_t n) {
  registerController.getStack().pushWord(source + 1);
  source = (8 * n) - 1;
}

// PCHL      11101001          -       Jump to address in H:L
void Instructions::PCHL(uint16_t &source) {
  source = registerController.getRegisterPair(RegisterPair::H);
}

// PUSH RP   11RP0101 *2       -       Push register pair on the stack
void Instructions::PUSH(RegisterPair registerPair) {
  registerController.getStack().pushWord(
      registerController.getRegisterPair(registerPair));
}

// POP RP    11RP0001 *2       *2      Pop  register pair from the stack
void Instructions::POP(RegisterPair registerPair) {
  registerController.setRegisterPair(registerPair,
                                     registerController.getStack().popWord());
}

// XTHL      11100011          -       Swap H:L with top word on stack
void Instructions::XTHL() {
  uint16_t stackTemp = registerController.getStack().popWord();
  registerController.getStack().pushWord(
      registerController.getRegisterPair(RegisterPair::H));
  registerController.setRegisterPair(RegisterPair::H, stackTemp);
}

// SPHL      11111001          -       Set SP to content of H:L
void Instructions::SPHL() {
  registerController.getStack().setStackPointer(
      registerController.getRegisterPair(RegisterPair::H));
}

// IN p      11011011 pa       -       Read input port into A
void Instructions::IN(int portNumber) {
  registerController.get(Registers::A)
      .setRegister(ioController.getDeviceValue(portNumber));
}

// OUT p     11010011 pa       -       Write A to output port
void Instructions::OUT(int portNumber) {
  ioController.setDeviceValue(
      portNumber, registerController.get(Registers::A).getRegister());
}

// HLT       01110110          -       Halt processor
bool Instructions::HLT() { return true; }
