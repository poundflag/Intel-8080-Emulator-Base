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

TEST_F(FlagRegisterTest, determineCarryOff) {
  flagRegister.processFlags(FlagRegister::FlagRule::CarryOnly, 0x2, 0x1, "-");
  GTEST_ASSERT_EQ(0, flagRegister.getFlag(FlagRegister::Carry));
}

TEST_F(FlagRegisterTest, determineParity) {
  flagRegister.processFlags(FlagRegister::FlagRule::All, 0b11110000, 0x0, "+");
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