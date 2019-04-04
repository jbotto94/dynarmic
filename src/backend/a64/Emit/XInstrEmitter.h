
#ifndef DYNARMIC_XINSTREMITTER_H
#define DYNARMIC_XINSTREMITTER_H

#include "Registers.h"
#include "common/common_types.h"
#include "Common.h"

namespace Dynarmic::BackendA64 {

    class XInstrEmitter {
    private:
        u8* m_code;

        void EncodeArithmeticInst(u32 instenc, bool flags, Reg Rd, Reg Rn, Reg Rm,
                                  ArithOption Option);
        void EncodeArithmeticCarryInst(u32 op, bool flags, Reg Rd, Reg Rn, Reg Rm);
        void EncodeCondCompareImmInst(u32 op, Reg Rn, u32 imm, u32 nzcv, CCFlags cond);
        void EncodeCondCompareRegInst(u32 op, Reg Rn, Reg Rm, u32 nzcv, CCFlags cond);
        void EncodeCondSelectInst(u32 instenc, Reg Rd, Reg Rn, Reg Rm, CCFlags cond);

    protected:
        void Write32(u32 value);

    public:
        // Add/Subtract (Extended/Shifted register)
        void ADD(Reg Rd, Reg Rn, Reg Rm);
        void ADD(Reg Rd, Reg Rn, Reg Rm, ArithOption Option);
        void ADDS(Reg Rd, Reg Rn, Reg Rm);
        void ADDS(Reg Rd, Reg Rn, Reg Rm, ArithOption Option);
        void SUB(Reg Rd, Reg Rn, Reg Rm);
        void SUB(Reg Rd, Reg Rn, Reg Rm, ArithOption Option);
        void SUBS(Reg Rd, Reg Rn, Reg Rm);
        void SUBS(Reg Rd, Reg Rn, Reg Rm, ArithOption Option);
        void CMN(Reg Rn, Reg Rm);
        void CMN(Reg Rn, Reg Rm, ArithOption Option);
        void CMP(Reg Rn, Reg Rm);
        void CMP(Reg Rn, Reg Rm, ArithOption Option);

        // Add/Subtract (with carry)
        void ADC(Reg Rd, Reg Rn, Reg Rm);
        void ADCS(Reg Rd, Reg Rn, Reg Rm);
        void SBC(Reg Rd, Reg Rn, Reg Rm);
        void SBCS(Reg Rd, Reg Rn, Reg Rm);

        // Conditional Compare (immediate)
        void CCMN(Reg Rn, u32 imm, u32 nzcv, CCFlags cond);
        void CCMP(Reg Rn, u32 imm, u32 nzcv, CCFlags cond);

        // Conditional Compare (register)
        void CCMN(Reg Rn, Reg Rm, u32 nzcv, CCFlags cond);
        void CCMP(Reg Rn, Reg Rm, u32 nzcv, CCFlags cond);

        // Conditional Select
        void CSEL(Reg Rd, Reg Rn, Reg Rm, CCFlags cond);
        void CSINC(Reg Rd, Reg Rn, Reg Rm, CCFlags cond);
        void CSINV(Reg Rd, Reg Rn, Reg Rm, CCFlags cond);
        void CSNEG(Reg Rd, Reg Rn, Reg Rm, CCFlags cond);

        // Data-Processing 1 source
        void RBIT(Reg Rd, Reg Rn);
        void REV16(Reg Rd, Reg Rn);
        void REV32(Reg Rd, Reg Rn);
        void REV64(Reg Rd, Reg Rn);
        void CLZ(Reg Rd, Reg Rn);
        void CLS(Reg Rd, Reg Rn);

        // Data-Processing 2 source
        void UDIV(Reg Rd, Reg Rn, Reg Rm);
        void SDIV(Reg Rd, Reg Rn, Reg Rm);
        void LSLV(Reg Rd, Reg Rn, Reg Rm);
        void LSRV(Reg Rd, Reg Rn, Reg Rm);
        void ASRV(Reg Rd, Reg Rn, Reg Rm);
        void RORV(Reg Rd, Reg Rn, Reg Rm);
        void CRC32B(Reg Rd, Reg Rn, Reg Rm);
        void CRC32H(Reg Rd, Reg Rn, Reg Rm);
        void CRC32W(Reg Rd, Reg Rn, Reg Rm);
        void CRC32CB(Reg Rd, Reg Rn, Reg Rm);
        void CRC32CH(Reg Rd, Reg Rn, Reg Rm);
        void CRC32CW(Reg Rd, Reg Rn, Reg Rm);
        void CRC32X(Reg Rd, Reg Rn, Reg Rm);
        void CRC32CX(Reg Rd, Reg Rn, Reg Rm);

        // Data-Processing 3 source
        void MADD(Reg Rd, Reg Rn, Reg Rm, Reg Ra);
        void MSUB(Reg Rd, Reg Rn, Reg Rm, Reg Ra);
        void SMADDL(Reg Rd, Reg Rn, Reg Rm, Reg Ra);
        void SMULL(Reg Rd, Reg Rn, Reg Rm);
        void SMSUBL(Reg Rd, Reg Rn, Reg Rm, Reg Ra);
        void SMULH(Reg Rd, Reg Rn, Reg Rm);
        void UMADDL(Reg Rd, Reg Rn, Reg Rm, Reg Ra);
        void UMULL(Reg Rd, Reg Rn, Reg Rm);
        void UMSUBL(Reg Rd, Reg Rn, Reg Rm, Reg Ra);
        void UMULH(Reg Rd, Reg Rn, Reg Rm);
        void MUL(Reg Rd, Reg Rn, Reg Rm);
        void MNEG(Reg Rd, Reg Rn, Reg Rm);
    };

};
#endif //DYNARMIC_XINSTREMITTER_H
