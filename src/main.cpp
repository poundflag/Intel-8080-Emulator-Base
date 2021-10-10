#include "bus/device/ram.h"
#include "bus/device/ramdebug.h"
#include "cpu.h"
#include "io/device/serialout.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main() {
  Cpu cpu = Cpu();
  cpu.getBusController().addChipRegion(
      0, 0x05AF,
      new RamDebug(
          "/Users/robin/Documents/GitHub/Intel-8080-Emulator/src/cpu_dia.com"));

  cpu.getBusController().addChipRegion(0x05AF, 0xFFFF, new Ram(0xFA60));

  cpu.getIOController().addIODevice(new SerialOut(0));

  cpu.step(1500); // 0x12D was the stop
  // The seoncd juimp branch has error

  return 0;
}