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
      new RamDebug("/Users/robin/Documents/GitHub/Intel-8080-Emulator/src/roms/"
                   "4kbas40.bin"));

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
      new RamDebug("/Users/robin/Documents/GitHub/Intel-8080-Emulator/src/roms/"
                   "8kbas.bin"));

  cpu.getBusController().addChipRegion(0x2000, 0xFFFE, new Ram(0xFFFE));
  uint8_t a[] = {0xFF};
  cpu.getIOController().addIODevice(new AltairSwitches(a, 1));
  cpu.getIOController().setDeviceValue(0xFF, 0b00100010); // b00100010

  uint8_t ports[4] = {0, 1, 0x10, 0x11};
  cpu.getIOController().addIODevice(
      new SIO(ports, sizeof(ports) / sizeof(ports[0])));

  cpu.run();
}

int main() {
  runBasic4K();
  getch();
  endwin();
  
}