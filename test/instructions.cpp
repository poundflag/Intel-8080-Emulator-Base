#include "../src/instructions.h"
#include <gtest/gtest.h>

class InstructionsTest : public ::testing::Test {

protected:
  BusController busController = BusController();
  RegisterController registerController = RegisterController(busController);
  Instructions instructions = Instructions(busController, registerController);
  Register lA;
  void SetUp() {
    busController.addChipRegion(ChipRegion(0, 10, new Ram(10)));
    lA = Register();
  }
};

TEST_F(InstructionsTest, MOV) {
  registerController.get(Registers::A).setRegister(0x15);
  instructions.MOV(Registers::B, Registers::A);
  GTEST_ASSERT_EQ(registerController.get(Registers::B).getRegister(), 0x15);
}

TEST_F(InstructionsTest, MVI) {
  registerController.get(Registers::A).setRegister(0x0);
  instructions.MVI(Registers::A, 0x12);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x12);
}

TEST_F(InstructionsTest, LDA) {
  busController.writeByte(0x2, 0x15);
  instructions.LDA(0x2);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x15);
}

TEST_F(InstructionsTest, STA) {
  registerController.get(Registers::A).setRegister(66);
  instructions.STA(0x2);
  GTEST_ASSERT_EQ(busController.readByte(0x2), 66);
}

TEST_F(InstructionsTest, ADD) {
  registerController.get(Registers::A).setRegister(0x12);
  registerController.get(Registers::B).setRegister(0x12);
  instructions.ADD(Registers::B);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x24);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000110);
}

TEST_F(InstructionsTest, ADD_OVERFLOW) {
  registerController.get(Registers::A).setRegister(0xFF);
  registerController.get(Registers::B).setRegister(0x10);
  instructions.ADD(Registers::B);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0xF);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000111);
}

TEST_F(InstructionsTest, ADI) {
  registerController.get(Registers::A).setRegister(0x12);
  instructions.ADI(0x12);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x24);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000110);
}

TEST_F(InstructionsTest, ADI_OVERFLOW) {
  registerController.get(Registers::A).setRegister(0xFF);
  instructions.ADI(0x10);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0xF);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000111);
}

TEST_F(InstructionsTest, ADC_NO_CARRY) {
  registerController.get(Registers::A).setRegister(0x1);
  registerController.get(Registers::B).setRegister(0x1);
  instructions.ADC(Registers::B);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x2);
}

TEST_F(InstructionsTest, ADC_CARRY) {
  registerController.get(Registers::A).setRegister(0x1);
  registerController.get(Registers::B).setRegister(0x1);
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, true);
  instructions.ADC(Registers::B);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x3);
}

TEST_F(InstructionsTest, ACI_NO_CARRY) {
  registerController.get(Registers::A).setRegister(0x1);
  instructions.ACI(0x1);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x2);
}

TEST_F(InstructionsTest, ACI_CARRY) {
  registerController.get(Registers::A).setRegister(0x1);
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, true);
  instructions.ACI(0x1);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x3);
}

TEST_F(InstructionsTest, SUB) {
  registerController.get(Registers::A).setRegister(0x5);
  registerController.get(Registers::B).setRegister(0x1);
  instructions.SUB(Registers::B);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x4);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000010);
}

TEST_F(InstructionsTest, SUB_UNDERFLOW) {
  registerController.get(Registers::A).setRegister(0x1);
  registerController.get(Registers::B).setRegister(0x5);
  instructions.SUB(Registers::B);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0xFC);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b10010111);
}

TEST_F(InstructionsTest, SUI) {
  registerController.get(Registers::A).setRegister(0x5);
  instructions.SUI(0x1);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x4);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000010);
}

TEST_F(InstructionsTest, SUI_UNDERFLOW) {
  registerController.get(Registers::A).setRegister(0x1);
  instructions.SUI(0x5);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0xFC);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b10010111);
}

TEST_F(InstructionsTest, SBB_NO_CARRY) {
  registerController.get(Registers::A).setRegister(0x5);
  registerController.get(Registers::B).setRegister(0x1);
  instructions.SBB(Registers::B);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x4);
}

TEST_F(InstructionsTest, SBB_CARRY) {
  registerController.get(Registers::A).setRegister(0x5);
  registerController.get(Registers::B).setRegister(0x1);
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, true);
  instructions.SBB(Registers::B);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x3);
}

TEST_F(InstructionsTest, SBI_NO_CARRY) {
  registerController.get(Registers::A).setRegister(0x5);
  instructions.SBI(0x1);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x4);
}

TEST_F(InstructionsTest, SBI_CARRY) {
  registerController.get(Registers::A).setRegister(0x5);
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, true);
  instructions.SBI(0x1);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x3);
}

TEST_F(InstructionsTest, INR) {
  registerController.get(Registers::A).setRegister(0x1);
  instructions.INR(Registers::A);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x2);
}

TEST_F(InstructionsTest, INR_OVERFLOW) {
  registerController.get(Registers::A).setRegister(0xFF);
  instructions.INR(Registers::A);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x0);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b01010110);
}

TEST_F(InstructionsTest, DCR) {
  registerController.get(Registers::A).setRegister(0x1);
  instructions.DCR(Registers::A);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x0);
}

TEST_F(InstructionsTest, DCR_OVERFLOW) {
  registerController.get(Registers::A).setRegister(0x0);
  instructions.DCR(Registers::A);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0xFF);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b10010110);
}

TEST_F(InstructionsTest, ANA) {
  registerController.get(Registers::A).setRegister(0x5);
  registerController.get(Registers::B).setRegister(0x1);
  instructions.ANA(Registers::B);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x01);
}

TEST_F(InstructionsTest, ANI) {
  registerController.get(Registers::A).setRegister(0x5);
  instructions.ANI(1);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x01);
}

TEST_F(InstructionsTest, ORA) {
  registerController.get(Registers::A).setRegister(0x1);
  registerController.get(Registers::B).setRegister(0x2);
  instructions.ORA(Registers::B);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x03);
}

TEST_F(InstructionsTest, ORI) {
  registerController.get(Registers::A).setRegister(0x1);
  instructions.ORI(0x2);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x03);
}

TEST_F(InstructionsTest, XRA) {
  registerController.get(Registers::A).setRegister(0x2);
  registerController.get(Registers::B).setRegister(0x5);
  instructions.XRA(Registers::B);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x07);
}

TEST_F(InstructionsTest, XRI) {
  registerController.get(Registers::A).setRegister(0x2);
  instructions.ORI(0x5);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x07);
}

TEST_F(InstructionsTest, CMP) {
  registerController.get(Registers::A).setRegister(0x12);
  instructions.CMP(Registers::A);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x12);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b01000110);
}

TEST_F(InstructionsTest, CPI) {
  registerController.get(Registers::A).setRegister(0x12);
  instructions.CPI(0x12);
  GTEST_ASSERT_EQ(registerController.get(Registers::A).getRegister(), 0x12);
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b01000110);
}

TEST_F(InstructionsTest, LHLD) {
  busController.writeWord(0x2, 0x1234);
  instructions.LHLD(0x2);
  GTEST_ASSERT_EQ(0x1234, registerController.getRegisterPair(RegisterPair::H));
}

TEST_F(InstructionsTest, SHLD) {
  registerController.setRegisterPair(RegisterPair::H, 0x1234);
  instructions.SHLD(0x2);
  GTEST_ASSERT_EQ(0x1234, busController.readWord(0x2));
}

TEST_F(InstructionsTest, LDAX) {
  registerController.setRegisterPair(RegisterPair::B, 0x0002);
  busController.writeByte(0x2, 0x60);
  instructions.LDAX(RegisterPair::B);
  GTEST_ASSERT_EQ(0x60, registerController.get(Registers::A).getRegister());
}

TEST_F(InstructionsTest, STAX) {
  registerController.setRegisterPair(RegisterPair::B, 0x0002);
  registerController.get(Registers::A).setRegister(0x33);
  instructions.STAX(RegisterPair::B);
  GTEST_ASSERT_EQ(0x33, busController.readByte(2));
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
  registerController.get(Registers::A).setRegister(0x61);
  registerController.getFlagRegister().setFlag(FlagRegister::Flag::Carry, 1);
  instructions.DAA();
  GTEST_ASSERT_EQ(0xC1, registerController.get(Registers::A).getRegister());
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b10000010);
}

TEST_F(InstructionsTest, DAA_2) {
  registerController.get(Registers::A).setRegister(0x2);
  instructions.DAA();
  GTEST_ASSERT_EQ(0x2, registerController.get(Registers::A).getRegister());
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000010);
}

TEST_F(InstructionsTest, DAA_3) {
  registerController.get(Registers::A).setRegister(0xFF);
  instructions.DAA();
  GTEST_ASSERT_EQ(0x65, registerController.get(Registers::A).getRegister());
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00010111);
}

TEST_F(InstructionsTest, RLC) {
  registerController.get(Registers::A).setRegister(0xC7);
  instructions.RLC();
  GTEST_ASSERT_EQ(0x8F, registerController.get(Registers::A).getRegister());
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000011);
}

TEST_F(InstructionsTest, RRC) {
  registerController.get(Registers::A).setRegister(0xC7);
  instructions.RRC();
  GTEST_ASSERT_EQ(0xE3, registerController.get(Registers::A).getRegister());
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000011);
}

TEST_F(InstructionsTest, RAL) {
  registerController.get(Registers::A).setRegister(0xC7);
  instructions.RAL();
  GTEST_ASSERT_EQ(0x8E, registerController.get(Registers::A).getRegister());
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000011);
}

TEST_F(InstructionsTest, RAR) {
  registerController.get(Registers::A).setRegister(0xC7);
  instructions.RAR();
  GTEST_ASSERT_EQ(0x63, registerController.get(Registers::A).getRegister());
  GTEST_ASSERT_EQ(registerController.getFlagRegister().getRegister(),
                  0b00000011);
}

TEST_F(InstructionsTest, CMA) {
  registerController.get(Registers::A).setRegister(0xF0);
  instructions.CMA();
  GTEST_ASSERT_EQ(0x0F, registerController.get(Registers::A).getRegister());
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
  // registerController.getStack().setStackPointer(0x5); // TODO Support wrapping
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