#include "bus/device/ramdebug.h"
#include "cpu.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main() {
  Cpu cpu = Cpu();
  cpu.getBusController().addChipRegion(
      0, 9,
      new RamDebug(
          "/Users/robin/Documents/GitHub/Intel-8080-Emulator/src/example.com"));

  cpu.step(3);
  return 0;
}