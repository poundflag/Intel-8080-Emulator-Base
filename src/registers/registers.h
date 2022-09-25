#ifndef __REGISTERS_H__
#define __REGISTERS_H__

namespace Registers {
enum Register { A = 0,
                B,
                C,
                D,
                E,
                H,
                L,
                // Those registers are not accessible to the normal user
                // (Can not be used in instructions)
                InstructionRegister,
                TemporaryHigh,
                TemporaryLow,
                MemoryReference };
}

enum RegisterPair { PSW,
                    B,
                    D,
                    H,
                    SP,
                    // Private Register
                    Temporary };
#endif // __REGISTERS_H__