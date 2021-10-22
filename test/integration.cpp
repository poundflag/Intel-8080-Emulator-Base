#include "../src/bus/device/ramdebug.h"
#include "../src/cpu.h"
#include "../src/io/model/iodevice.h"
#include <gtest/gtest.h>
#include <string>

class IntegrationsTest : public ::testing::Test {

protected:
  Cpu cpu = Cpu();
  void SetUp() {}
};

TEST_F(IntegrationsTest, EasyCpuTest) {
  cpu.getBusController().addChipRegion(
      0, 0x05AF,
      new RamDebug("/Users/robin/Documents/GitHub/Intel-8080-Emulator/src/roms/"
                   "cpu_dia.com"));

  cpu.getBusController().addChipRegion(0x05AF, 0xFFFF, new Ram(0xFA60));

  uint8_t b[] = {0};
  cpu.getIOController().addIODevice(new IODevice(b, sizeof(b) / sizeof(b[0])));
  std::string lOutput = "";
  char c = 0;

  for (int i = 0; i < 1500; i++) {
    cpu.step(1);
    if (cpu.getIOController().getDeviceValue(0) != 0) {
      c = cpu.getIOController().getDeviceValue(0);
      lOutput += c;
      cpu.getIOController().setDeviceValue(0, 0);
    }
  }

  bool found = false;
  std::string foundMessage = "CPU IS OPERATIONAL$";

  found = lOutput.find(foundMessage) != std::string::npos;

  ASSERT_TRUE(found);
}

TEST_F(IntegrationsTest, 8080Preliminary) {

  // cpu.getBusController().addChipRegion(0x0, 0x99, new Ram(0x100));
  cpu.getBusController().addChipRegion(
      0x100, 0x3FF + 0x100,
      new RamDebug("/Users/robin/Documents/GitHub/Intel-8080-Emulator/src/roms/"
                   "8080PRE.COM"));

  cpu.getBusController().addChipRegion(0x0400, 0xFFFF, new Ram(0xFA60));

  std::string lOutput = "";
  char currentChar = 0;

  int programCounter = 0;
  std::setvbuf(stdout, NULL, _IONBF, 0);
  while (programCounter != 0x32A) {
    cpu.step(1);
    programCounter = cpu.getProgramCounter();
  }

  uint16_t lDRegPair =
      cpu.getRegisterController().getRegisterPair(RegisterPair::D);

  while (currentChar != '$') {
    currentChar = cpu.getBusController().readByte(lDRegPair);
    lOutput.push_back(currentChar);
    lDRegPair++;
    if (lDRegPair == 0) {
      std::cout << "Hello";
      currentChar = '$';
    }
  }

  bool found = false;
  std::string foundMessage = "8080 Preliminary tests complete$";

  found = lOutput.find(foundMessage) != std::string::npos;

  ASSERT_TRUE(found);
}
/*
TEST_F(IntegrationsTest, 8080Excerciser) {

  cpu.getBusController().addChipRegion(0x0, 0x99, new Ram(0x100));
  cpu.getBusController().addChipRegion(
      0x100, 0x11FF + 0x100,
      new RamDebug("/Users/robin/Documents/GitHub/Intel-8080-Emulator/src/roms/"
                   "8080EXER.COM"));

  cpu.getBusController().addChipRegion(0x11FF + 0x100, 0xFFFF, new Ram(0xFA60));

  std::string lOutput = "";
  char currentChar = 0;

  int programCounter = 0;

  cpu.setProgramCounter(cpu.getProgramCounter() + 0x100);

  while (true) {
    cpu.step(1);
    uint16_t lDRegPair =
        cpu.getRegisterController().getRegisterPair(RegisterPair::D);
    if (cpu.getProgramCounter() == 5) {
      while (currentChar != '$') {
        currentChar = cpu.getBusController().readByte(lDRegPair);
        std::cout << currentChar;
        lDRegPair++;
      }
      currentChar = 0;
      cpu.getBusController().writeByte(5, 0xC9);
    }
  }

  bool found = false;
  std::string foundMessage = "8080 Preliminary tests complete$";

  found = lOutput.find(foundMessage) != std::string::npos;

  ASSERT_TRUE(found);
}*/