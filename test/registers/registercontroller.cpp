#include "../../src/registers/registercontroller.h"
#include "../../src/registers/registers.h"
#include <gtest/gtest.h>

class RegisterControllerTest : public ::testing::Test {

protected:
  std::shared_ptr<BusController> busController =
      std::make_shared<BusController>();
  RegisterController registerController = RegisterController(busController);
  void SetUp() { busController->addChipRegion(ChipRegion(0, 10, new Ram(10))); }
};

TEST_F(RegisterControllerTest, getRegister) {
  registerController.get(Registers::A).setRegister(0x15);

  GTEST_ASSERT_EQ(0x15,
                  registerController.get(Registers::A).getRegister());
}
