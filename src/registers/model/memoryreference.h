#include "../../bus/buscontroller.h"
#include "register.h"

#ifndef __MEMORYREFERENCE_H__
#define __MEMORYREFERENCE_H__

class MemoryReference : public Register {
private:
  BusController &busController;

public:
  MemoryReference(BusController &busController); // TODO Finish later when the register pair structure is clear
};

#endif // __MEMORYREFERENCE_H__