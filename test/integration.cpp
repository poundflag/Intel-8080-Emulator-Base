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

void interceptBDOSCall(Cpu &cpu, std::string &pOutput) {
  // Stolen from
  // https://github.com/GunshipPenguin/lib8080/blob/master/test/integration/cpmloader.c

  std::setvbuf(stdout, NULL, _IONBF, 0);
  if (cpu.getRegisterController().get(Registers::C).getRegister() == 2) {
    if (cpu.getRegisterController().get(Registers::E).getRegister() != 0) {
      std::cout
          << (char)cpu.getRegisterController().get(Registers::E).getRegister();
      pOutput +=
          (char)cpu.getRegisterController().get(Registers::E).getRegister();
    }
  } else if (cpu.getRegisterController().get(Registers::C).getRegister() == 9) {
    for (int addr =
             cpu.getRegisterController().getRegisterPair(RegisterPair::D);
         cpu.getBusController().readByte(addr) != '$'; addr++) {
      if (cpu.getBusController().readByte(addr) != 0) {
        std::cout << (char)cpu.getBusController().readByte(addr);
        pOutput += (char)cpu.getBusController().readByte(addr);
      }
    }
  }
}

TEST_F(IntegrationsTest, TST8080) {
  cpu.getBusController().addChipRegion(0x0, 0x99, new Ram(0x100));
  cpu.getBusController().addChipRegion(
      0x100, 0x5FF + 0x100,
      new RamDebug("/Users/robin/Documents/GitHub/Intel-8080-Emulator/src/roms/"
                   "TST8080.COM"));

  cpu.getBusController().addChipRegion(0x0400, 0xFFFF, new Ram(0xFA60));

  std::string lOutput = "";

  cpu.getBusController().writeByte(5, 0xC9);
  cpu.setProgramCounter(0x100);

  while (true) {
    cpu.step(1);

    if (cpu.getProgramCounter() == 0) {
      break;
    }

    if (cpu.getProgramCounter() == 5) {
      interceptBDOSCall(cpu, lOutput);
    }
  }

  std::string foundMessage =
      "MICROCOSM ASSOCIATES 8080/8085 CPU DIAGNOSTIC\r\n VERSION 1.0  (C) "
      "1980\r\n\r\n CPU IS OPERATIONAL";

  ASSERT_TRUE(lOutput == foundMessage);
}

TEST_F(IntegrationsTest, 8080PRE) {
  cpu.getBusController().addChipRegion(0x0, 0x99, new Ram(0x100));
  cpu.getBusController().addChipRegion(
      0x100, 0x3FF + 0x100,
      new RamDebug("/Users/robin/Documents/GitHub/Intel-8080-Emulator/src/roms/"
                   "8080PRE.COM"));

  cpu.getBusController().addChipRegion(0x0400, 0xFFFF, new Ram(0xFA60));

  std::string lOutput = "";

  cpu.getBusController().writeByte(5, 0xC9);
  cpu.setProgramCounter(0x100);

  while (true) {
    cpu.step(1);

    if (cpu.getProgramCounter() == 0) {
      break;
    }

    if (cpu.getProgramCounter() == 5) {
      interceptBDOSCall(cpu, lOutput);
    }
  }

  std::string foundMessage = "8080 Preliminary tests complete";

  ASSERT_TRUE(lOutput == foundMessage);
}

TEST_F(IntegrationsTest, CPUTEST) {
  cpu.getBusController().addChipRegion(0x0, 0x99, new Ram(0x100));
  cpu.getBusController().addChipRegion(
      0x100, 0x4AFF + 0x100,
      new RamDebug("/Users/robin/Documents/GitHub/Intel-8080-Emulator/src/roms/"
                   "CPUTEST.COM"));

  cpu.getBusController().addChipRegion(0x4B00 + 0x100, 0xFFFF, new Ram(0xFA60));

  std::string lOutput = "";

  cpu.getBusController().writeByte(5, 0xC9);
  cpu.setProgramCounter(0x100);

  while (true) {
    cpu.step(1);

    if (cpu.getProgramCounter() == 0) {
      break;
    }

    if (cpu.getProgramCounter() == 5) {
      interceptBDOSCall(cpu, lOutput);
    }
  }

  std::string foundMessage =
      "\r\nDIAGNOSTICS II V1.2 - CPU TEST\r\nCOPYRIGHT (C) 1981 - SUPERSOFT "
      "ASSOCIATES\r\n\nABCDEFGHIJKLMNOPQRSTUVWXYZ\r\nCPU IS 8080/8085\r\nBEGIN "
      "TIMING TEST\r\n\a\aEND TIMING TEST\r\nCPU TESTS OK\r\n";

  ASSERT_TRUE(lOutput == foundMessage);
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