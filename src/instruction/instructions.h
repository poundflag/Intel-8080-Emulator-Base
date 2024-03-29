#include "../bus/buscontroller.h"
#include "../core/alu.h"
#include "../io/iocontroller.h"
#include "../registers/model/register.h"
#include "../registers/registercontroller.h"
#include "../registers/registers.h"
#include "../registers/model/flagregister.h"

#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

// Contains all instructions for the i8080
class Instructions {
private:
  BusController &busController;
  RegisterController &registerController;
  IOController &ioController;
  ALU &alu;

  bool conditionSuccessful(FlagRegister::Condition condition);
  void memoryReadOneByte(uint8_t machineCycle);
  void memoryReadTwoBytes();

public:
  Instructions(BusController &busController,
               RegisterController &registerController,
               IOController &ioController, ALU &alu);
  void MOV(Registers::Register destination, Registers::Register source);
  void MVI(Registers::Register destination);
  void LXI(RegisterPair registerPair);
  void LDA();
  void STA();
  void LHLD();
  void SHLD();
  void LDAX(RegisterPair indirectAddress);
  void STAX(RegisterPair indirectAddress);
  void XCHG();
  void ADD(Registers::Register source);
  void ADI();
  void ADC(Registers::Register source);
  void ACI();
  void SUB(Registers::Register source);
  void SUI();
  void SBB(Registers::Register source);
  void SBI();
  void INR(Registers::Register destination);
  void DCR(Registers::Register destination);
  void INX(RegisterPair destination);
  void DCX(RegisterPair destination);
  void DAD(RegisterPair source);
  void DAA();
  void ANA(Registers::Register source);
  void ANI();
  void ORA(Registers::Register source);
  void ORI();
  void XRA(Registers::Register source);
  void XRI();
  void CMP(Registers::Register source);
  void CPI();
  void RLC();
  void RRC();
  void RAL();
  void RAR();
  void CMA();
  void CMC();
  void STC();
  void JMP();
  bool JMPCondition(FlagRegister::Condition condition);
  void CALL();
  bool CALLCondition(FlagRegister::Condition condition);
  void RET();
  bool RETCondition(FlagRegister::Condition condition);
  void RST(uint8_t n);
  void PCHL(uint16_t &source);
  void PUSH(RegisterPair registerPair);
  void POP(RegisterPair registerPair);
  void XTHL();
  void SPHL();
  void IN(int portNumber);
  void OUT(int portNumber);
  void EI();
  void DI();
  bool HLT();
};

#endif // __INSTRUCTIONS_H__