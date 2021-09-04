#include "../../../src/registers/model/stack.h"
#include <gtest/gtest.h>

class StackTest : public ::testing::Test {

protected:
  std::shared_ptr<BusController> busController =
      std::make_shared<BusController>();
  Stack stack = Stack(busController);
  void SetUp() { busController->addChipRegion(ChipRegion(0, 10, new Ram(10))); }
};

TEST_F(StackTest, pushAndPopByte) {
  stack.pushByte(0x15);
  GTEST_ASSERT_EQ(0x15, stack.popByte());
}

TEST_F(StackTest, pushAndCheckByteMemoryAddress) {
  stack.setRegister(0x10);
  stack.pushByte(0x15);
  GTEST_ASSERT_EQ(0x15, busController->readByte(0x0F));
  stack.popByte();
  GTEST_ASSERT_EQ(0x0, busController->readByte(0x0F));
}

TEST_F(StackTest, pushAndPopWord) {
  stack.pushWord(0x1234);
  GTEST_ASSERT_EQ(0x1234, stack.popWord());
}

TEST_F(StackTest, pushAndCheckWordMemoryAddress) {
  stack.setRegister(0x10);
  stack.pushWord(0x1234);
  GTEST_ASSERT_EQ(0x1234, busController->readWord(0x0E));
  stack.popWord();
  GTEST_ASSERT_EQ(0x0, busController->readWord(0x0E));
}