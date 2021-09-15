#include "bus/buscontroller.h"
#include "registers/model/register.h"
#include "registers/registercontroller.h"
#include "registers/registers.h"

#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

class Instructions {
private:
  std::shared_ptr<BusController> busController;
  std::shared_ptr<RegisterController> registerController;

public:
  Instructions(std::shared_ptr<BusController> busController,
               std::shared_ptr<RegisterController> registerController);
  void MOV(Register &destination, Register &source);
  void MVI(Register &destination, uint8_t immediate);
  void LXI(RegisterPair registerPair, uint16_t immediate);
  void LDA(uint16_t address);
  void STA(uint16_t address);
  void LHLD();
  void SHLD();
  void LDAX();
  void STAX();
  void XCHG();
  void ADD(Register source);
  void ADI(uint8_t immediate);
  void ADC(Register source);
  void ACI(uint8_t immediate);
  void SUB(Register source);
  void SUI(uint8_t immediate);
  void SBB(Register source);
  void SBI(uint8_t immediate);
  void INR(Register &destination);
  void DCR(Register &destination);
  void INX();
  void DCX();
  void DAD();
  void DAA();
  void ANA(Register source);
  void ANI(uint8_t immediate);
  void ORA(Register source);
  void ORI(uint8_t immediate);
  void XRA(Register source);
  void XRI(uint8_t immediate);
  void CMP(Register source);
  void CPI(uint8_t immediate);
  void RLC();
  void RRC();
  void RAL();
  void RAR();
  void CMA();
  void CMC();
  void STC();
  void JMP(uint16_t &source, uint16_t address);
  void conditionSuccessful();
  void JMPCondition();
  void CALL(uint16_t &source, uint16_t address);
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