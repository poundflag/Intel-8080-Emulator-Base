#include "../../src/registers/registercontroller.h"
#include "../../src/registers/registers.h"
#include "../../src/bus/device/ram.h"
#include <gtest/gtest.h>

class RegisterControllerTest : public ::testing::Test {

protected:
  BusController busController = BusController();
  RegisterController registerController = RegisterController(busController);
  void SetUp() { busController.addChipRegion(0, 10, new Ram(10)); }
};

TEST_F(RegisterControllerTest, getRegister) {
  registerController.setRegister(Registers::A, 0x15);

  GTEST_ASSERT_EQ(0x15, registerController.getRegister(Registers::A));
}

TEST_F(RegisterControllerTest, getMemoryReference) {
  registerController.setRegisterPair(RegisterPair::H, 0x0005);
  // busController.writeByte(0x0005, 0x12);
  registerController.setRegister(Registers::MemoryReference, 0x15);

  GTEST_ASSERT_EQ(0x15, registerController.getRegister(Registers::MemoryReference));
  GTEST_ASSERT_EQ(0x15,
                  busController.readByte(
                      registerController.getRegisterPair(RegisterPair::H)));
}

TEST_F(RegisterControllerTest, getRegisterPairWithPairSet) {
  registerController.setRegisterPair(RegisterPair::B, 0x1234);

  GTEST_ASSERT_EQ(0x1234, registerController.getRegisterPair(RegisterPair::B));
}

TEST_F(RegisterControllerTest, getRegisterPairWithSingleSet) {
  registerController.setRegister(Registers::B, 0x12);
  registerController.setRegister(Registers::C, 0x34);

  GTEST_ASSERT_EQ(0x1234, registerController.getRegisterPair(RegisterPair::B));
}