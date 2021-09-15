#include "../src/instructions.h"
#include <gtest/gtest.h>

class InstructionsTest : public ::testing::Test {

protected:
  std::shared_ptr<BusController> busController =
      std::make_shared<BusController>();
  std::shared_ptr<RegisterController> registerController =
      std::make_shared<RegisterController>(busController);
  Instructions instructions = Instructions(busController, registerController);
  Register lA;
  void SetUp() {
    busController->addChipRegion(ChipRegion(0, 10, new Ram(10)));
    lA = Register();
  }
};

TEST_F(InstructionsTest, MOV) {
  lA.setRegister(0x15);
  Register lB = Register();
  instructions.MOV(lB, lA);
  GTEST_ASSERT_EQ(lB.getRegister(), 0x15);
}

TEST_F(InstructionsTest, MVI) {
  instructions.MVI(lA, 0x12);
  GTEST_ASSERT_EQ(lA.getRegister(), 0x12);
}

TEST_F(InstructionsTest, LDA) {
  busController->writeByte(0x2, 0x15);
  instructions.LDA(0x2);
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x15);
}

TEST_F(InstructionsTest, STA) {
  registerController->get(Registers::A).setRegister(66);
  instructions.STA(0x2);
  GTEST_ASSERT_EQ(busController->readByte(0x2), 66);
}

TEST_F(InstructionsTest, ADD) {
  registerController->get(Registers::A).setRegister(0x12);
  registerController->get(Registers::B).setRegister(0x12);
  instructions.ADD(registerController->get(Registers::B));
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x24);
  GTEST_ASSERT_EQ(registerController->getFlagRegister().getRegister(),
                  0b00000110);
}

TEST_F(InstructionsTest, ADD_OVERFLOW) {
  registerController->get(Registers::A).setRegister(0xFF);
  registerController->get(Registers::B).setRegister(0x10);
  instructions.ADD(registerController->get(Registers::B));
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0xF);
  GTEST_ASSERT_EQ(registerController->getFlagRegister().getRegister(),
                  0b00000111);
}

TEST_F(InstructionsTest, ADI) {
  registerController->get(Registers::A).setRegister(0x12);
  instructions.ADI(0x12);
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x24);
  GTEST_ASSERT_EQ(registerController->getFlagRegister().getRegister(),
                  0b00000110);
}

TEST_F(InstructionsTest, ADI_OVERFLOW) {
  registerController->get(Registers::A).setRegister(0xFF);
  instructions.ADI(0x10);
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0xF);
  GTEST_ASSERT_EQ(registerController->getFlagRegister().getRegister(),
                  0b00000111);
}

TEST_F(InstructionsTest, ADC_NO_CARRY) {
  registerController->get(Registers::A).setRegister(0x1);
  registerController->get(Registers::B).setRegister(0x1);
  instructions.ADC(registerController->get(Registers::B));
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x2);
}

TEST_F(InstructionsTest, ADC_CARRY) {
  registerController->get(Registers::A).setRegister(0x1);
  registerController->get(Registers::B).setRegister(0x1);
  registerController->getFlagRegister().setFlag(FlagRegister::Flag::Carry,
                                                true);
  instructions.ADC(registerController->get(Registers::B));
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x3);
}

TEST_F(InstructionsTest, ACI_NO_CARRY) {
  registerController->get(Registers::A).setRegister(0x1);
  instructions.ACI(0x1);
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x2);
}

TEST_F(InstructionsTest, ACI_CARRY) {
  registerController->get(Registers::A).setRegister(0x1);
  registerController->getFlagRegister().setFlag(FlagRegister::Flag::Carry,
                                                true);
  instructions.ACI(0x1);
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x3);
}

TEST_F(InstructionsTest, SUB) {
  registerController->get(Registers::A).setRegister(0x5);
  registerController->get(Registers::B).setRegister(0x1);
  instructions.SUB(registerController->get(Registers::B));
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x4);
  GTEST_ASSERT_EQ(registerController->getFlagRegister().getRegister(),
                  0b00000010);
}

TEST_F(InstructionsTest, SUB_UNDERFLOW) {
  registerController->get(Registers::A).setRegister(0x1);
  registerController->get(Registers::B).setRegister(0x5);
  instructions.SUB(registerController->get(Registers::B));
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0xFC);
  GTEST_ASSERT_EQ(registerController->getFlagRegister().getRegister(),
                  0b10010111);
}

TEST_F(InstructionsTest, SUI) {
  registerController->get(Registers::A).setRegister(0x5);
  instructions.SUI(0x1);
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x4);
  GTEST_ASSERT_EQ(registerController->getFlagRegister().getRegister(),
                  0b00000010);
}

TEST_F(InstructionsTest, SUI_UNDERFLOW) {
  registerController->get(Registers::A).setRegister(0x1);
  instructions.SUI(0x5);
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0xFC);
  GTEST_ASSERT_EQ(registerController->getFlagRegister().getRegister(),
                  0b10010111);
}

TEST_F(InstructionsTest, SBB_NO_CARRY) {
  registerController->get(Registers::A).setRegister(0x5);
  registerController->get(Registers::B).setRegister(0x1);
  instructions.SBB(registerController->get(Registers::B));
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x4);
}

TEST_F(InstructionsTest, SBB_CARRY) {
  registerController->get(Registers::A).setRegister(0x5);
  registerController->get(Registers::B).setRegister(0x1);
  registerController->getFlagRegister().setFlag(FlagRegister::Flag::Carry,
                                                true);
  instructions.SBB(registerController->get(Registers::B));
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x3);
}

TEST_F(InstructionsTest, SBI_NO_CARRY) {
  registerController->get(Registers::A).setRegister(0x5);
  instructions.SBI(0x1);
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x4);
}

TEST_F(InstructionsTest, SBI_CARRY) {
  registerController->get(Registers::A).setRegister(0x5);
  registerController->getFlagRegister().setFlag(FlagRegister::Flag::Carry,
                                                true);
  instructions.SBI(0x1);
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x3);
}

TEST_F(InstructionsTest, INR) {
  registerController->get(Registers::A).setRegister(0x1);
  instructions.INR(registerController->get(Registers::A));
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x2);
}

TEST_F(InstructionsTest, INR_OVERFLOW) {
  registerController->get(Registers::A).setRegister(0xFF);
  instructions.INR(registerController->get(Registers::A));
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x0);
  GTEST_ASSERT_EQ(registerController->getFlagRegister().getRegister(),
                  0b01010110);
}

TEST_F(InstructionsTest, DCR) {
  registerController->get(Registers::A).setRegister(0x1);
  instructions.DCR(registerController->get(Registers::A));
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x0);
}

TEST_F(InstructionsTest, DCR_OVERFLOW) {
  registerController->get(Registers::A).setRegister(0x0);
  instructions.DCR(registerController->get(Registers::A));
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0xFF);
  GTEST_ASSERT_EQ(registerController->getFlagRegister().getRegister(),
                  0b10010110);
}

TEST_F(InstructionsTest, ANA) {
  registerController->get(Registers::A).setRegister(0x5);
  registerController->get(Registers::B).setRegister(0x1);
  instructions.ANA(registerController->get(Registers::B));
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x01);
}

TEST_F(InstructionsTest, ANI) {
  registerController->get(Registers::A).setRegister(0x5);
  instructions.ANI(1);
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x01);
}

TEST_F(InstructionsTest, ORA) {
  registerController->get(Registers::A).setRegister(0x1);
  registerController->get(Registers::B).setRegister(0x2);
  instructions.ORA(registerController->get(Registers::B));
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x03);
}

TEST_F(InstructionsTest, ORI) {
  registerController->get(Registers::A).setRegister(0x1);
  instructions.ORI(0x2);
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x03);
}

TEST_F(InstructionsTest, XRA) {
  registerController->get(Registers::A).setRegister(0x2);
  registerController->get(Registers::B).setRegister(0x5);
  instructions.XRA(registerController->get(Registers::B));
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x07);
}

TEST_F(InstructionsTest, XRI) {
  registerController->get(Registers::A).setRegister(0x2);
  instructions.ORI(0x5);
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x07);
}

TEST_F(InstructionsTest, CMP) {
  registerController->get(Registers::A).setRegister(0x12);
  instructions.CMP(registerController->get(Registers::A));
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x12);
  GTEST_ASSERT_EQ(registerController->getFlagRegister().getRegister(),
                  0b01000110);
}

TEST_F(InstructionsTest, CPI) {
  registerController->get(Registers::A).setRegister(0x12);
  instructions.CPI(0x12);
  GTEST_ASSERT_EQ(registerController->get(Registers::A).getRegister(), 0x12);
  GTEST_ASSERT_EQ(registerController->getFlagRegister().getRegister(),
                  0b01000110);
}