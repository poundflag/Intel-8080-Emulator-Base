#include "bus/buscontroller.h"
#include "registers/model/register.h"
#include "registers/registercontroller.h"
#include "registers/registers.h"

#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

class Instructions {
private:
  BusController &busController;
  RegisterController &registerController;
  bool conditionSuccessful(FlagRegister::Condition condition);

public:
  Instructions(BusController &busController,
               RegisterController &registerController);
  void MOV(Registers::Register destination,
           Registers::Register source); // TODO Fix the paramenter to enum
  void MVI(Registers::Register destination, uint8_t immediate);
  void LXI(RegisterPair registerPair, uint16_t immediate);
  void LDA(uint16_t address);
  void STA(uint16_t address);
  void LHLD(uint16_t address);
  void SHLD(uint16_t address);
  void LDAX(RegisterPair indirectAddress);
  void STAX(RegisterPair indirectAddress);
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
  void INX(RegisterPair destination);
  void DCX(RegisterPair destination);
  void DAD(RegisterPair source);
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
  void JMP(uint16_t &source, uint16_t address);
  bool JMPCondition(uint16_t &source, uint16_t address,
                    FlagRegister::Condition condition);
  void CALL(uint16_t &source, uint16_t address);
  bool CALLCondition(uint16_t &source, uint16_t address,
                     FlagRegister::Condition condition);
  void RET(uint16_t &source);
  bool RETCondition(uint16_t &source, FlagRegister::Condition condition);
  void RST();
  void PCHL(uint16_t &source);
  void PUSH(RegisterPair registerPair);
  void POP(RegisterPair registerPair);
  void XTHL();
  void SPHL();
  void IN();
  void OUT(int portNumber);
  void EI();
  void DI();
  bool HLT();
};

#endif // __INSTRUCTIONS_H__