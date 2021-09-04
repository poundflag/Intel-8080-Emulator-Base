#include "bus/buscontroller.h"
#include "registers/model/register.h"
#include "registers/registercontroller.h"
#include "registers/registers.h"

#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

class Instructions {

public:
  void MOV(Register &destination, Register &source);
  void MVI(Register destination, uint8_t immediate);
  void LXI();
  void LDA(uint8_t address);
  void STA(uint8_t address);
  void LHLD();
  void SHLD();
  void LDAX();
  void STAX();
  void XCHG();
  void ADD(Registers::Register source);
  void ADI(uint8_t immediate);
  void ADC(Registers::Register source);
  void ACI(uint8_t immediate);
  void SUB(Registers::Register source);
  void SUI(uint8_t immediate);
  void SBB(Registers::Register source);
  void SBI(uint8_t immediate);
  void INR(Registers::Register destination);
  void DCR(Registers::Register destination);
  void INX();
  void DCX();
  void DAD();
  void DAA();
  void ANA(Registers::Register source);
  void ANI(uint8_t immediate);
  void ORA(Registers::Register source);
  void ORI(uint8_t immediate);
  void XRA(Registers::Register source);
  void XRI(uint8_t immediate);
  void CMP(Registers::Register source);
  void CPI(uint8_t immediate);
  void RLC();
  void RRC();
  void RAL();
  void RAR();
  void CMA();
  void CMC();
  void STC();
  void JMP(uint16_t address);
  void conditionSuccessful();
  void JMPCondition();
  void CALL(uint16_t address);
  void CALLCondition();
  void RET();
  void RETCondition();
  void RST();
  void PCHL();
  void PUSH();
  void POP();
  void XTHL();
  void SPHL();
  void IN();
  void OUT();
  void EI();
  void DI();
  bool HLT();
};

#endif // __INSTRUCTIONS_H__