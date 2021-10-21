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

int main() {

  /*initscr();

  char c = 0;
  while (c != 10) {
    c = getch();
  }
  // endwin();
  */
  // initscr();
  // initscr();
  Cpu cpu = Cpu();
  /*cpu.getBusController().addChipRegion(
      0x0, 0x0EFC,
      new RamDebug("/Users/robin/Documents/GitHub/Intel-8080-Emulator/src/roms/"
                   "BASIC4k32.rom"));

  cpu.getBusController().addChipRegion(0x0EFD, 0x32FF, new Ram(0x32FF));
  uint8_t a[] = {0xFF};
  // uint8_t b[] = {1};
  // cpu.getIOController().addIODevice(new SerialOut(b, 1));
  cpu.getIOController().addIODevice(new AltairSwitches(a, 1));
  cpu.getIOController().setDeviceValue(0xFF, 0b00000000);

  uint8_t ports[4] = {0, 1, 0x10, 0x11};
  cpu.getIOController().addIODevice(
      new SIO(ports, sizeof(ports) / sizeof(ports[0])));

  std::setvbuf(stdout, NULL, _IONBF, 0);

  // cpu.step(1500);
  cpu.run();*/

  cpu.getBusController().addChipRegion(0x0, 0x99, new Ram(0x100));
  cpu.getBusController().addChipRegion(
      0x100, 0x11FF + 0x100,
      new RamDebug("/Users/robin/Documents/GitHub/Intel-8080-Emulator/src/roms/"
                   "8080EXER.COM"));

  cpu.getBusController().addChipRegion(0x1200 + 0x100, 0xFFFF, new Ram(0xFA60));

  std::string lOutput = "";
  char currentChar = 0;

  int programCounter = 0;

  cpu.setProgramCounter(cpu.getProgramCounter() + 0x100);

  while (true) {
    cpu.step(1);
    uint16_t lDRegPair =
        cpu.getRegisterController().getRegisterPair(RegisterPair::D);
    if (cpu.getProgramCounter() == 5) {
      std::setvbuf(stdout, NULL, _IONBF, 0);
      while (currentChar != '$') {
        currentChar = cpu.getBusController().readByte(lDRegPair);
        std::cout << currentChar;
        lDRegPair++;
      }
      currentChar = 0;
      cpu.getBusController().writeByte(5, 0xC9);
    }
  }
}