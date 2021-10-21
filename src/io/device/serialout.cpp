#include "serialout.h"

SerialOut::SerialOut(uint8_t portAddress[], int size)
    : IODevice(portAddress, size) {}

void SerialOut::setValue(uint8_t value, uint8_t portAddress) {
  this->value = value;
  std::cout << (char)value;
}
