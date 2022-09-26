#include "registercontroller.h"

RegisterController::RegisterController(BusController &busController)
    : busController(busController) {}

uint8_t RegisterController::getRegister(Registers::Register registerIndex) {
    if (registerIndex == Registers::MemoryReference) {
        return busController.readByte(getRegisterPair(RegisterPair::H));
    }
    return registers[registerIndex];
}

void RegisterController::setRegister(Registers::Register registerIndex,
                                     uint8_t value) {
    if (registerIndex == Registers::MemoryReference) {
        busController.writeByte(getRegisterPair(RegisterPair::H), value);
    }
    registers[registerIndex] = value;
}

uint16_t RegisterController::getRegisterPair(RegisterPair registerPair) {
    // Decode the enum and get the value
    switch (registerPair) {
        case B:
            return (getRegister(Registers::B) << 8) | getRegister(Registers::C);
            break;
        case D:
            return (getRegister(Registers::D) << 8) | getRegister(Registers::E);
            break;
        case H:
            return (getRegister(Registers::H) << 8) | getRegister(Registers::L);
            break;
        case PSW:
            return (getRegister(Registers::A) << 8) | getFlagRegister().getRegister();
            break;
        case SP:
            return stackRegister.getStackPointer();
            break;
        case Temporary:
            return (getRegister(Registers::TemporaryHigh) << 8) | getRegister(Registers::TemporaryLow);
    };
    return 0;
}

void RegisterController::setRegisterPair(RegisterPair registerPair,
                                         uint16_t value) {
    // Decode the enum and set the value
    uint8_t higherByte = value & 0xFF;
    uint8_t lowerByte = value >> 8;
    switch (registerPair) {
        case B:
            setRegister(Registers::B, lowerByte);
            setRegister(Registers::C, higherByte);
            break;
        case D:
            setRegister(Registers::D, lowerByte);
            setRegister(Registers::E, higherByte);
            break;
        case H:
            setRegister(Registers::H, lowerByte);
            setRegister(Registers::L, higherByte);
            break;
        case PSW:
            setRegister(Registers::A, lowerByte);
            getFlagRegister().setRegister(higherByte);
            break;
        case SP:
            stackRegister.setStackPointer(value);
            break;
        case Temporary:
            setRegister(Registers::TemporaryHigh, lowerByte);
            setRegister(Registers::TemporaryLow, higherByte);
            break;
    };
}

uint16_t &RegisterController::getProgramCounter() { return programCounter; }

void RegisterController::fetchNextInstruction() {
    machineCycle = 0xFF;
    setRegister(Registers::InstructionRegister,
                busController.readByte(++programCounter));
}

uint8_t RegisterController::getMachineCycle() {
    return machineCycle;
}

void RegisterController::setMachineCycle(uint8_t value) {
    machineCycle = value;
}

void RegisterController::incrementMachineCycle() {
    machineCycle++;    
}

FlagRegister &RegisterController::getFlagRegister() { return flagRegister; }

Stack &RegisterController::getStack() { return stackRegister; }

RegisterController::~RegisterController() {}


