#include "register.h"

Register::Register() {
    value = 0;
}

void Register::setRegister(uint8_t value) {
    this -> value = value;
}

uint8_t Register::getRegister() {
    return value;
}
