#ifndef __FLAGENUM_H__
#define __FLAGENUM_H__

enum class FlagRule { All, Partial, CarryOnly };

enum Flag { Carry = 0, AuxiliaryCarry, Parity, Zero, Signed };
#endif // __FLAGENUM_H__