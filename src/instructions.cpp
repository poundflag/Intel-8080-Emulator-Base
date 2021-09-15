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

void Instructions::JMP(uint16_t &source, uint16_t address) {
  // JMP a     11000011 lb hb    -       Unconditional jump
  source = address;
}

void Instructions::CALL(uint16_t &source, uint16_t address) {
  // CALL a    11001101 lb hb    -       Unconditional subroutine call
  registerController->getStack().pushWord(source);
  source = address;
}
