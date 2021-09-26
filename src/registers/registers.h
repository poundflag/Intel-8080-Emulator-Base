#ifndef __REGISTERS_H__
#define __REGISTERS_H__

namespace Registers {
enum Register { A = 0, B, C, D, E, H, L, M, F };
} // namespace Registers

enum RegisterPair { PSW, B, D, H, SP };
#endif // __REGISTERS_H__