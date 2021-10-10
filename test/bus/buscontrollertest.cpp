#include "../../src/bus/buscontroller.h"
#include "../../src/bus/device/ram.h"
#include <gtest/gtest.h>

class BusControllerTest : public ::testing::Test {
protected:
  BusController *busController;
  void SetUp() {
    busController = new BusController();

    ChipRegion aa = ChipRegion(0, 10, new Ram(10));
    busController->addChipRegion(aa);
  }

  void TearDown() {
    if (busController == NULL) {
      delete busController;
    }
  }
};

TEST_F(BusControllerTest, writeAndReadByte) {
  busController->writeByte(1, 0x25);
  GTEST_ASSERT_EQ(0x25, busController->readByte(1));
}

TEST_F(BusControllerTest, writeAndReadOutOfSpace) {
  busController->writeByte(0x1FF, 0x25);
  GTEST_ASSERT_EQ(0x0, busController->readByte(0));
}

TEST_F(BusControllerTest, writeAndReadWord) {
  busController->writeWord(0x5, 0x1234);
  GTEST_ASSERT_EQ(0x1234, busController->readWord(0x5));
  GTEST_ASSERT_EQ(0x34, busController->readByte(0x5));
  GTEST_ASSERT_EQ(0x12, busController->readByte(0x6));
}