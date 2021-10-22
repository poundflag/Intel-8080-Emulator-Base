#include "sio.h"
#include <iostream>
#include <ncurses.h>

SIO::SIO(uint8_t ports[], int size) : IODevice{ports, size} {}

void SIO::setValue(uint8_t value, uint8_t portAddress) {
  if (value == 'Z') {
    int a = 0;
  }
  std::cout << (char)value;
}

uint8_t SIO::getValue(uint8_t portAddress) {
  if (portAddress == 0x10 || portAddress == 0x0) {
    return 0x02;
  } else if (portAddress == 1) {
    // return 0x41;
    char c = 0;

    // std::cin >> c;
    /*
        if (c == 'o') {
          c = '\r';
          std::cout << "A";
        }
    */
    if (index < test.length()) {
      c = test[index];
      index++;
    } else {
      while (c == 0) {
        std::cout << "b";
        std::cin >> c;
        if (c == 'm') {
          c = '\r';
        }
        if (c == 'n') {
          c = ' ';
        }
      }
    }

    return c;
  }
  return 0;
}
