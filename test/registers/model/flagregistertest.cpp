#include "../../../src/registers/model/flagregister.h"
#include "../../../src/registers/model/flagenum.h"
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
  flagRegister.processFlags(FlagRule::CarryOnly, 0xFF, 0x1, "+");
  GTEST_ASSERT_EQ(1, flagRegister.getFlag(Flag::Carry));
}

TEST_F(FlagRegisterTest, determineCarryMinus) {
  flagRegister.processFlags(FlagRule::CarryOnly, 0x1, 0xFF, "-");
  GTEST_ASSERT_EQ(1, flagRegister.getFlag(Flag::Carry));
}

TEST_F(FlagRegisterTest, determineCarryOff) {
  flagRegister.processFlags(FlagRule::CarryOnly, 0x2, 0x1, "-");
  GTEST_ASSERT_EQ(0, flagRegister.getFlag(Flag::Carry));
}

TEST_F(FlagRegisterTest, determineParity) {
  flagRegister.processFlags(FlagRule::All, 0b11110000, 0x0, "+");
  GTEST_ASSERT_EQ(1, flagRegister.getFlag(Flag::Parity));
}

TEST_F(FlagRegisterTest, determineAuxiliaryCarry) { GTEST_ASSERT_EQ(1, 1); }

TEST_F(FlagRegisterTest, determineZero) {
  flagRegister.processFlags(FlagRule::All, 1, 1, "-");
  GTEST_ASSERT_EQ(1, flagRegister.getFlag(Flag::Zero));
}

TEST_F(FlagRegisterTest, determineSigned) {
  flagRegister.processFlags(FlagRule::All, 0b10000000, 0, "+");
  GTEST_ASSERT_EQ(1, flagRegister.getFlag(Flag::Signed));
}