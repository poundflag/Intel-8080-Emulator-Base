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

void interceptBDOSCall(Cpu &cpu) {
  // Stolen from
  // https://github.com/GunshipPenguin/lib8080/blob/master/test/integration/cpmloader.c
  /*
  void intercept_bdos_call(struct i8080 *cpu) {
  if (cpu->C == 2) { // BDOS function 2 (C_WRITE) - Console output
    if (cpu->E != 0) {
      putchar((char) cpu->E);
    }
  } else if (cpu->C == 9) { // BDOS function 9 (C_WRITESTR) - Output string
    for (int addr = ((cpu->D << 8) | cpu->E); i8080_read_byte(cpu, addr) !=
'$';addr++) { if (i8080_read_byte(cpu, addr) != 0) { putchar((char)
i8080_read_byte(cpu, addr));
      }
    }
  }
}
  */
  std::setvbuf(stdout, NULL, _IONBF, 0);
  if (cpu.getRegisterController().get(Registers::C).getRegister() == 2) {
    if (cpu.getRegisterController().get(Registers::E).getRegister() != 0) {
      std::cout
          << (char)cpu.getRegisterController().get(Registers::E).getRegister();
    }
  } else if (cpu.getRegisterController().get(Registers::C).getRegister() == 9) {
    for (int addr =
             cpu.getRegisterController().getRegisterPair(RegisterPair::D);
         cpu.getBusController().readByte(addr) != '$'; addr++) {
      if (cpu.getBusController().readByte(addr) != 0) {
        std::cout << (char)cpu.getBusController().readByte(addr);
      }
    }
  }
}

int main() {

  Cpu cpu = Cpu();
  /*cpu.getBusController().addChipRegion(
       0x0, 0x0FFF,
       new
   RamDebug("/Users/robin/Documents/GitHub/Intel-8080-Emulator/src/roms/"
                    "4kbas32.bin"));

   cpu.getBusController().addChipRegion(0x1000, 0xFFFE, new Ram(0xFFFE));
   uint8_t a[] = {0xFF};
   cpu.getIOController().addIODevice(new AltairSwitches(a, 1));
   cpu.getIOController().setDeviceValue(0xFF, 0b00); // b00100010

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
                   "8080EXM.COM"));

  cpu.getBusController().addChipRegion(0x1200 + 0x100, 0xFFFF, new Ram(0xFFFE));

  std::string lOutput = "";
  char currentChar = 0;

  int programCounter = 0;

  cpu.setProgramCounter(0x100);

  cpu.getBusController().writeByte(5, 0xC9);

  while (true) {
    cpu.step(1);

    if (cpu.getProgramCounter() == 0) {
      return 0;
    }

    if (cpu.getProgramCounter() == 5) {
      interceptBDOSCall(cpu);
    }
  }
}