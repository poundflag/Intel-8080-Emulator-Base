#include "serialout.h"

SerialOut::SerialOut(uint8_t portNumber) : IODevice(portNumber) {}

void SerialOut::setValue(uint8_t value) {
  this->value = value;
  std::cout << (char)value;
}
