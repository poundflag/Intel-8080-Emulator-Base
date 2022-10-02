#include "sio.h"

SIO::SIO(uint8_t ports[], int size) : IODevice{ports, size} {
  initscr();
  cbreak();
  noecho();
  clear();
  // refresh();
  scrollok(stdscr, TRUE);
  // mvcur(0, 0, 0, 0);
}

void SIO::setValue(uint8_t value, uint8_t portAddress) {
  // std::cout << (char)value;
  if (value == '\r') {
    addch(' ');
  } else {
    addch(value);
  }
  refresh();
}

uint8_t SIO::getValue(uint8_t portAddress) {
  if (portAddress == 0x10 || portAddress == 0x0) {
    return 0x02;
  } else if (portAddress == 1) {

    int lChar = getch();
    if (lChar == '\n') {
      return '\r';
    }

    return lChar;
  }
  return 0;
}