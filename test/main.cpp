#include "bus/device/ramtest.cpp"
#include "bus/buscontrollertest.cpp"
#include "registers/model/flagregistertest.cpp"
#include "../src/registers/model/flagregister.h"
#include <gtest/gtest.h>

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}