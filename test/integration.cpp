#include "../src/bus/device/ramdebug.h"
#include "../src/cpu.h"
#include "../src/io/model/iodevice.h"
#include "../src/bus/device/ram.h"
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
  if (cpu.getRegisterController().getRegister(Registers::C) == 2) {
    if (cpu.getRegisterController().getRegister(Registers::E) != 0) {
      std::cout
          << (char)cpu.getRegisterController().getRegister(Registers::E);
      pOutput +=
          (char)cpu.getRegisterController().getRegister(Registers::E);
    }
  } else if (cpu.getRegisterController().getRegister(Registers::C) == 9) {
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
      new RamDebug("/home/robin/Dokumente/GitHub/Intel-8080-Emulator-Base/src/roms/TST8080.COM"));

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
      new RamDebug("/home/robin/Dokumente/GitHub/Intel-8080-Emulator-Base/src/roms/8080PRE.COM"));

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
      new RamDebug("/home/robin/Dokumente/GitHub/Intel-8080-Emulator-Base/src/roms/CPUTEST.COM"));

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

/*TEST_F(IntegrationsTest, 8080Excerciser) {

  cpu.getBusController().addChipRegion(0x0, 0x99, new Ram(0x100));
  cpu.getBusController().addChipRegion(
      0x100, 0x11FF + 0x100,
      new RamDebug("/home/robin/Dokumente/GitHub/Intel-8080-Emulator-Base/src/roms/8080EXM.COM"));

  cpu.getBusController().addChipRegion(0x1200 + 0x100, 0xFFFF, new Ram(0xFFFE));

  std::string lOutput = "";
  char currentChar = 0;

  int programCounter = 0;

  cpu.setProgramCounter(0x100);

  cpu.getBusController().writeByte(5, 0xC9);

  while (true) {
    cpu.step(1);

    if (cpu.getProgramCounter() == 0) {
      break;
    }

    if (cpu.getProgramCounter() == 5) {
      interceptBDOSCall(cpu, lOutput);
    }
  }

// TODO Finish
  std::string lExpected =
      "8080 instruction exerciser\n\rdad <b,d,h,sp>................  PASS! crc "
      "is:14474ba6\n\raluop nn......................  PASS! crc "
      "is:9e922f9e\n\raluop <b,c,d,e,h,l,m,a>.......  PASS! crc "
      "is:cf762c86\n\r<daa,cma,stc,cmc>.............  PASS! crc "
      "is:bb3f030c\n\r<inr,dcr> a...................  PASS! crc "
      "is:adb6460e\n\r<inr,dcr> b...................  PASS! crc "
      "is:83ed1345\n\r<inx,dcx> b...................  PASS! crc "
      "is:f79287cd\n\r<inr,dcr> c...................  PASS! crc "
      "is:e5f6721b\n\r<inr,dcr> d...................  PASS! crc "
      "is:15b5579a\n\r<inx,dcx> d...................  PASS! crc "
      "is:7f4e2501\n\r<inr,dcr> e...................  PASS! crc "
      "is:cf2ab396\n\r<inr,dcr> h...................  PASS! crc "
      "is:12b2952c\n\r<inx,dcx> h...................  PASS! crc "
      "is:9f2b23c0\n\r<inr,dcr> l...................  PASS! crc "
      "is:ff57d356\n\r<inr,dcr> m...................  PASS! crc "
      "is:92e963bd\n\r<inx,dcx> sp..................  PASS! crc "
      "is:d5702fab\n\rlhld nnnn.....................  PASS! crc "
      "is:a9c3d5cb\n\rshld nnnn.....................  PASS! crc "
      "is:e8864f26\n\rlxi <b,d,h,sp>,nnnn...........  PASS! crc "
      "is:fcf46e12\n\rldax <b,d>....................  PASS! crc "
      "is:2b821d5f\n\rmvi <b,c,d,e,h,l,m,a>,nn......  PASS! crc "
      "is:eaa72044\n\rmov <bcdehla>,<bcdehla>.......  PASS! crc "
      "is:10b58cee\n\rsta nnnn / lda nnnn...........  PASS! crc "
      "is:ed57af72\n\r<rlc,rrc,ral,rar>.............  PASS! crc "
      "is:e0d89235\n\rstax <b,d>....................  PASS! crc "
      "is:2b0471e9\n\rTests complete";
  
  ASSERT_TRUE(lOutput == lExpected);
}*/