#include "../../../src/registers/model/flagregister.h"
#include <gtest/gtest.h>

class FlagRegisterTest : public ::testing::Test {

protected:
  FlagRegister flagRegister;
  void SetUp() { flagRegister = FlagRegister(); }

  void TearDown() {}
};

TEST_F(FlagRegisterTest, getInitialValue) {
  GTEST_ASSERT_EQ(0b00000010, flagRegister.getRegister());
}

TEST_F(FlagRegisterTest, determineCarryPlus) {
  flagRegister.processFlags(FlagRegister::FlagRule::CarryOnly, 0xFF, 0x1, "+");
  GTEST_ASSERT_EQ(1, flagRegister.getFlag(FlagRegister::Carry));
}

TEST_F(FlagRegisterTest, determineCarryMinus) {
  flagRegister.processFlags(FlagRegister::FlagRule::CarryOnly, 0x1, 0xFF, "-");
  GTEST_ASSERT_EQ(1, flagRegister.getFlag(FlagRegister::Carry));
}

TEST_F(FlagRegisterTest, determineCarryMinus2) {
  flagRegister.processFlags(FlagRegister::FlagRule::CarryOnly, 0x1, 0x5, "-");
  GTEST_ASSERT_EQ(1, flagRegister.getFlag(FlagRegister::Carry));
}

TEST_F(FlagRegisterTest, determineCarryOff) {
  flagRegister.processFlags(FlagRegister::FlagRule::CarryOnly, 0x2, 0x1, "-");
  GTEST_ASSERT_EQ(0, flagRegister.getFlag(FlagRegister::Carry));
}

TEST_F(FlagRegisterTest, determineParity) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0b11110000, 0x0, "+");
  GTEST_ASSERT_EQ(1, flagRegister.getFlag(FlagRegister::Parity));
}

TEST_F(FlagRegisterTest, determineParity_1) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0x99, 0x0, "+");
  GTEST_ASSERT_EQ(1, flagRegister.getFlag(FlagRegister::Parity));
}

TEST_F(FlagRegisterTest, determineNoParity) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0b11110010, 0x0, "+");
  GTEST_ASSERT_EQ(0, flagRegister.getFlag(FlagRegister::Parity));
}

TEST_F(FlagRegisterTest, determineAuxiliaryCarry) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0xF, 1, "+");
  GTEST_ASSERT_EQ(1, flagRegister.getFlag(FlagRegister::AuxiliaryCarry));
}

TEST_F(FlagRegisterTest, determineNoAuxiliaryCarry) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0x7, 1, "+");
  GTEST_ASSERT_EQ(0, flagRegister.getFlag(FlagRegister::AuxiliaryCarry));
}

TEST_F(FlagRegisterTest, determineZero) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 1, 1, "-");
  GTEST_ASSERT_EQ(1, flagRegister.getFlag(FlagRegister::Zero));
}

TEST_F(FlagRegisterTest, determineSigned) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0b10000000, 0, "+");
  GTEST_ASSERT_EQ(1, flagRegister.getFlag(FlagRegister::Signed));
}

TEST_F(FlagRegisterTest, flagregisterAddTest) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0xC9, 0x10, "+");
  GTEST_ASSERT_EQ(0b10000010, flagRegister.getRegister());
}

TEST_F(FlagRegisterTest, flagregisterAddTest1) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0xA1, 0xC9, "+");
  GTEST_ASSERT_EQ(0b00000111, flagRegister.getRegister());
}

TEST_F(FlagRegisterTest, flagregisterAddTest2) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0xDE, 0xFE, "+");
  GTEST_ASSERT_EQ(0b10010011, flagRegister.getRegister());
}

TEST_F(FlagRegisterTest, flagregisterAddTest3) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0xC9, 0x10, "+");
  GTEST_ASSERT_EQ(0b10000010, flagRegister.getRegister());
}

TEST_F(FlagRegisterTest, flagregisterAddTest4) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0x20, 0x30, "+");
  GTEST_ASSERT_EQ(0b00000110, flagRegister.getRegister());
}

TEST_F(FlagRegisterTest, flagregisterAddTest5) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0x80, 0x41, "+");
  GTEST_ASSERT_EQ(0b10000010, flagRegister.getRegister());
}

TEST_F(FlagRegisterTest, flagregisterAddTest6) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0x01, 0xD2, "+");
  GTEST_ASSERT_EQ(0b10000010, flagRegister.getRegister());
}

TEST_F(FlagRegisterTest, flagregisterMinusTest) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0xDE, 0xFE, "-");
  GTEST_ASSERT_EQ(0b10000011, flagRegister.getRegister());
}

TEST_F(FlagRegisterTest, flagregisterMinusTest1) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0x23, 0x45, "-");
  GTEST_ASSERT_EQ(0b10000111, flagRegister.getRegister());
}

TEST_F(FlagRegisterTest, flagregisterMinusTest2) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0x00, 0xAA, "-");
  GTEST_ASSERT_EQ(0b00000111, flagRegister.getRegister());
}

TEST_F(FlagRegisterTest, flagregisterMinusTest3) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0x21, 0xDE, "-");
  GTEST_ASSERT_EQ(0b00000011, flagRegister.getRegister());
}

TEST_F(FlagRegisterTest, flagregisterMinusTest4) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0x1, 0x20, "-");
  GTEST_ASSERT_EQ(0b10010111, flagRegister.getRegister());
}

TEST_F(FlagRegisterTest, flagregisterMinusTest5) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0x23, 0xFE, "-");
  GTEST_ASSERT_EQ(0b00000011, flagRegister.getRegister());
}

TEST_F(FlagRegisterTest, flagregisterMinusTest6) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0x11, 0x11, "-");
  GTEST_ASSERT_EQ(0b01010110, flagRegister.getRegister());
}