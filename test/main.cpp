#include "bus/device/ramtest.cpp"
#include "bus/buscontollertest.cpp"
#include <gtest/gtest.h>

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}