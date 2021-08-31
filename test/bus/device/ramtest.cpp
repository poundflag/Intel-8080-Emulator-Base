#include "../../../src/bus/device/ram.h"
#include <gtest/gtest.h>

class RamTest : public ::testing::Test {

protected:
  Ram *ramDevice;
  void SetUp() { ramDevice = new Ram(0x10); }

  void TearDown() { delete ramDevice; } 
};

TEST_F(RamTest, readEmptyRam) {
  int value = 0;
  for (int i = 0; i < 0x10; i++) {
    value += ramDevice->read(i);
  }

  GTEST_ASSERT_EQ(value, 0);
}

TEST_F(RamTest, writeAndReadToRam) {
  int value = 0x20;
  ramDevice->write(0xA, value);
  GTEST_ASSERT_EQ(value, ramDevice->read(0xA));
}