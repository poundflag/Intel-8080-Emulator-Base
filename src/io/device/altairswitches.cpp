#include "altairswitches.h"

AltairSwitches::AltairSwitches(uint8_t portAddress[], int size)
    : IODevice{portAddress, size} {}

uint8_t AltairSwitches::getValue(uint8_t portAddress) { return value; }

void AltairSwitches::setValue(uint8_t value,uint8_t portAddress) { this->value = value; }
