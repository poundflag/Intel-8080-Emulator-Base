#include "../../src/registers/registercontroller.h"
#include "../../src/registers/registers.h"
#include <gtest/gtest.h>

class RegisterControllerTest : public ::testing::Test {

protected:
  BusController busController = BusController();
  RegisterController registerController = RegisterController(busController);
  void SetUp() { busController.addChipRegion(ChipRegion(0, 10, new Ram(10))); }
};

TEST_F(RegisterControllerTest, getRegister) {
  registerController.get(Registers::A).setRegister(0x15);

  GTEST_ASSERT_EQ(0x15, registerController.get(Registers::A).getRegister());
}

TEST_F(RegisterControllerTest, getMemoryReference) {
  registerController.setRegisterPair(RegisterPair::H, 0x0005);
  // busController.writeByte(0x0005, 0x12);
  registerController.get(Registers::M).setRegister(0x15);

  GTEST_ASSERT_EQ(0x15, registerController.get(Registers::M).getRegister());
  GTEST_ASSERT_EQ(0x15,
                  busController.readByte(
                      registerController.getRegisterPair(RegisterPair::H)));
}

TEST_F(RegisterControllerTest, getRegisterPairWithPairSet) {
  registerController.setRegisterPair(RegisterPair::B, 0x1234);

  GTEST_ASSERT_EQ(0x1234, registerController.getRegisterPair(RegisterPair::B));
}

TEST_F(RegisterControllerTest, getRegisterPairWithSingleSet) {
  registerController.get(Registers::B).setRegister(0x12);
  registerController.get(Registers::C).setRegister(0x34);

  GTEST_ASSERT_EQ(0x1234, registerController.getRegisterPair(RegisterPair::B));
}