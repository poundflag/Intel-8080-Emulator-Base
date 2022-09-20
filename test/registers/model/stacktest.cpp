#include "../../../src/registers/model/stack.h"
#include <gtest/gtest.h>

class StackTest : public ::testing::Test {

protected:
  BusController busController = BusController();
  Stack stack = Stack(busController);
  void SetUp() { busController.addChipRegion(0, 10, new Ram(10)); }
};

TEST_F(StackTest, pushAndPopByte) {
  stack.setStackPointer(0x5);
  stack.pushByte(0x15);
  GTEST_ASSERT_EQ(0x15, stack.popByte());
}

TEST_F(StackTest, pushAndCheckByteMemoryAddress) {
  stack.setStackPointer(0x8);
  stack.pushByte(0x15);
  GTEST_ASSERT_EQ(0x15, busController.readByte(0x07));
  stack.popByte();
  GTEST_ASSERT_EQ(0x0, busController.readByte(0x07));
}

TEST_F(StackTest, pushAndPopWord) {
  stack.setStackPointer(0x5);
  stack.pushWord(0x1234);
  GTEST_ASSERT_EQ(0x1234, stack.popWord());
}

TEST_F(StackTest, pushAndCheckWordMemoryAddress) {
  stack.setStackPointer(0x5);
  stack.pushWord(0x1234);
  GTEST_ASSERT_EQ(0x1234, busController.readWord(0x03));
  stack.popWord();
  GTEST_ASSERT_EQ(0x1234, busController.readWord(0x03));
}