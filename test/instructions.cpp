#include "../src/instruction/instructions.h"
#include "../src/bus/device/ram.h"
#include <gtest/gtest.h>

class InstructionsTest : public ::testing::Test {

protected:
  BusController busController = BusController();
  RegisterController registerController = RegisterController(busController);
  IOController ioController = IOController();
  ALU alu = ALU(registerController);
  Instructions instructions =
      Instructions(busController, registerController, ioController, alu);
      Ram *ram = new Ram(0xFFFF);
  void SetUp() {
    busController.addChipRegion(0, 0xFFFF, ram);
    registerController.getProgramCounter() = 0;
    registerController.setMachineCycle(0);
    registerController.setRegisterPair(RegisterPair::Temporary, 0);
  }
  void TearDown() {
    delete ram;
    ram = new Ram(0xFFFF);
  }
};

TEST_F(InstructionsTest, MOV) {
  registerController.setRegister(Registers::A, 0x15);
  instructions.MOV(Registers::B, Registers::A);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::B), 0x15);
}

TEST_F(InstructionsTest, MVI) {
  registerController.setRegister(Registers::A, 0);
  busController.writeByte(1, 0x12);
  instructions.MVI(Registers::A);
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  instructions.MVI(Registers::A);
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 2);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x12);
}

// TODO LXI

TEST_F(InstructionsTest, LDA) {
  registerController.setRegister(Registers::A, 0);
  busController.writeByte(0x1, 0x00);
  busController.writeByte(0x2, 0x05);
  busController.writeByte(0x5, 0x15);
  instructions.LDA();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  instructions.LDA();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 2);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 2);
  instructions.LDA();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 5);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 3);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0);
  instructions.LDA();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 3);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x15);
}

TEST_F(InstructionsTest, STA) {
  registerController.setRegister(Registers::A, 66);
  busController.writeByte(1, 0);
  busController.writeByte(2, 0x10);
  instructions.STA();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(busController.readByte(0x10), 0);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  instructions.STA();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 2);
  GTEST_ASSERT_EQ(busController.readByte(0x10), 0);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 2);
  instructions.STA();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 0x10);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 3);
  GTEST_ASSERT_EQ(busController.readByte(0x10), 0);
  instructions.STA();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 3);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(busController.readByte(0x10), 66);
}

TEST_F(InstructionsTest, ADD) {
  registerController.setRegister(Registers::A, 0x12);
  registerController.setRegister(Registers::B, 0x12);
  instructions.ADD(Registers::B);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x24);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000110);
}

TEST_F(InstructionsTest, ADD_OVERFLOW) {
  registerController.setRegister(Registers::A, 0xFF);
  registerController.setRegister(Registers::B, 0x10);
  instructions.ADD(Registers::B);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0xF);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000111);
}

TEST_F(InstructionsTest, ADI) {
  registerController.setRegister(Registers::A, 0x12);
  busController.writeByte(1, 0x12);
  instructions.ADI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x12);

  instructions.ADI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 2);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x24);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000110);
}

TEST_F(InstructionsTest, ADI_OVERFLOW) {
  registerController.setRegister(Registers::A, 0xFF);
  busController.writeByte(1, 0x10);
  instructions.ADI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0xFF);

  instructions.ADI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 2);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0xF);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000111);
}

TEST_F(InstructionsTest, ADC_NO_CARRY) {
  registerController.setRegister(Registers::A, 0x1);
  registerController.setRegister(Registers::B, 0x1);
  instructions.ADC(Registers::B);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x2);
}

TEST_F(InstructionsTest, ADC_CARRY) {
  registerController.setRegister(Registers::A, 1);
  registerController.setRegister(Registers::B, 1);
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, true);
  instructions.ADC(Registers::B);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x3);
}

TEST_F(InstructionsTest, ACI_NO_CARRY) {
  registerController.setRegister(Registers::A, 1);
  busController.writeByte(1, 0x1);
  instructions.ACI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 1);

  instructions.ACI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 2);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 2);
}

TEST_F(InstructionsTest, ACI_CARRY) {
  registerController.setRegister(Registers::A, 1);
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, true);
  busController.writeByte(1, 0x1);
  instructions.ACI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 1);

  instructions.ACI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 2);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 3);
}

TEST_F(InstructionsTest, SUB) {
  registerController.setRegister(Registers::A, 5);
  registerController.setRegister(Registers::B, 1);
  instructions.SUB(Registers::B);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x4);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00010010);
}

TEST_F(InstructionsTest, SUB_UNDERFLOW) {
  registerController.setRegister(Registers::A, 1);
  registerController.setRegister(Registers::B, 5);
  instructions.SUB(Registers::B);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0xFC);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b10000111);
}

TEST_F(InstructionsTest, SUI) {
  registerController.setRegister(Registers::A, 5);
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, false);
  busController.writeByte(1, 0x1);
  instructions.SUI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 5);

  instructions.SUI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 2);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 4);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00010010);
}

TEST_F(InstructionsTest, SUI_UNDERFLOW) {
  registerController.setRegister(Registers::A, 1);
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, false);
  busController.writeByte(1, 0x5);
  instructions.SUI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 1);

  instructions.SUI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 2);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0xFC);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b10000111);
}

TEST_F(InstructionsTest, SBB_NO_CARRY) {
  registerController.setRegister(Registers::A, 5);
  registerController.setRegister(Registers::B, 1);
  instructions.SBB(Registers::B);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x4);
}

TEST_F(InstructionsTest, SBB_CARRY) {
  registerController.setRegister(Registers::A, 5);
  registerController.setRegister(Registers::B, 1);
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, true);
  instructions.SBB(Registers::B);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x3);
}

TEST_F(InstructionsTest, SBI_NO_CARRY) {
  registerController.setRegister(Registers::A, 5);
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, false);
  busController.writeByte(1, 0x1);
  instructions.SBI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 5);

  instructions.SBI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 2);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x4);
}

TEST_F(InstructionsTest, SBI_CARRY) {
  registerController.setRegister(Registers::A, 5);
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, true);
  busController.writeByte(1, 0x1);
  instructions.SBI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 5);

  instructions.SBI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 2);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x3);
}

TEST_F(InstructionsTest, INR) {
  registerController.setRegister(Registers::A, 1);
  instructions.INR(Registers::A);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x2);
}

TEST_F(InstructionsTest, INR_OVERFLOW) {
  registerController.setRegister(Registers::A, 0xFF);
  instructions.INR(Registers::A);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x0);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b01010110);
}

TEST_F(InstructionsTest, DCR) {
  registerController.setRegister(Registers::A, 1);
  instructions.DCR(Registers::A);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x0);
}

TEST_F(InstructionsTest, DCR1) {
  registerController.setRegister(Registers::B, 2);
  instructions.DCR(Registers::B);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::B), 0x1);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getFlag(
                      FlagRegister::AuxiliaryCarry),
                  1);
}

TEST_F(InstructionsTest, DCR_OVERFLOW) {
  registerController.setRegister(Registers::A, 0);
  instructions.DCR(Registers::A);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0xFF);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b10000110);
}

TEST_F(InstructionsTest, ANA) {
  registerController.setRegister(Registers::A, 5);
  registerController.setRegister(Registers::B, 1);
  instructions.ANA(Registers::B);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x01);
}

TEST_F(InstructionsTest, ANI) {
  registerController.setRegister(Registers::A, 5);
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, false);
  busController.writeByte(1, 0x1);
  instructions.ANI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 5);

  instructions.ANI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 2);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x1);
}

TEST_F(InstructionsTest, ORA) {
  registerController.setRegister(Registers::A, 1);
  registerController.setRegister(Registers::B, 2);
  instructions.ORA(Registers::B);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x03);
}

TEST_F(InstructionsTest, ORI) {
  registerController.setRegister(Registers::A, 1);
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, false);
  busController.writeByte(1, 0x2);
  instructions.ORI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 1);

  instructions.ORI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 2);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x03);
}

TEST_F(InstructionsTest, XRA) {
  registerController.setRegister(Registers::A, 2);
  registerController.setRegister(Registers::B, 5);
  instructions.XRA(Registers::B);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x07);
}

TEST_F(InstructionsTest, XRI) {
  registerController.setRegister(Registers::A, 2);
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, false);
  busController.writeByte(1, 0x5);
  instructions.XRI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 2);

  instructions.XRI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 2);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x07);
}

TEST_F(InstructionsTest, CMP) {
  registerController.setRegister(Registers::A, 0x12);
  instructions.CMP(Registers::A);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x12);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b01010110);
}

TEST_F(InstructionsTest, CPI) {
  registerController.setRegister(Registers::A, 0x12);
  busController.writeByte(1, 0x12);
  instructions.CPI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x12);

  instructions.CPI();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 2);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(registerController.getRegister(Registers::A), 0x12);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b01010110);
}

TEST_F(InstructionsTest, LHLD) {
  busController.writeWord(0x1, 0x1234);
  instructions.LHLD();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  GTEST_ASSERT_EQ(registerController.getRegisterPair(RegisterPair::H), 0);
  GTEST_ASSERT_EQ(registerController.getRegisterPair(RegisterPair::Temporary), 0x0034);

  instructions.LHLD();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 2);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 2);
  GTEST_ASSERT_EQ(registerController.getRegisterPair(RegisterPair::H), 0);
  GTEST_ASSERT_EQ(registerController.getRegisterPair(RegisterPair::Temporary), 0x1234);

  instructions.LHLD();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 3);
  GTEST_ASSERT_EQ(registerController.getRegisterPair(RegisterPair::Temporary), 0x0002);
  GTEST_ASSERT_EQ(registerController.getRegisterPair(RegisterPair::H), 0);
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 0x1234);

  instructions.LHLD();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 0x1235);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 4);
  GTEST_ASSERT_EQ(registerController.getRegisterPair(RegisterPair::Temporary), 0x0002);
  GTEST_ASSERT_EQ(registerController.getRegisterPair(RegisterPair::H), 0);

  instructions.LHLD();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 0x3);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(registerController.getRegisterPair(RegisterPair::Temporary), 0x1234);
  GTEST_ASSERT_EQ(registerController.getRegisterPair(RegisterPair::H), 0x1234);
}

TEST_F(InstructionsTest, SHLD) {
  busController.writeWord(0x1, 0x1234);
  registerController.setRegisterPair(RegisterPair::H, 0x4567);
  instructions.SHLD();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  GTEST_ASSERT_EQ(busController.readWord(0x1234), 0);
  GTEST_ASSERT_EQ(registerController.getRegisterPair(RegisterPair::Temporary), 0x0034);

  instructions.SHLD();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 2);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 2);
  GTEST_ASSERT_EQ(busController.readWord(0x1234), 0);
  GTEST_ASSERT_EQ(registerController.getRegisterPair(RegisterPair::Temporary), 0x1234);

  instructions.SHLD();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 3);
  GTEST_ASSERT_EQ(registerController.getRegisterPair(RegisterPair::Temporary), 0x0002);
  GTEST_ASSERT_EQ(busController.readWord(0x1234), 0);
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 0x1234);

  instructions.SHLD();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 0x1235);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 4);
  GTEST_ASSERT_EQ(registerController.getRegisterPair(RegisterPair::Temporary), 0x0002);
  GTEST_ASSERT_EQ(busController.readWord(0x1234), 0);

  instructions.SHLD();
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 0x3);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(registerController.getRegisterPair(RegisterPair::Temporary), 0x1234);
  GTEST_ASSERT_EQ(registerController.getRegisterPair(RegisterPair::H), 0x4567);
  GTEST_ASSERT_EQ(busController.readWord(0x1234), 0x4567);
}

TEST_F(InstructionsTest, LDAX) {
  registerController.setRegisterPair(RegisterPair::B, 0x7890);
  busController.writeByte(0x7890, 0x45);
  instructions.LDAX(RegisterPair::B);
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 0x7890);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  GTEST_ASSERT_EQ(0x00, registerController.getRegister(Registers::A));

  instructions.LDAX(RegisterPair::B);
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(0x45, registerController.getRegister(Registers::A));
}

TEST_F(InstructionsTest, STAX) {
  registerController.setRegisterPair(RegisterPair::B, 0x1111);
  registerController.setRegister(Registers::A, 0x22);
  instructions.STAX(RegisterPair::B);
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 0x1111);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 1);
  GTEST_ASSERT_EQ(0x00, busController.readByte(0x1111));

  instructions.STAX(RegisterPair::B);
  registerController.incrementMachineCycle();
  GTEST_ASSERT_EQ(registerController.getProgramCounter(), 1);
  GTEST_ASSERT_EQ(registerController.getMachineCycle(), 0);
  GTEST_ASSERT_EQ(0x22, busController.readByte(0x1111));
}

TEST_F(InstructionsTest, XCHG) {
  registerController.setRegisterPair(RegisterPair::D, 0x1234);
  registerController.setRegisterPair(RegisterPair::H, 0x5678);
  instructions.XCHG();
  GTEST_ASSERT_EQ(0x1234, registerController.getRegisterPair(RegisterPair::H));
  GTEST_ASSERT_EQ(0x5678, registerController.getRegisterPair(RegisterPair::D));
}

TEST_F(InstructionsTest, INX) {
  registerController.setRegisterPair(RegisterPair::B, 0x0002);
  instructions.INX(RegisterPair::B);
  GTEST_ASSERT_EQ(0x3, registerController.getRegisterPair(RegisterPair::B));
}

TEST_F(InstructionsTest, DCX) {
  registerController.setRegisterPair(RegisterPair::B, 0x0002);
  instructions.DCX(RegisterPair::B);
  GTEST_ASSERT_EQ(0x1, registerController.getRegisterPair(RegisterPair::B));
}

TEST_F(InstructionsTest, DAD) {
  registerController.setRegisterPair(RegisterPair::B, 0x1111);
  registerController.setRegisterPair(RegisterPair::H, 0x2222);
  instructions.DAD(RegisterPair::B);
  GTEST_ASSERT_EQ(0x3333, registerController.getRegisterPair(RegisterPair::H));
  GTEST_ASSERT_EQ(
      registerController.getFlagRegister().getFlag(FlagRegister::Flag::Carry),
      false);
}

TEST_F(InstructionsTest, DAD_OVERFLOW) {
  registerController.setRegisterPair(RegisterPair::B, 0x1111);
  registerController.setRegisterPair(RegisterPair::H, 0xFFFF);
  instructions.DAD(RegisterPair::B);
  GTEST_ASSERT_EQ(0x1110, registerController.getRegisterPair(RegisterPair::H));
  GTEST_ASSERT_EQ(
      registerController.getFlagRegister().getFlag(FlagRegister::Flag::Carry),
      true);
}

TEST_F(InstructionsTest, DAA_1) {
  registerController.setRegister(Registers::A, 0x61);
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, 1);
  instructions.DAA();
  GTEST_ASSERT_EQ(0xC1, registerController.getRegister(Registers::A));
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b10000011);
}

TEST_F(InstructionsTest, DAA_2) {
  registerController.setRegister(Registers::A, 0x2);
  instructions.DAA();
  GTEST_ASSERT_EQ(0x2, registerController.getRegister(Registers::A));
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000010);
}

TEST_F(InstructionsTest, DAA_3) {
  registerController.setRegister(Registers::A, 0xFF);
  instructions.DAA();
  GTEST_ASSERT_EQ(0x65, registerController.getRegister(Registers::A));
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00010111);
}

TEST_F(InstructionsTest, RLC) {
  registerController.setRegister(Registers::A, 0xC7);
  instructions.RLC();
  GTEST_ASSERT_EQ(0x8F, registerController.getRegister(Registers::A));
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000011);
}

TEST_F(InstructionsTest, RRC) {
  registerController.setRegister(Registers::A, 0xC7);
  instructions.RRC();
  GTEST_ASSERT_EQ(0xE3, registerController.getRegister(Registers::A));
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000011);
}

TEST_F(InstructionsTest, RAL) {
  registerController.setRegister(Registers::A, 0xC7);
  instructions.RAL();
  GTEST_ASSERT_EQ(0x8E, registerController.getRegister(Registers::A));
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000011);
}

TEST_F(InstructionsTest, RAR) {
  registerController.setRegister(Registers::A, 0xC7);
  instructions.RAR();
  GTEST_ASSERT_EQ(0x63, registerController.getRegister(Registers::A));
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000011);
}

TEST_F(InstructionsTest, CMA) {
  registerController.setRegister(Registers::A, 0xF0);
  instructions.CMA();
  GTEST_ASSERT_EQ(0x0F, registerController.getRegister(Registers::A));
}

TEST_F(InstructionsTest, CMC) {
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, 1);
  instructions.CMC();
  GTEST_ASSERT_EQ(false, registerController.getFlagRegister().getFlag(
                             FlagRegister::Flag::Carry));
}

TEST_F(InstructionsTest, STC) {
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, 0);
  instructions.STC();
  GTEST_ASSERT_EQ(true, registerController.getFlagRegister().getFlag(
                            FlagRegister::Flag::Carry));
}

TEST_F(InstructionsTest, PUSH) {
  registerController.setRegisterPair(RegisterPair::H, 0x1234);
  registerController.getStack().setStackPointer(0x3);
  instructions.PUSH(RegisterPair::H);
  GTEST_ASSERT_EQ(0x1234, registerController.getStack().popWord());
}

TEST_F(InstructionsTest, POP) {
  // registerController.getStack().setStackPointer(0x5); // TODO Support
  // wrapping
  registerController.getStack().pushWord(0x1234);
  instructions.POP(RegisterPair::H);
  GTEST_ASSERT_EQ(0x1234, registerController.getRegisterPair(RegisterPair::H));
}

TEST_F(InstructionsTest, XTHL) {
  registerController.getStack().pushWord(0x1234);
  registerController.setRegisterPair(RegisterPair::H, 0x5678);
  instructions.XTHL();
  GTEST_ASSERT_EQ(0x1234, registerController.getRegisterPair(RegisterPair::H));
  GTEST_ASSERT_EQ(0x5678, registerController.getStack().popWord());
}

TEST_F(InstructionsTest, SPHL) {
  registerController.setRegisterPair(RegisterPair::H, 0x1234);
  instructions.SPHL();
  GTEST_ASSERT_EQ(0x1234, registerController.getStack().getStackPointer());
}