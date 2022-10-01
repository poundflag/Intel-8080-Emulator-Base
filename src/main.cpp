#include "bus/device/ram.h"
#include "bus/device/ramdebug.h"
#include "cpu.h"
#include "io/device/altairswitches.h"
#include "io/device/serialout.h"
#include "io/device/sio.h"
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <string>

using namespace std;

void runBasic4K() {
  Cpu cpu = Cpu();
  cpu.getBusController().addChipRegion(
      0x0, 0x0EF8,
      new RamDebug("/home/robin/Dokumente/GitHub/Intel-8080-Emulator-Base/src/roms/4kbas40.bin"));

  cpu.getBusController().addChipRegion(0xEF9, 0xFFFE, new Ram(0xFFFE));
  uint8_t a[] = {0xFF};
  cpu.getIOController().addIODevice(new AltairSwitches(a, 1));
  cpu.getIOController().setDeviceValue(0xFF, 0b00100010); // b00100010

  uint8_t ports[4] = {0, 1, 0x10, 0x11};
  cpu.getIOController().addIODevice(
      new SIO(ports, sizeof(ports) / sizeof(ports[0])));

  cpu.run();
}

void runBasic8K() {
  Cpu cpu = Cpu();
  cpu.getBusController().addChipRegion(
      0x0, 0x1FFF,
      new RamDebug("/home/robin/Dokumente/GitHub/Intel-8080-Emulator-Base/src/roms/8kbas.bin"));

  cpu.getBusController().addChipRegion(0x2000, 0xFFFE, new Ram(0xFFFE));
  uint8_t a[] = {0xFF};
  cpu.getIOController().addIODevice(new AltairSwitches(a, 1));
  cpu.getIOController().setDeviceValue(0xFF, 0b00100010); // b00100010

  uint8_t ports[4] = {0, 1, 0x10, 0x11};
  cpu.getIOController().addIODevice(
      new SIO(ports, sizeof(ports) / sizeof(ports[0])));

  cpu.run();
}

void basicInstructionRun() {
  Cpu cpu = Cpu();
  Ram *ram = new Ram(0x100);
  ram->write(0, 0x40);
  ram->write(1, 0x06);
  ram->write(2, 0x27);

  cpu.getBusController().addChipRegion(0x0, 0x9FF, ram);

  cpu.run();
}

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

void TST8080() {

  Cpu cpu = Cpu();
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
}

int main() {
  // runBasic4K();
  TST8080();
  getch();
  endwin();
}