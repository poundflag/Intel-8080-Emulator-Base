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

  cpu.getBusController().addChipRegion(0x0, 0x9FF, ram);

  cpu.run();
}

int main() {
  // runBasic4K();
  basicInstructionRun();
  getch();
  endwin();
}