#include "instructiondecoder.h"

InstructionDecoder::InstructionDecoder(Instructions &instructions,
                                       BusController &busController,
                                       RegisterController &registerController)
    : instructions(instructions), busController(busController),
      registerController(registerController) {}

// Decode all instructions
bool InstructionDecoder::instructionDecoder(uint8_t opcode) {
    if (registerController.getProgramCounter() == 0x0100) {
        std::cout << "";
    }
    bool skipIncrement = false;
    switch (opcode) {
            // NOP Opcodes
        case 0:
        case 0x10:
        case 0x20:
        case 0x30:
        case 0x08:
        case 0x18:
        case 0x28:
        case 0x38:
            registerController.fetchNextInstruction();
            break;

        // LXI Opcodes
        case 0x01:
            instructions.LXI(RegisterPair::B);
            break;
        case 0x11:
            instructions.LXI(RegisterPair::D);
            break;
        case 0x21:
            instructions.LXI(RegisterPair::H);
            break;
        case 0x31:
            instructions.LXI(RegisterPair::SP);
            break;

        // STAX Opcodes
        case 0x02:
            instructions.STAX(RegisterPair::B);
            break;
        case 0x12:
            instructions.STAX(RegisterPair::D);
            break;

        // SHLD Opcode
        case 0x22:
            instructions.SHLD();
            break;

            // STA Opcode
        case 0x32:
            instructions.STA();
            break;

        // INX Opcodes
        case 0x03:
            instructions.INX(RegisterPair::B);
            break;
        case 0x13:
            instructions.INX(RegisterPair::D);
            break;
        case 0x23:
            instructions.INX(RegisterPair::H);
            break;
        case 0x33:
            instructions.INX(RegisterPair::SP);
            break;

        // INR Opcodes
        case 0x3C:
            instructions.INR(Registers::A);
            break;
        case 0x04:
            instructions.INR(Registers::B);
            break;
        case 0x0C:
            instructions.INR(Registers::C);
            break;
        case 0x14:
            instructions.INR(Registers::D);
            break;
        case 0x1C:
            instructions.INR(Registers::E);
            break;
        case 0x24:
            instructions.INR(Registers::H);
            break;
        case 0x2C:
            instructions.INR(Registers::L);
            break;
        case 0x34:
            instructions.INR(Registers::MemoryReference);
            break;

        // DCR Opcodes
        case 0x3D:
            instructions.DCR(Registers::A);
            break;
        case 0x05:
            instructions.DCR(Registers::B);
            break;
        case 0x0D:
            instructions.DCR(Registers::C);
            break;
        case 0x15:
            instructions.DCR(Registers::D);
            break;
        case 0x1D:
            instructions.DCR(Registers::E);
            break;
        case 0x25:
            instructions.DCR(Registers::H);
            break;
        case 0x2D:
            instructions.DCR(Registers::L);
            break;
        case 0x35:
            instructions.DCR(Registers::MemoryReference);
            break;

        // MVI Opcodes
        case 0x3E:
            instructions.MVI(Registers::A);
            break;
        case 0x06:
            instructions.MVI(Registers::B);
            break;
        case 0x0E:
            instructions.MVI(Registers::C);
            break;
        case 0x16:
            instructions.MVI(Registers::D);
            break;
        case 0x1E:
            instructions.MVI(Registers::E);
            break;
        case 0x26:
            instructions.MVI(Registers::H);
            break;
        case 0x2E:
            instructions.MVI(Registers::L);
            break;
        case 0x36:
            instructions.MVI(Registers::MemoryReference);
            break;

        // RLC Opcode
        case 0x07:
            instructions.RLC();
            break;

        // RRC Opcode
        case 0x0F:
            instructions.RRC();
            break;

        // RAL Opcode
        case 0x17:
            instructions.RAL();
            break;

        // RAR Opcode
        case 0x1F:
            instructions.RAR();
            break;

        // DAA Opcode
        case 0x27:
            instructions.DAA();
            break;

        // STC Opcode
        case 0x37:
            instructions.STC();
            break;

        // CMA Opcode
        case 0x2F:
            instructions.CMA();
            break;

        // CMC Opcode
        case 0x3F:
            instructions.CMC();
            break;

        // DAD Opcodes
        case 0x09:
            instructions.DAD(RegisterPair::B);
            break;
        case 0x19:
            instructions.DAD(RegisterPair::D);
            break;
        case 0x29:
            instructions.DAD(RegisterPair::H);
            break;
        case 0x39:
            instructions.DAD(RegisterPair::SP);
            break;

        // LDAX Opcodes
        case 0x0A:
            instructions.LDAX(RegisterPair::B);
            break;
        case 0x1A:
            instructions.LDAX(RegisterPair::D);
            break;

        // LHLD Opcode
        case 0x2A:
            instructions.LHLD();
            break;

        // LDA Opcode
        case 0x3A:
            instructions.LDA();
            break;

        // DCX Opcode
        case 0x0B:
            instructions.DCX(RegisterPair::B);
            break;
        case 0x1B:
            instructions.DCX(RegisterPair::D);
            break;
        case 0x2B:
            instructions.DCX(RegisterPair::H);
            break;
        case 0x3B:
            instructions.DCX(RegisterPair::SP);
            break;

        // MOV Opcodes
        case 0x40:
            instructions.MOV(Registers::B, Registers::B);
            break;
        case 0x41:
            instructions.MOV(Registers::B, Registers::C);
            break;
        case 0x42:
            instructions.MOV(Registers::B, Registers::D);
            break;
        case 0x43:
            instructions.MOV(Registers::B, Registers::E);
            break;
        case 0x44:
            instructions.MOV(Registers::B, Registers::H);
            break;
        case 0x45:
            instructions.MOV(Registers::B, Registers::L);
            break;
        case 0x46:
            instructions.MOV(Registers::B, Registers::MemoryReference);
            break;
        case 0x47:
            instructions.MOV(Registers::B, Registers::A);
            break;
        case 0x48:
            instructions.MOV(Registers::C, Registers::B);
            break;
        case 0x49:
            instructions.MOV(Registers::C, Registers::C);
            break;
        case 0x4A:
            instructions.MOV(Registers::C, Registers::D);
            break;
        case 0x4B:
            instructions.MOV(Registers::C, Registers::E);
            break;
        case 0x4C:
            instructions.MOV(Registers::C, Registers::H);
            break;
        case 0x4D:
            instructions.MOV(Registers::C, Registers::L);
            break;
        case 0x4E:
            instructions.MOV(Registers::C, Registers::MemoryReference);
            break;
        case 0x4F:
            instructions.MOV(Registers::C, Registers::A);
            break;
        case 0x50:
            instructions.MOV(Registers::D, Registers::B);
            break;
        case 0x51:
            instructions.MOV(Registers::D, Registers::C);
            break;
        case 0x52:
            instructions.MOV(Registers::D, Registers::D);
            break;
        case 0x53:
            instructions.MOV(Registers::D, Registers::E);
            break;
        case 0x54:
            instructions.MOV(Registers::D, Registers::H);
            break;
        case 0x55:
            instructions.MOV(Registers::D, Registers::L);
            break;
        case 0x56:
            instructions.MOV(Registers::D, Registers::MemoryReference);
            break;
        case 0x57:
            instructions.MOV(Registers::D, Registers::A);
            break;
        case 0x58:
            instructions.MOV(Registers::E, Registers::B);
            break;
        case 0x59:
            instructions.MOV(Registers::E, Registers::C);
            break;
        case 0x5A:
            instructions.MOV(Registers::E, Registers::D);
            break;
        case 0x5B:
            instructions.MOV(Registers::E, Registers::E);
            break;
        case 0x5C:
            instructions.MOV(Registers::E, Registers::H);
            break;
        case 0x5D:
            instructions.MOV(Registers::E, Registers::L);
            break;
        case 0x5E:
            instructions.MOV(Registers::E, Registers::MemoryReference);
            break;
        case 0x5F:
            instructions.MOV(Registers::E, Registers::A);
            break;
        case 0x60:
            instructions.MOV(Registers::H, Registers::B);
            break;
        case 0x61:
            instructions.MOV(Registers::H, Registers::C);
            break;
        case 0x62:
            instructions.MOV(Registers::H, Registers::D);
            break;
        case 0x63:
            instructions.MOV(Registers::H, Registers::E);
            break;
        case 0x64:
            instructions.MOV(Registers::H, Registers::H);
            break;
        case 0x65:
            instructions.MOV(Registers::H, Registers::L);
            break;
        case 0x66:
            instructions.MOV(Registers::H, Registers::MemoryReference);
            break;
        case 0x67:
            instructions.MOV(Registers::H, Registers::A);
            break;
        case 0x68:
            instructions.MOV(Registers::L, Registers::B);
            break;
        case 0x69:
            instructions.MOV(Registers::L, Registers::C);
            break;
        case 0x6A:
            instructions.MOV(Registers::L, Registers::D);
            break;
        case 0x6B:
            instructions.MOV(Registers::L, Registers::E);
            break;
        case 0x6C:
            instructions.MOV(Registers::L, Registers::H);
            break;
        case 0x6D:
            instructions.MOV(Registers::L, Registers::L);
            break;
        case 0x6E:
            instructions.MOV(Registers::L, Registers::MemoryReference);
            break;
        case 0x6F:
            instructions.MOV(Registers::L, Registers::A);
            break;
        case 0x70:
            instructions.MOV(Registers::MemoryReference, Registers::B);
            break;
        case 0x71:
            instructions.MOV(Registers::MemoryReference, Registers::C);
            break;
        case 0x72:
            instructions.MOV(Registers::MemoryReference, Registers::D);
            break;
        case 0x73:
            instructions.MOV(Registers::MemoryReference, Registers::E);
            break;
        case 0x74:
            instructions.MOV(Registers::MemoryReference, Registers::H);
            break;
        case 0x75:
            instructions.MOV(Registers::MemoryReference, Registers::L);
            break;
        case 0x77:
            instructions.MOV(Registers::MemoryReference, Registers::A);
            break;
        case 0x78:
            instructions.MOV(Registers::A, Registers::B);
            break;
        case 0x79:
            instructions.MOV(Registers::A, Registers::C);
            break;
        case 0x7A:
            instructions.MOV(Registers::A, Registers::D);
            break;
        case 0x7B:
            instructions.MOV(Registers::A, Registers::E);
            break;
        case 0x7C:
            instructions.MOV(Registers::A, Registers::H);
            break;
        case 0x7D:
            instructions.MOV(Registers::A, Registers::L);
            break;
        case 0x7E:
            instructions.MOV(Registers::A, Registers::MemoryReference);
            break;
        case 0x7F:
            instructions.MOV(Registers::A, Registers::A);
            break;

            // ADD Opcodes
        case 0x80:
            instructions.ADD(Registers::B);
            break;
        case 0x81:
            instructions.ADD(Registers::C);
            break;
        case 0x82:
            instructions.ADD(Registers::D);
            break;
        case 0x83:
            instructions.ADD(Registers::E);
            break;
        case 0x84:
            instructions.ADD(Registers::H);
            break;
        case 0x85:
            instructions.ADD(Registers::L);
            break;
        case 0x86:
            instructions.ADD(Registers::MemoryReference);
            break;
        case 0x87:
            instructions.ADD(Registers::A);
            break;

            // ADC Opcodes
        case 0x88:
            instructions.ADC(Registers::B);
            break;
        case 0x89:
            instructions.ADC(Registers::C);
            break;
        case 0x8A:
            instructions.ADC(Registers::D);
            break;
        case 0x8B:
            instructions.ADC(Registers::E);
            break;
        case 0x8C:
            instructions.ADC(Registers::H);
            break;
        case 0x8D:
            instructions.ADC(Registers::L);
            break;
        case 0x8E:
            instructions.ADC(Registers::MemoryReference);
            break;
        case 0x8F:
            instructions.ADC(Registers::A);
            break;

            // SUB Opcodes
        case 0x90:
            instructions.SUB(Registers::B);
            break;
        case 0x91:
            instructions.SUB(Registers::C);
            break;
        case 0x92:
            instructions.SUB(Registers::D);
            break;
        case 0x93:
            instructions.SUB(Registers::E);
            break;
        case 0x94:
            instructions.SUB(Registers::H);
            break;
        case 0x95:
            instructions.SUB(Registers::L);
            break;
        case 0x96:
            instructions.SUB(Registers::MemoryReference);
            break;
        case 0x97:
            instructions.SUB(Registers::A);
            break;
        case 0x98:
            instructions.SBB(Registers::B);
            break;
        case 0x99:
            instructions.SBB(Registers::C);
            break;
        case 0x9A:
            instructions.SBB(Registers::D);
            break;
        case 0x9B:
            instructions.SBB(Registers::E);
            break;
        case 0x9C:
            instructions.SBB(Registers::H);
            break;
        case 0x9D:
            instructions.SBB(Registers::L);
            break;
        case 0x9E:
            instructions.SBB(Registers::MemoryReference);
            break;
        case 0x9F:
            instructions.SBB(Registers::A);
            break;

            // ANA Opcodes
        case 0xA0:
            instructions.ANA(Registers::B);
            break;
        case 0xA1:
            instructions.ANA(Registers::C);
            break;
        case 0xA2:
            instructions.ANA(Registers::D);
            break;
        case 0xA3:
            instructions.ANA(Registers::E);
            break;
        case 0xA4:
            instructions.ANA(Registers::H);
            break;
        case 0xA5:
            instructions.ANA(Registers::L);
            break;
        case 0xA6:
            instructions.ANA(Registers::MemoryReference);
            break;
        case 0xA7:
            instructions.ANA(Registers::A);
            break;

            // XRA Opcodes
        case 0xA8:
            instructions.XRA(Registers::B);
            break;
        case 0xA9:
            instructions.XRA(Registers::C);
            break;
        case 0xAA:
            instructions.XRA(Registers::D);
            break;
        case 0xAB:
            instructions.XRA(Registers::E);
            break;
        case 0xAC:
            instructions.XRA(Registers::H);
            break;
        case 0xAD:
            instructions.XRA(Registers::L);
            break;
        case 0xAE:
            instructions.XRA(Registers::MemoryReference);
            break;
        case 0xAF:
            instructions.XRA(Registers::A);
            break;

            // ORA Opcodes
        case 0xB0:
            instructions.ORA(Registers::B);
            break;
        case 0xB1:
            instructions.ORA(Registers::C);
            break;
        case 0xB2:
            instructions.ORA(Registers::D);
            break;
        case 0xB3:
            instructions.ORA(Registers::E);
            break;
        case 0xB4:
            instructions.ORA(Registers::H);
            break;
        case 0xB5:
            instructions.ORA(Registers::L);
            break;
        case 0xB6:
            instructions.ORA(Registers::MemoryReference);
            break;
        case 0xB7:
            instructions.ORA(Registers::A);
            break;

            // CMP Opcodes
        case 0xB8:
            instructions.CMP(Registers::B);
            break;
        case 0xB9:
            instructions.CMP(Registers::C);
            break;
        case 0xBA:
            instructions.CMP(Registers::D);
            break;
        case 0xBB:
            instructions.CMP(Registers::E);
            break;
        case 0xBC:
            instructions.CMP(Registers::H);
            break;
        case 0xBD:
            instructions.CMP(Registers::L);
            break;
        case 0xBE:
            instructions.CMP(Registers::MemoryReference);
            break;
        case 0xBF:
            instructions.CMP(Registers::A);
            break;

        // Rccc Opcodes
        case 0xC0:
            skipIncrement =
                instructions.RETCondition(FlagRegister::Condition::NotZero);
            break;
        case 0xD0:
            skipIncrement =
                instructions.RETCondition(FlagRegister::Condition::NotCarry);
            break;
        case 0xE0:
            skipIncrement =
                instructions.RETCondition(FlagRegister::Condition::ParityOdd);
            break;
        case 0xF0:
            skipIncrement =
                instructions.RETCondition(FlagRegister::Condition::Positive);
            break;
        case 0xC8:
            skipIncrement = instructions.RETCondition(FlagRegister::Condition::Zero);
            break;
        case 0xD8:
            skipIncrement = instructions.RETCondition(FlagRegister::Condition::Carry);
            break;
        case 0xE8:
            skipIncrement =
                instructions.RETCondition(FlagRegister::Condition::ParityEven);
            break;
        case 0xF8:
            skipIncrement = instructions.RETCondition(FlagRegister::Condition::Minus);
            break;

        // POP Opcodes
        case 0xC1:
            instructions.POP(RegisterPair::B);
            break;
        case 0xD1:
            instructions.POP(RegisterPair::D);
            break;
        case 0xE1:
            instructions.POP(RegisterPair::H);
            break;
        case 0xF1:
            instructions.POP(RegisterPair::PSW);
            break;

        // Jccc Opcodes
        case 0xC2:
            skipIncrement = instructions.JMPCondition(
                FlagRegister::Condition::NotZero);
            break;
        case 0xD2:
            skipIncrement = instructions.JMPCondition(
                FlagRegister::Condition::NotCarry);
            break;
        case 0xE2:
            skipIncrement = instructions.JMPCondition(
                FlagRegister::Condition::ParityOdd);
            break;
        case 0xF2:
            skipIncrement = instructions.JMPCondition(
                FlagRegister::Condition::Positive);
            break;
        case 0xCA:
            skipIncrement =
                instructions.JMPCondition(FlagRegister::Condition::Zero);
            break;
        case 0xDA:
            skipIncrement = instructions.JMPCondition(
                FlagRegister::Condition::Carry);
            break;
        case 0xEA:
            skipIncrement = instructions.JMPCondition(
                FlagRegister::Condition::ParityEven);
            break;
        case 0xFA:
            skipIncrement = instructions.JMPCondition(
                FlagRegister::Condition::Minus);
            break;

        // JMP Opcodes
        case 0xC3:
        case 0xCB:
            instructions.JMP();
            skipIncrement = true;
            break;

        // OUT Opcode
        case 0xD3:
            instructions.OUT(getNextByte());
            break;

        // XTHL Opcode
        case 0xE3:
            instructions.XTHL();
            break;

        // DI Opcode
        case 0xF3:
            // instructions.DI();
            registerController.fetchNextInstruction();
            break;

        // Cccc Opcodes
        case 0xC4:
            skipIncrement = instructions.CALLCondition(FlagRegister::Condition::NotZero);
            break;
        case 0xD4:
            skipIncrement = instructions.CALLCondition(FlagRegister::Condition::NotCarry);
            break;
        case 0xE4:
            skipIncrement = instructions.CALLCondition(FlagRegister::Condition::ParityOdd);
            break;
        case 0xF4:
            skipIncrement = instructions.CALLCondition(FlagRegister::Condition::Positive);
            break;
        case 0xCC:
            skipIncrement = instructions.CALLCondition(FlagRegister::Condition::Zero);
            break;
        case 0xDC:
            skipIncrement = instructions.CALLCondition(FlagRegister::Condition::Carry);
            break;
        case 0xEC:
            skipIncrement = instructions.CALLCondition(FlagRegister::Condition::ParityEven);
            break;
        case 0xFC:
            skipIncrement = instructions.CALLCondition(FlagRegister::Condition::Minus);
            break;

        // PUSH Opcodes
        case 0xC5:
            instructions.PUSH(RegisterPair::B);
            break;
        case 0xD5:
            instructions.PUSH(RegisterPair::D);
            break;
        case 0xE5:
            instructions.PUSH(RegisterPair::H);
            break;
        case 0xF5:
            instructions.PUSH(RegisterPair::PSW);
            break;

        // ADI Opcode
        case 0xC6:
            instructions.ADI();
            break;

        // SUI Opcode
        case 0xD6:
            instructions.SUI();
            break;

        // ANI Opcode
        case 0xE6:
            instructions.ANI();
            break;

        // ORI Opcode
        case 0xF6:
            instructions.ORI();
            break;

            // RST Opcodes
        case 0xC7:
            instructions.RST(0);
            break;
        case 0xCF:
            instructions.RST(1);
            break;
        case 0xD7:
            instructions.RST(2);
            break;
        case 0xDF:
            instructions.RST(3);
            break;
        case 0xE7:
            instructions.RST(4);
            break;
        case 0xEF:
            instructions.RST(5);
            break;
        case 0xF7:
            instructions.RST(6);
            break;
        case 0xFF:
            instructions.RST(7);
            break;

            // RET Opcodes
        case 0xC9:
        case 0xD9:
            instructions.RET();
            skipIncrement = true;
            break;

        // PCHL Opcode
        case 0xE9:
            instructions.PCHL(registerController.getProgramCounter());
            skipIncrement = true;
            break;

        // SPHL Opcode
        case 0xF9:
            instructions.SPHL();
            break;

        // IN Opcode
        case 0xDB:
            instructions.IN(getNextByte());
            break;

        // XCHG Opcode
        case 0xEB:
            instructions.XCHG();
            break;

        // EI Opcode
        case 0xFB:
            // instructions.EI();
            registerController.fetchNextInstruction();
            break;

        // CALL Opcodes
        case 0xCD:
        case 0xDD:
        case 0xED:
        case 0xFD:
            instructions.CALL();
            skipIncrement = true;
            break;

        // ACI Opcode
        case 0xCE:
            instructions.ACI();
            break;

        // SBI Opcode
        case 0xDE:
            instructions.SBI();
            break;

        // XRI Opcode
        case 0xEE:
            instructions.XRI();
            break;

        // CPI Opcode
        case 0xFE:
            instructions.CPI();
            break;

        case 0x76:
            return instructions.HLT();
            break;

        default:
            std::cout << std::endl
                      << "Unknown Opcode " << std::hex << int(opcode) << std::endl;
            std::cout << "At line " << std::hex << int(registerController.getProgramCounter()) << std::endl;
            break;
    }

    // If the instructions does not need more than one cycle, just increment the
    // PC
    // if (!skipIncrement) {
    // registerController.getProgramCounter()++;
    // }
    registerController.incrementMachineCycle();

    return false;
}

// Get the next word from memory
// Also increment the PC
uint16_t InstructionDecoder::getNextWord() {
  int result = busController.readWord(registerController.getProgramCounter() + 1);
  registerController.getProgramCounter() += 2;
  return result;
}

// Get the next byte from memory
// Also increment the PC
uint8_t InstructionDecoder::getNextByte() {
  int result = busController.readByte(registerController.getProgramCounter() + 1);
  registerController.getProgramCounter()++;
  return result;
}