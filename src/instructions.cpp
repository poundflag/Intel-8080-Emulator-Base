#include "instructions.h"
#include "registers/model/flagregister.h"

Instructions::Instructions(
    std::shared_ptr<BusController> busController,
    std::shared_ptr<RegisterController> registerController)
    : busController(move(busController)),
      registerController(move(registerController)) {}

void Instructions::MOV(Register &destination, Register &source) {
  // MOV D,S   01DDDSSS - Move register to register
  destination.setRegister(source.getRegister());
}

void Instructions::MVI(Register &destination, uint8_t immediate) {
  // MVI D,#   00DDD110 db - Move immediate to register
  destination.setRegister(immediate);
}

void Instructions::LXI(RegisterPair registerPair, uint16_t immediate) {
  // LXI RP,#  00RP0001 lb hb - Load register pair immediate
}

void Instructions::LDA(uint16_t address) {
  // LDA a     00111010 lb hb - Load A from memory
  uint8_t memoryValue = busController->readByte(address);
  registerController->get(Registers::A).setRegister(memoryValue);
}

void Instructions::STA(uint16_t address) {
  // STA a     00110010 lb hb - Store A to memory
  busController->writeByte(address,
                           registerController->get(Registers::A).getRegister());
}

void Instructions::LHLD(uint16_t address) {
  // LHLD a    00101010 lb hb    -       Load H:L from memory
  registerController->setRegisterPair(RegisterPair::H,
                                      busController->readWord(address));
}

void Instructions::SHLD(uint16_t address) {
  // SHLD a    00100010 lb hb    -       Store H:L to memory
  busController->writeWord(
      address, registerController->getRegisterPair(RegisterPair::H));
}

void Instructions::LDAX(RegisterPair indirectAddress) {
  // LDAX RP   00RP1010 *1       -       Load indirect through BC or DE
  uint16_t address =
      registerController->getRegisterPair(indirectAddress); // Tidy up
  registerController->get(Registers::A)
      .setRegister(busController->readByte(address));
}

void Instructions::STAX(RegisterPair indirectAddress) {
  // STAX RP   00RP0010 *1       -       Store indirect through BC or DE
  uint16_t address = registerController->getRegisterPair(indirectAddress);
  busController->writeByte(address,
                           registerController->get(Registers::A).getRegister());
}

void Instructions::XCHG() {
  // XCHG      11101011          -       Exchange DE and HL content
  uint16_t temp = registerController->getRegisterPair(RegisterPair::D);
  registerController->setRegisterPair(
      RegisterPair::D, registerController->getRegisterPair(RegisterPair::H));
  registerController->setRegisterPair(RegisterPair::H, temp);
}

void Instructions::ADD(Register source) {
  // ADD S     10000SSS          ZSPCA   Add register to A
  uint8_t aValue = registerController->get(Registers::A).getRegister();
  uint8_t sourceValue = source.getRegister();

  // Add it in a 16 bit type to avoid an overflow, and therefore the wrong value
  uint16_t result = aValue + sourceValue;
  registerController->get(Registers::A).setRegister((uint8_t)result);
  registerController->getFlagRegister().processFlags(
      FlagRegister::FlagRule::All, aValue, sourceValue, "+");
}

void Instructions::ADI(uint8_t immediate) {
  // ADI #     11000110 db       ZSCPA   Add immediate to A
  uint8_t aValue = registerController->get(Registers::A).getRegister();

  // Add it in a 16 bit type to avoid an overflow, and therefore the wrong value
  uint16_t result = aValue + immediate;
  registerController->get(Registers::A).setRegister((uint8_t)result);
  registerController->getFlagRegister().processFlags(
      FlagRegister::FlagRule::All, aValue, immediate, "+");
}

void Instructions::ADC(Register source) {
  // ADC S     10001SSS          ZSCPA   Add register to A with carry
  uint8_t aValue = registerController->get(Registers::A).getRegister();
  uint8_t sourceValue = source.getRegister();

  // Add it in a 16 bit type to avoid an overflow, and therefore the wrong value
  uint16_t result = aValue + sourceValue;

  // If the carry flag is on
  if (registerController->getFlagRegister().getFlag(
          FlagRegister::Flag::Carry)) {
    sourceValue += 1; // TODO Potentially can lead to a wrong value
    result += 1;
  }

  registerController->get(Registers::A).setRegister((uint8_t)result);
  registerController->getFlagRegister().processFlags(
      FlagRegister::FlagRule::All, aValue, sourceValue, "+");
}

void Instructions::ACI(uint8_t immediate) {
  // ACI #     11001110 db       ZSCPA   Add immediate to A with carry

  uint8_t aValue = registerController->get(Registers::A).getRegister();

  // Add it in a 16 bit type to avoid an overflow, and therefore the wrong value
  uint16_t result = aValue + immediate;

  // If the carry flag is on
  if (registerController->getFlagRegister().getFlag(
          FlagRegister::Flag::Carry)) {
    immediate += 1; // TODO Potentially can lead to a wrong value
    result += 1;
  }

  registerController->get(Registers::A).setRegister((uint8_t)result);
  registerController->getFlagRegister().processFlags(
      FlagRegister::FlagRule::All, aValue, immediate, "+");
}

void Instructions::SUB(Register source) {
  // SUB S     10010SSS          ZSCPA   Subtract register from A
  uint8_t aValue = registerController->get(Registers::A).getRegister();
  uint8_t sourceValue = source.getRegister();

  uint8_t result = aValue - sourceValue;
  registerController->get(Registers::A).setRegister(result);
  registerController->getFlagRegister().processFlags(
      FlagRegister::FlagRule::All, aValue, sourceValue, "-");
}

void Instructions::SUI(uint8_t immediate) {
  // SUI #     11010110 db       ZSCPA   Subtract immediate from A
  uint8_t aValue = registerController->get(Registers::A).getRegister();

  uint8_t result = aValue - immediate;
  registerController->get(Registers::A).setRegister(result);
  registerController->getFlagRegister().processFlags(
      FlagRegister::FlagRule::All, aValue, immediate, "-");
}

void Instructions::SBB(Register source) {
  // SBB S     10011SSS          ZSCPA   Subtract register from A with borrow
  uint8_t aValue = registerController->get(Registers::A).getRegister();
  uint8_t sourceValue = source.getRegister();

  // Add it in a 16 bit type to avoid an overflow, and therefore the wrong value
  uint16_t result = aValue - sourceValue;

  // If the carry flag is on
  if (registerController->getFlagRegister().getFlag(
          FlagRegister::Flag::Carry)) {
    sourceValue -= 1; // TODO Potentially can lead to a wrong value
    result -= 1;
  }

  registerController->get(Registers::A).setRegister((uint8_t)result);
  registerController->getFlagRegister().processFlags(
      FlagRegister::FlagRule::All, aValue, sourceValue, "-");
}

void Instructions::SBI(uint8_t immediate) {
  // SBI #     11011110 db       ZSCPA   Subtract immediate from A with borrow
  uint8_t aValue = registerController->get(Registers::A).getRegister();

  // Add it in a 16 bit type to avoid an overflow, and therefore the wrong value
  uint16_t result = aValue - immediate;

  // If the carry flag is on
  if (registerController->getFlagRegister().getFlag(
          FlagRegister::Flag::Carry)) {
    immediate -= 1; // TODO Potentially can lead to a wrong value
    result -= 1;
  }

  registerController->get(Registers::A).setRegister((uint8_t)result);
  registerController->getFlagRegister().processFlags(
      FlagRegister::FlagRule::All, aValue, immediate, "-");
}

void Instructions::INR(Register &destination) {
  // INR D     00DDD100          ZSPA    Increment register
  uint8_t temp = destination.getRegister();
  destination.setRegister(temp + 1);
  registerController->getFlagRegister().processFlags(
      FlagRegister::FlagRule::Partial, temp, 1, "+");
}

void Instructions::DCR(Register &destination) {
  // DCR D     00DDD101          ZSPA    Decrement register
  uint8_t temp = destination.getRegister();
  destination.setRegister(temp - 1);
  registerController->getFlagRegister().processFlags(
      FlagRegister::FlagRule::Partial, temp, 1, "-");
}

void Instructions::INX(RegisterPair destination) {
  // INX RP    00RP0011          -       Increment register pair
  uint16_t temp = registerController->getRegisterPair(destination);
  registerController->setRegisterPair(destination, temp + 1);
}

void Instructions::DCX(RegisterPair destination) {
  // DCX RP    00RP1011          -       Decrement register pair
  uint16_t temp = registerController->getRegisterPair(destination);
  registerController->setRegisterPair(destination, temp - 1);
}

void Instructions::DAD(RegisterPair source) {
  // DAD RP    00RP1001          C       Add register pair to HL (16 bit add)
  uint16_t sourceValue = registerController->getRegisterPair(source);
  uint16_t hValue = registerController->getRegisterPair(RegisterPair::H);
  registerController->setRegisterPair(RegisterPair::H, hValue + sourceValue);
  registerController->getFlagRegister().processFlags(
      FlagRegister::FlagRule::DAD, sourceValue, hValue, "+");
}

void Instructions::DAA() {
  // DAA       00100111          ZSPCA   Decimal Adjust accumulator
  // Stolen from https://github.com/Milkdrop/Intel-8080-Emulator/blob/master/CPU.cpp Line: 434
  int value = registerController->get(Registers::A).getRegister();

  uint8_t tempValue = 0;

  // If the least significant four bits of the accumulator represents a number
  // greater than 9, or if the Auxiliary Carry bit is equal to one, the
  // accumulator is incremented by six. Otherwise, no incrementing occurs.
  if ((value & 0xF) > 9 || registerController->getFlagRegister().getFlag(
                               FlagRegister::Flag::AuxiliaryCarry)) {
    tempValue += 0x06;
  }

  // If the most significant four bits of the accumulator now represent a number
  // greater than 9, or if the normal carry bit is equal to one, the most
  // significant four bits of the accumulator are incremented by six.
  if (((value >> 4) >= 9 && (value & 0xF) > 9) || (value >> 4) > 9 ||
      registerController->getFlagRegister().getFlag(
          FlagRegister::Flag::Carry)) {
    tempValue += 0x60;
  }

  registerController->getFlagRegister().processFlags(
      FlagRegister::FlagRule::All, value, tempValue, "+");

  registerController->get(Registers::A).setRegister(value + tempValue);
}

void Instructions::ANA(Register source) {
  // ANA S     10100SSS          ZSCPA   AND register with A
  uint8_t sourceValue = source.getRegister();
  uint8_t aValue = registerController->get(Registers::A).getRegister();
  registerController->get(Registers::A).setRegister(aValue & sourceValue);
  registerController->getFlagRegister().processFlags(
      FlagRegister::FlagRule::All, aValue & sourceValue, 0, "+");
}

void Instructions::ANI(uint8_t immediate) {
  // ANI #     11100110 db       ZSPCA   AND immediate with A
  uint8_t aValue = registerController->get(Registers::A).getRegister();
  registerController->get(Registers::A).setRegister(aValue & immediate);
  registerController->getFlagRegister().processFlags(
      FlagRegister::FlagRule::All, aValue & immediate, 0, "+");
}

void Instructions::ORA(Register source) {
  // ORA S     10110SSS          ZSPCA   OR  register with A
  uint8_t sourceValue = source.getRegister();
  uint8_t aValue = registerController->get(Registers::A).getRegister();
  registerController->get(Registers::A).setRegister(aValue | sourceValue);
  registerController->getFlagRegister().processFlags(
      FlagRegister::FlagRule::All, aValue | sourceValue, 0, "+");
}

void Instructions::ORI(uint8_t immediate) {
  // ORI #     11110110          ZSPCA   OR  immediate with A
  uint8_t aValue = registerController->get(Registers::A).getRegister();
  registerController->get(Registers::A).setRegister(aValue | immediate);
  registerController->getFlagRegister().processFlags(
      FlagRegister::FlagRule::All, aValue | immediate, 0, "+");
}

void Instructions::XRA(Register source) {
  // XRA S     10101SSS          ZSPCA   ExclusiveOR register with A
  uint8_t sourceValue = source.getRegister();
  uint8_t aValue = registerController->get(Registers::A).getRegister();
  registerController->get(Registers::A).setRegister(aValue ^ sourceValue);
  registerController->getFlagRegister().processFlags(
      FlagRegister::FlagRule::All, aValue ^ sourceValue, 0, "+");
}

void Instructions::XRI(uint8_t immediate) {
  // XRI #     11101110 db       ZSPCA   ExclusiveOR immediate with A
  uint8_t aValue = registerController->get(Registers::A).getRegister();
  registerController->get(Registers::A).setRegister(aValue ^ immediate);
  registerController->getFlagRegister().processFlags(
      FlagRegister::FlagRule::All, aValue ^ immediate, 0, "+");
}

void Instructions::CMP(Register source) {
  // CMP S     10111SSS          ZSPCA   Compare register with A
  uint8_t intermediate = registerController->get(Registers::A).getRegister();
  SUB(source);
  registerController->get(Registers::A).setRegister(intermediate);
}

void Instructions::CPI(uint8_t immediate) {
  // CPI #     11111110          ZSPCA   Compare immediate with A
  uint8_t intermediate = registerController->get(Registers::A).getRegister();
  SUI(immediate);
  registerController->get(Registers::A).setRegister(intermediate);
}

void Instructions::RLC() {
  // RLC       00000111          C       Rotate A left
  uint16_t value = registerController->get(Registers::A).getRegister();
  value = value << 1 | ((value & 0x80) >> 7);
  registerController->getFlagRegister().setFlag(FlagRegister::Flag::Carry,
                                                (value & 0x100) == 0x100);

  registerController->get(Registers::A).setRegister(value);
}

void Instructions::RRC() {
  // RRC       00001111          C       Rotate A right

  uint16_t value = registerController->get(Registers::A).getRegister();
  bool carry = (value & 0x01) == 0x01;
  if (carry) {
    value = 0x80 | value >> 1;
  } else {
    value = value >> 1;
  }
  registerController->getFlagRegister().setFlag(FlagRegister::Flag::Carry,
                                                carry);
  registerController->get(Registers::A).setRegister(value);
}

void Instructions::RAL() {
  // RAL       00010111          C       Rotate A left through carry

  uint16_t value = registerController->get(Registers::A).getRegister();
  bool carry = (value & 0x80) == 0x80;
  if (registerController->getFlagRegister().getFlag(
          FlagRegister::Flag::Carry)) {
    value = value << 1 | 1;
  } else {
    value = value << 1 | 0;
  }
  registerController->getFlagRegister().setFlag(FlagRegister::Flag::Carry,
                                                carry);
  registerController->get(Registers::A).setRegister(value);
}

void Instructions::RAR() {
  // RAR       00011111          C       Rotate A right through carry

  uint16_t value = registerController->get(Registers::A).getRegister();
  bool carry = (value & 0x1) == 0x1;
  if (registerController->getFlagRegister().getFlag(
          FlagRegister::Flag::Carry)) {
    value = 0x80 | value >> 1;
  } else {
    value = value >> 1;
  }
  registerController->getFlagRegister().setFlag(FlagRegister::Flag::Carry,
                                                carry);
  registerController->get(Registers::A).setRegister(value);
}

void Instructions::CMA() {
  // CMA       00101111          -       Compliment A
  uint8_t value = registerController->get(Registers::A).getRegister();
  registerController->get(Registers::A).setRegister(~value);
}

void Instructions::CMC() {
  // CMC       00111111          C       Compliment Carry flag
  bool carry =
      registerController->getFlagRegister().getFlag(FlagRegister::Flag::Carry);
  registerController->getFlagRegister().setFlag(FlagRegister::Flag::Carry,
                                                false);
}

void Instructions::STC() {
  // STC       00110111          C       Set Carry flag
  registerController->getFlagRegister().setFlag(FlagRegister::Flag::Carry,
                                                true);
}

void Instructions::JMP(uint16_t &source, uint16_t address) {
  // JMP a     11000011 lb hb    -       Unconditional jump
  source = address;
}

bool Instructions::conditionSuccessful(FlagRegister::Condition condition) {
  // Checks the condition for all the condi. jmp, ret and call instr.
  FlagRegister flagRegister = registerController->getFlagRegister();
  switch (condition) {
  case FlagRegister::Condition::Carry:
    return flagRegister.getFlag(FlagRegister::Flag::Carry) ? 1 : 0;
    break;
  case FlagRegister::Condition::NotCarry:
    return !flagRegister.getFlag(FlagRegister::Flag::Carry) ? 1 : 0;
    break;
  case FlagRegister::Condition::Minus:
    return flagRegister.getFlag(FlagRegister::Flag::Signed) ? 1 : 0;
    break;
  case FlagRegister::Condition::Positive:
    return !flagRegister.getFlag(FlagRegister::Flag::Signed) ? 1 : 0;
    break;
  case FlagRegister::Condition::ParityEven:
    return flagRegister.getFlag(FlagRegister::Flag::Parity) ? 1 : 0;
    break;
  case FlagRegister::Condition::ParityOdd:
    return !flagRegister.getFlag(FlagRegister::Flag::Parity) ? 1 : 0;
    break;
  case FlagRegister::Condition::Zero:
    return flagRegister.getFlag(FlagRegister::Flag::Zero) ? 1 : 0;
    break;
  case FlagRegister::Condition::NotZero:
    return !flagRegister.getFlag(FlagRegister::Flag::Zero) ? 1 : 0;
    break;
  }
}

void Instructions::JMPCondition(uint16_t &source, uint16_t address,
                                FlagRegister::Condition condition) {
  // Jccc a    11CCC010 lb hb    -       Conditional jump
  if (conditionSuccessful(condition)) {
    JMP(source, address);
  }
}

void Instructions::CALL(uint16_t &source, uint16_t address) {
  // CALL a    11001101 lb hb    -       Unconditional subroutine call
  registerController->getStack().pushWord(source);
  source = address;
}

void Instructions::CALLCondition(uint16_t &source, uint16_t address,
                                 FlagRegister::Condition condition) {
  // Cccc a    11CCC100 lb hb    -       Conditional subroutine call
  if (conditionSuccessful(condition)) {
    CALL(source, address);
  }
}

void Instructions::RET(uint16_t &source) {
  // RET       11001001          -       Unconditional return from subroutine
  source = registerController->getStack().popWord();
}

void Instructions::RETCondition(uint16_t &source,
                                FlagRegister::Condition condition) {
  // Rccc      11CCC000          -       Conditional return from subroutine
}

void Instructions::PCHL(uint16_t &source) {
  // PCHL      11101001          -       Jump to address in H:L
  source = registerController->getRegisterPair(RegisterPair::H);
}

void Instructions::PUSH(RegisterPair registerPair) {
  // PUSH RP   11RP0101 *2       -       Push register pair on the stack
  registerController->getStack().pushWord(
      registerController->getRegisterPair(registerPair));
}

void Instructions::POP(RegisterPair registerPair) {
  // POP RP    11RP0001 *2       *2      Pop  register pair from the stack
  registerController->setRegisterPair(registerPair,
                                      registerController->getStack().popWord());
}

void Instructions::XTHL() {
  // XTHL      11100011          -       Swap H:L with top word on stack
  uint16_t stackTemp = registerController->getStack().popWord();
  registerController->getStack().pushWord(
      registerController->getRegisterPair(RegisterPair::H));
  registerController->setRegisterPair(RegisterPair::H, stackTemp);
}

void Instructions::SPHL() {
  // SPHL      11111001          -       Set SP to content of H:L
  registerController->getStack().setStackPointer(
      registerController->getRegisterPair(RegisterPair::H));
}
