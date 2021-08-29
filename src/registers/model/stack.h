#include "register.h"

#ifndef __STACK_H__
#define __STACK_H__

class Stack : public Register {
    private:
    uint16_t stackPointer;
};

#endif // __STACK_H__