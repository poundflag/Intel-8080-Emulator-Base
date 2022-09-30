#include "instructions.h"

Instructions::Instructions(BusController &busController,
                           RegisterController &registerController,
                           IOController &ioController, ALU &alu)
    : busController(busController), registerController(registerController), ioController(ioController), alu(alu) {}

// MOV D,S   01DDDSSS - Move register to register
void Instructions::MOV(Registers::Register destination,
                       Registers::Register source) {
    registerController.setRegister(destination,
                                   registerController.getRegister(source));
    registerController.fetchNextInstruction();
}

// MVI D,#   00DDD110 db - Move immediate to register
void Instructions::MVI(Registers::Register destination) {
    switch (registerController.getMachineCycle()) {
        // Read the immediate value
        case 0:
            registerController.getProgramCounter()++;
            break;
        // Set the register to the fetched value and fetch the next instruction
        case 1:
            registerController.setRegister(destination, busController.readByte(registerController.getProgramCounter()));
        default:
            registerController.fetchNextInstruction();
            break;
    }
}

// LXI RP,#  00RP0001 lb hb - Load register pair immediate
void Instructions::LXI(RegisterPair registerPair, uint16_t immediate) {
    switch (registerController.getMachineCycle()) {
        // Read the 16-Bit Value
        case 0:
            registerController.setRegister(Registers::TemporaryHigh,
                                           busController.readByte(++registerController.getProgramCounter()));
            break;
        case 1:
            registerController.setRegister(Registers::TemporaryLow,
                                           busController.readByte(++registerController.getProgramCounter()));
            break;
        // Set the register pair to the temporary value
        case 2:
            registerController.setRegisterPair(registerPair,
                                               registerController.getRegisterPair(RegisterPair::Temporary));
        default:
            registerController.fetchNextInstruction();
            break;
    }
}

// LDA a     00111010 lb hb - Load A from memory
void Instructions::LDA() {
    memoryReadOneByte(registerController.getMachineCycle());
    switch (registerController.getMachineCycle()) {
        case 3:
            registerController.setRegister(Registers::A, busController.readByte(registerController.getRegisterPair(RegisterPair::Temporary)));
            registerController.fetchNextInstruction();
        default:
            break;
    }
}

// STA a     00110010 lb hb - Store A to memory
void Instructions::STA() {
    memoryReadOneByte(registerController.getMachineCycle());
    switch (registerController.getMachineCycle()) {
            // Save the value in the address
        case 3:
            busController.writeByte(registerController.getRegisterPair(RegisterPair::Temporary),
                                    registerController.getRegister(Registers::A));
            registerController.fetchNextInstruction();
        default:
            break;
    }
}

// LHLD a    00101010 lb hb    -       Load H:L from memory
void Instructions::LHLD() {
    uint16_t temp = 0;
   switch (registerController.getMachineCycle()) {// Read the 16-Bit Value
        case 0:
            registerController.setRegister(Registers::TemporaryLow,
                                           busController.readByte(++registerController.getProgramCounter()));
            break;
        case 1:
            registerController.setRegister(Registers::TemporaryHigh,
                                           busController.readByte(++registerController.getProgramCounter()));
            break;
        // Once the value has been loaded set the register pair
        case 2:
            temp = registerController.getProgramCounter();
            registerController.getProgramCounter() = registerController.getRegisterPair(RegisterPair::Temporary);
            registerController.setRegisterPair(RegisterPair::Temporary, temp);
            break;
        case 3:
            registerController.setRegisterPair(RegisterPair::H, busController.readWord(registerController.getProgramCounter()));
            registerController.getProgramCounter()++;
            break;
        case 4:
            temp = registerController.getProgramCounter()-1;
            registerController.getProgramCounter() = registerController.getRegisterPair(RegisterPair::Temporary);
            registerController.setRegisterPair(RegisterPair::Temporary, temp);
            registerController.setRegisterPair(RegisterPair::H,
                                               registerController.getRegisterPair(RegisterPair::Temporary));
        default:
            registerController.fetchNextInstruction();
            break;
    }
}

// SHLD a    00100010 lb hb    -       Store H:L to memory
void Instructions::SHLD() {
    uint16_t temp = 0;
    switch (registerController.getMachineCycle()) {
        case 0:
            registerController.setRegister(Registers::TemporaryLow,
                                           busController.readByte(++registerController.getProgramCounter()));
            break;
        case 1:
            registerController.setRegister(Registers::TemporaryHigh,
                                           busController.readByte(++registerController.getProgramCounter()));
            break;
        // Once the value has been loaded set the register pair
        case 2:
            temp = registerController.getProgramCounter();
            registerController.getProgramCounter() = registerController.getRegisterPair(RegisterPair::Temporary);
            registerController.setRegisterPair(RegisterPair::Temporary, temp);
            break;
        case 3:
            registerController.getProgramCounter()++;
            break;
        case 4:
            temp = registerController.getProgramCounter() - 1;
            registerController.getProgramCounter() = registerController.getRegisterPair(RegisterPair::Temporary);
            registerController.setRegisterPair(RegisterPair::Temporary, temp);
            busController.writeWord(registerController.getRegisterPair(RegisterPair::Temporary),
                                    registerController.getRegisterPair(RegisterPair::H));
        default:
            registerController.fetchNextInstruction();
            break;
    }
}

// LDAX RP   00RP1010 *1       -       Load indirect through BC or DE
void Instructions::LDAX(RegisterPair indirectAddress) {
    uint16_t temp;
    switch (registerController.getMachineCycle()) {
        case 0:
            // Set the reg. pair address in the temp register
            registerController.setRegisterPair(RegisterPair::Temporary, registerController.getRegisterPair(indirectAddress));

            // TODO LATER IN SWAP FUNCTION
            temp = registerController.getProgramCounter();
            registerController.getProgramCounter() = registerController.getRegisterPair(RegisterPair::Temporary);
            registerController.setRegisterPair(RegisterPair::Temporary, temp);
            break;
        case 1:
            temp = registerController.getProgramCounter();
            registerController.getProgramCounter() = registerController.getRegisterPair(RegisterPair::Temporary);
            registerController.setRegisterPair(RegisterPair::Temporary, temp);
            registerController.setRegister(Registers::A, busController.readByte(registerController.getRegisterPair(RegisterPair::Temporary)));
        default:
            registerController.fetchNextInstruction();
            break;
    }
}

// STAX RP   00RP0010 *1       -       Store indirect through BC or DE
void Instructions::STAX(RegisterPair indirectAddress) {
    uint16_t temp;
    switch (registerController.getMachineCycle()) {
        case 0:
            // Set the reg. pair address in the temp register
            registerController.setRegisterPair(RegisterPair::Temporary, registerController.getRegisterPair(indirectAddress));

            // TODO LATER IN SWAP FUNCTION
            temp = registerController.getProgramCounter();
            registerController.getProgramCounter() = registerController.getRegisterPair(RegisterPair::Temporary);
            registerController.setRegisterPair(RegisterPair::Temporary, temp);
            break;
        case 1:
            temp = registerController.getProgramCounter();
            registerController.getProgramCounter() = registerController.getRegisterPair(RegisterPair::Temporary);
            registerController.setRegisterPair(RegisterPair::Temporary, temp);
            busController.writeByte(registerController.getRegisterPair(RegisterPair::Temporary), registerController.getRegister(Registers::A));
        default:
            registerController.fetchNextInstruction();
            break;
    }
}

// XCHG      11101011          -       Exchange DE and HL content
void Instructions::XCHG() {
    uint16_t temp = registerController.getRegisterPair(RegisterPair::D);
    registerController.setRegisterPair(
        RegisterPair::D, registerController.getRegisterPair(RegisterPair::H));
    registerController.setRegisterPair(RegisterPair::H, temp);
    registerController.fetchNextInstruction();
}

// ADD S     10000SSS          ZSPCA   Add register to A
void Instructions::ADD(Registers::Register source) {
    uint8_t aValue = registerController.getRegister(Registers::A);
    uint8_t sourceValue = registerController.getRegister(source);

    registerController.setRegister(Registers::A,
                                   alu.performAdd(aValue, sourceValue, 0));
    registerController.fetchNextInstruction();
}

// ADI #     11000110 db       ZSCPA   Add immediate to A
void Instructions::ADI() {
    switch (registerController.getMachineCycle()) {
        case 0:
            registerController.getProgramCounter()++;
            break;
        case 1:
            uint8_t aValue = registerController.getRegister(Registers::A);

            registerController.setRegister(Registers::A,
                                           alu.performAdd(aValue, busController.readByte(registerController.getProgramCounter()), 0));
            registerController.fetchNextInstruction();
            break;
    }
}

// ADC S     10001SSS          ZSCPA   Add register to A with carry
void Instructions::ADC(Registers::Register source) {
    uint8_t aValue = registerController.getRegister(Registers::A);
    uint8_t sourceValue = registerController.getRegister(source);

    registerController.setRegister(
        Registers::A, alu.performAdd(aValue, sourceValue,
                                     registerController.getFlagRegister().getFlag(
                                         FlagRegister::Flag::Carry)));
    registerController.fetchNextInstruction();
}

// ACI #     11001110 db       ZSCPA   Add immediate to A with carry
void Instructions::ACI() {
    switch (registerController.getMachineCycle()) {
        case 0:
            registerController.getProgramCounter()++;
            break;
        case 1:
            uint8_t aValue = registerController.getRegister(Registers::A);
            registerController.setRegister(Registers::A, alu.performAdd(aValue, busController.readByte(registerController.getProgramCounter()),
                                                                        registerController.getFlagRegister().getFlag(
                                                                            FlagRegister::Flag::Carry)));
            registerController.fetchNextInstruction();
            break;
    }
}

// SUB S     10010SSS          ZSCPA   Subtract register from A
void Instructions::SUB(Registers::Register source) {
    uint8_t aValue = registerController.getRegister(Registers::A);
    uint8_t sourceValue = registerController.getRegister(source);

    registerController.setRegister(Registers::A, alu.performSub(aValue, sourceValue, 0));
    registerController.fetchNextInstruction();
}

// SUI #     11010110 db       ZSCPA   Subtract immediate from A
void Instructions::SUI() {
    switch (registerController.getMachineCycle()) {
        case 0:
            registerController.getProgramCounter()++;
            break;
        case 1:
            uint8_t aValue = registerController.getRegister(Registers::A);

            registerController
                .setRegister(Registers::A, alu.performSub(
                                               aValue, busController.readByte(registerController.getProgramCounter()), 0));
            registerController.fetchNextInstruction();
            break;
    }
}

// SBB S     10011SSS          ZSCPA   Subtract register from A with borrow
void Instructions::SBB(Registers::Register source) {
    uint8_t aValue = registerController.getRegister(Registers::A);
    uint8_t sourceValue = registerController.getRegister(source);

    registerController.setRegister(Registers::A, alu.performSub(
                                                     aValue, sourceValue,
                                                     registerController.getFlagRegister().getFlag(FlagRegister::Carry)));
    registerController.fetchNextInstruction();
}

// SBI #     11011110 db       ZSCPA   Subtract immediate from A with borrow
void Instructions::SBI() {
    switch (registerController.getMachineCycle()) {
        case 0:
            registerController.getProgramCounter()++;
            break;
        case 1:
            uint8_t aValue = registerController.getRegister(Registers::A);

            registerController.setRegister(Registers::A, alu.performSub(
                                                             aValue, busController.readByte(registerController.getProgramCounter()),
                                                             registerController.getFlagRegister().getFlag(FlagRegister::Carry)));
            registerController.fetchNextInstruction();
            break;
    }
}

// INR D     00DDD100          ZSPA    Increment register
void Instructions::INR(Registers::Register destination) {
    uint8_t temp = registerController.getRegister(destination);
    bool acResult = (temp & 0x0F) == 0x0F;
    registerController.setRegister(destination, temp + 1);
    registerController.getFlagRegister().processFlags(
        FlagRegister::FlagRule::Partial, temp, 1, "+");
    registerController.getFlagRegister().setFlag(FlagRegister::AuxiliaryCarry,
                                                 acResult);
    registerController.fetchNextInstruction();
}

// DCR D     00DDD101          ZSPA    Decrement register
void Instructions::DCR(Registers::Register destination) {
    uint8_t temp = registerController.getRegister(destination);
    bool acResult = (!(temp & 0x0F)) == 0x0;
    registerController.setRegister(destination, temp - 1);
    registerController.getFlagRegister().processFlags(
        FlagRegister::FlagRule::Partial, temp, 1, "-");
    registerController.getFlagRegister().setFlag(FlagRegister::AuxiliaryCarry,
                                                 acResult);
    registerController.fetchNextInstruction();
}

// INX RP    00RP0011          -       Increment register pair
void Instructions::INX(RegisterPair destination) {
    uint16_t temp = registerController.getRegisterPair(destination);
    registerController.setRegisterPair(destination, temp + 1);
    registerController.fetchNextInstruction();
}

// DCX RP    00RP1011          -       Decrement register pair
void Instructions::DCX(RegisterPair destination) {
    uint16_t temp = registerController.getRegisterPair(destination);
    registerController.setRegisterPair(destination, temp - 1);
    registerController.fetchNextInstruction();
}

// DAD RP    00RP1001          C       Add register pair to HL (16 bit add)
void Instructions::DAD(RegisterPair source) {
    uint16_t sourceValue = registerController.getRegisterPair(source);
    uint16_t hValue = registerController.getRegisterPair(RegisterPair::H);
    bool carryResult = (hValue + sourceValue) > 0xFFFF;
    registerController.setRegisterPair(RegisterPair::H, hValue + sourceValue);
    registerController.getFlagRegister().setFlag(FlagRegister::Carry,
                                                 carryResult);
    registerController.fetchNextInstruction();
}

// DAA       00100111          ZSPCA   Decimal Adjust accumulator
void Instructions::DAA() {
    // Stolen from
    // https://github.com/GunshipPenguin/lib8080/blob/master/src/i8080.c
    // Line: 405 | I don't really know, what this mess does
    int value = registerController.getRegister(Registers::A);

    uint8_t tempValue = 0;

    // If the least significant four bits of the accumulator represents a number
    // greater than 9, or if the Auxiliary Carry bit is equal to one, the
    // accumulator is incremented by six. Otherwise, no incrementing occurs.
    if (((value & 0xF) > 9) || registerController.getFlagRegister().getFlag(
                                   FlagRegister::Flag::AuxiliaryCarry)) {
        tempValue += 0x06;
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

    registerController.setRegister(Registers::A, value + tempValue);
    registerController.fetchNextInstruction();
}

// ANA S     10100SSS          ZSCPA   AND register with A
void Instructions::ANA(Registers::Register source) {
    uint8_t sourceValue = registerController.getRegister(source);
    uint8_t aValue = registerController.getRegister(Registers::A);

    registerController.setRegister(Registers::A, alu.performAnd(aValue, sourceValue));
    registerController.fetchNextInstruction();
}

// ANI #     11100110 db       ZSPCA   AND immediate with A
void Instructions::ANI() {
    switch (registerController.getMachineCycle()) {
        case 0:
            registerController.getProgramCounter()++;
            break;
        case 1:
            uint8_t aValue = registerController.getRegister(Registers::A);

            registerController.setRegister(Registers::A, alu.performAnd(aValue, busController.readByte(registerController.getProgramCounter())));
            registerController.fetchNextInstruction();
            break;
    }
}

// ORA S     10110SSS          ZSPCA   OR  register with A
void Instructions::ORA(Registers::Register source) {
    uint8_t sourceValue = registerController.getRegister(source);
    uint8_t aValue = registerController.getRegister(Registers::A);

    registerController.setRegister(Registers::A, alu.performOr(aValue, sourceValue));
    registerController.fetchNextInstruction();
}

// ORI #     11110110          ZSPCA   OR  immediate with A
void Instructions::ORI() {
    switch (registerController.getMachineCycle()) {
        case 0:
            registerController.getProgramCounter()++;
            break;
        case 1:
            uint8_t aValue = registerController.getRegister(Registers::A);

            registerController.setRegister(Registers::A, alu.performOr(aValue, busController.readByte(registerController.getProgramCounter())));
            registerController.fetchNextInstruction();
            break;
    }
}

// XRA S     10101SSS          ZSPCA   ExclusiveOR register with A
void Instructions::XRA(Registers::Register source) {
    uint8_t sourceValue = registerController.getRegister(source);
    uint8_t aValue = registerController.getRegister(Registers::A);
    registerController.setRegister(Registers::A, aValue ^ sourceValue);
    registerController.getFlagRegister().processFlags(
        FlagRegister::FlagRule::All, aValue ^ sourceValue, 0, "+");
    registerController.fetchNextInstruction();
}

// XRI #     11101110 db       ZSPCA   ExclusiveOR immediate with A
void Instructions::XRI() {
    switch (registerController.getMachineCycle()) {
        case 0:
            registerController.getProgramCounter()++;
            break;
        case 1:
            uint8_t aValue = registerController.getRegister(Registers::A);
            registerController.setRegister(Registers::A, aValue ^ busController.readByte(registerController.getProgramCounter()));
            registerController.getFlagRegister().processFlags(FlagRegister::FlagRule::All,
                                                              aValue ^ busController.readByte(registerController.getProgramCounter()), 0, "+");
            registerController.fetchNextInstruction();
            break;
    }
}

// CMP S     10111SSS          ZSPCA   Compare register with A
void Instructions::CMP(Registers::Register source) {
    uint8_t intermediate =
        registerController.getRegister(Registers::A);
    SUB(source);
    registerController.setRegister(Registers::A, intermediate);
    registerController.fetchNextInstruction();
}

// CPI #     11111110          ZSPCA   Compare immediate with A
void Instructions::CPI() {
    switch (registerController.getMachineCycle()) {
        case 0:
            registerController.getProgramCounter()++;
            break;
        case 1:
            uint8_t aValue = registerController.getRegister(Registers::A);

            registerController
                .setRegister(Registers::A, alu.performSub(
                                               aValue, busController.readByte(registerController.getProgramCounter()), 0));

            registerController.setRegister(Registers::A, aValue);
            registerController.fetchNextInstruction();
            break;
    }
}

// RLC       00000111          C       Rotate A left
void Instructions::RLC() {
    uint16_t value = registerController.getRegister(Registers::A);
    value = value << 1 | ((value & 0x80) >> 7);
    registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry,
                                                 (value & 0x100) == 0x100);

    registerController.setRegister(Registers::A, value);
    registerController.fetchNextInstruction();
}

// RRC       00001111          C       Rotate A right
void Instructions::RRC() {
    uint16_t value = registerController.getRegister(Registers::A);

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
    registerController.setRegister(Registers::A, value);
    registerController.fetchNextInstruction();
}

// RAL       00010111          C       Rotate A left through carry
void Instructions::RAL() {
    uint16_t value = registerController.getRegister(Registers::A);

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
    registerController.setRegister(Registers::A, value);
    registerController.fetchNextInstruction();
}

// RAR       00011111          C       Rotate A right through carry
void Instructions::RAR() {
    uint16_t value = registerController.getRegister(Registers::A);

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
    registerController.setRegister(Registers::A, value);
    registerController.fetchNextInstruction();
}

// CMA       00101111          -       Compliment A
void Instructions::CMA() {
    uint8_t value = registerController.getRegister(Registers::A);
    registerController.setRegister(Registers::A, ~value);
    registerController.fetchNextInstruction();
}

// CMC       00111111          C       Compliment Carry flag
void Instructions::CMC() {
    bool carry =
        registerController.getFlagRegister().getFlag(FlagRegister::Flag::Carry);
    registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry,
                                                 !carry);
    registerController.fetchNextInstruction();
}

// STC       00110111          C       Set Carry flag
void Instructions::STC() {
    registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, true);
    registerController.fetchNextInstruction();
}

// JMP a     11000011 lb hb    -       Unconditional jump
void Instructions::JMP() {
    uint16_t temp = 0;
    switch (registerController.getMachineCycle()) {
        case 0:
            registerController.setRegister(Registers::TemporaryLow,
                                           busController.readByte(++registerController.getProgramCounter()));
            break;
        case 1:
            registerController.setRegister(Registers::TemporaryHigh,
                                           busController.readByte(++registerController.getProgramCounter()));
            break;
        // Once the value has been loaded set the register pair
        case 2:
            registerController.getProgramCounter() = registerController.getRegisterPair(RegisterPair::Temporary)-1;
        default:
            registerController.fetchNextInstruction();
            break;
    }
}

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
bool Instructions::JMPCondition(FlagRegister::Condition condition) {
    bool success = conditionSuccessful(condition);
    if (success) {
        JMP();
    } else {
        // If the condition fails, then skip over the current jmp instr.
        registerController.getProgramCounter()+=2;
        registerController.fetchNextInstruction();
    }
    return success;
}

// CALL a    11001101 lb hb    -       Unconditional subroutine call
void Instructions::CALL() {
    switch (registerController.getMachineCycle()) {
        case 0:
            registerController.setRegister(Registers::TemporaryLow,
                                           busController.readByte(++registerController.getProgramCounter()));
            break;
        case 1:
            registerController.setRegister(Registers::TemporaryHigh,
                                           busController.readByte(++registerController.getProgramCounter()));
            break;
        // Once the value has been loaded set the register pair
        case 2:
            registerController.getStack().pushWord(registerController.getProgramCounter() + 1);
            registerController.getProgramCounter() = registerController.getRegisterPair(RegisterPair::Temporary) - 1;
        default:
            registerController.fetchNextInstruction();
            break;
    }
}

// Cccc a    11CCC100 lb hb    -       Conditional subroutine call
bool Instructions::CALLCondition(FlagRegister::Condition condition) {
    bool success = conditionSuccessful(condition);
    if (success) {
        CALL();
    } else {
        // If the condition fails, then skip over the current call instr.
        registerController.getProgramCounter()+=2;
        registerController.fetchNextInstruction();
    }
    return success;
}

// RET       11001001          -       Unconditional return from subroutine
void Instructions::RET() {
    registerController.getProgramCounter() = registerController.getStack().popWord() + 1;
    registerController.fetchNextInstruction();
}

// Rccc      11CCC000          -       Conditional return from subroutine
bool Instructions::RETCondition(FlagRegister::Condition condition) {
    bool success = conditionSuccessful(condition);
    if (success) {
        RET();
    } else {
        // If the condition fails, then skip over the current call instr.
        registerController.getProgramCounter();
        registerController.fetchNextInstruction();
    }
    return success;
}

// RST n     11NNN111          -       Restart (Call n*8)
void Instructions::RST(uint8_t n) {
    registerController.getStack().pushWord(registerController.getProgramCounter() + 1);
    registerController.getProgramCounter() = (8 * n) - 1;
    registerController.fetchNextInstruction();
}

// PCHL      11101001          -       Jump to address in H:L
void Instructions::PCHL(uint16_t &source) {
    source = registerController.getRegisterPair(RegisterPair::H);
    registerController.fetchNextInstruction();
}

// PUSH RP   11RP0101 *2       -       Push register pair on the stack
void Instructions::PUSH(RegisterPair registerPair) {
    uint16_t temp;
    switch (registerController.getMachineCycle()) {
        case 0:
            temp = registerController.getProgramCounter();
            registerController.getProgramCounter() = registerController.getStack().getStackPointer() - 1;
            registerController.setRegisterPair(RegisterPair::Temporary, temp);
            break;
        case 1:
            registerController.getProgramCounter()--;
            break;
        case 2:
            registerController.getProgramCounter() = registerController.getRegisterPair(RegisterPair::Temporary);

            registerController.getStack().pushWord(
                registerController.getRegisterPair(registerPair));
            registerController.fetchNextInstruction();
            break;
    }
}

// POP RP    11RP0001 *2       *2      Pop  register pair from the stack
void Instructions::POP(RegisterPair registerPair) {
    uint16_t temp;
    switch (registerController.getMachineCycle()) {
        case 0:
            temp = registerController.getProgramCounter();
            registerController.getProgramCounter() = registerController.getStack().getStackPointer();
            registerController.setRegisterPair(RegisterPair::Temporary, temp);
            break;
        case 1:
            registerController.getProgramCounter()++;
            break;
        case 2:
            registerController.getProgramCounter() = registerController.getRegisterPair(RegisterPair::Temporary);

            registerController.setRegisterPair(registerPair,
                                               registerController.getStack().popWord());
            registerController.fetchNextInstruction();
            break;
    }
}

// XTHL      11100011          -       Swap H:L with top word on stack
void Instructions::XTHL() {
    uint16_t stackTemp = registerController.getStack().popWord();
    registerController.getStack().pushWord(
        registerController.getRegisterPair(RegisterPair::H));
    registerController.setRegisterPair(RegisterPair::H, stackTemp);
    registerController.fetchNextInstruction();
}

// SPHL      11111001          -       Set SP to content of H:L
void Instructions::SPHL() {
    registerController.getStack().setStackPointer(
        registerController.getRegisterPair(RegisterPair::H));
    registerController.fetchNextInstruction();
}

// IN p      11011011 pa       -       Read input port into A
void Instructions::IN(int portNumber) {
    registerController.setRegister(Registers::A, ioController.getDeviceValue(portNumber));
    registerController.fetchNextInstruction();
}

// OUT p     11010011 pa       -       Write A to output port
void Instructions::OUT(int portNumber) {
    ioController.setDeviceValue(
        portNumber, registerController.getRegister(Registers::A));
    registerController.fetchNextInstruction();
}

// HLT       01110110          -       Halt processor
bool Instructions::HLT() { return true; }

void Instructions::memoryReadOneByte(uint8_t machineCycle) {
    uint16_t temp = 0;
    switch (machineCycle) {
        // Read the 16-Bit Address
        case 0:
            registerController.setRegister(Registers::TemporaryHigh,
                                           busController.readByte(++registerController.getProgramCounter()));
            break;
        case 1:
            registerController.setRegister(Registers::TemporaryLow,
                                           busController.readByte(++registerController.getProgramCounter()));
            break;
            // Switch the temporary register with the program counter
            // To simulate the fetch instruction
        case 2:
            temp = registerController.getProgramCounter();
            registerController.getProgramCounter() = registerController.getRegisterPair(RegisterPair::Temporary);
            registerController.setRegisterPair(RegisterPair::Temporary, temp);
            break;
        case 3:
            temp = registerController.getRegisterPair(RegisterPair::Temporary);
            registerController.setRegisterPair(RegisterPair::Temporary, registerController.getProgramCounter());
            registerController.getProgramCounter() = temp;
        default:
            break;
    }
}