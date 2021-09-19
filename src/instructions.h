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
  bool conditionSuccessful(FlagRegister::Condition condition);

public:
  Instructions(std::shared_ptr<BusController> busController,
               std::shared_ptr<RegisterController> registerController);
  void MOV(Register &destination,
           Register &source); // TODO Fix the paramenter to enum
  void MVI(Register &destination, uint8_t immediate);
  void LXI(RegisterPair registerPair, uint16_t immediate);
  void LDA(uint16_t address);
  void STA(uint16_t address);
  void LHLD(uint16_t address);
  void SHLD(uint16_t address);
  void LDAX(RegisterPair indirectAddress);
  void STAX(RegisterPair indirectAddress);
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
  void INX(RegisterPair destination);
  void DCX(RegisterPair destination);
  void DAD(RegisterPair source);
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
  void JMPCondition(uint16_t &source, uint16_t address,
                    FlagRegister::Condition condition);
  void CALL(uint16_t &source, uint16_t address);
  void CALLCondition(uint16_t &source, uint16_t address,
                     FlagRegister::Condition condition);
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