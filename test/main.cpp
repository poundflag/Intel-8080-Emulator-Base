#include <gtest/gtest.h>
/*#include "bus/device/ramtest.cpp"
#include "bus/buscontrollertest.cpp"
#include "registers/model/flagregistertest.cpp"
#include "registers/model/stacktest.cpp"
#include "registers/registercontroller.cpp"
#include "instructions.cpp"
#include "integration.cpp"*/

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}