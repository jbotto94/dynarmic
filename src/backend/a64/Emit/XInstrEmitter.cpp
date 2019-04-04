

#include "XInstrEmitter.h"
#include "Common.h"

namespace Dynarmic::BackendA64 {
    void XInstrEmitter::EncodeArithmeticInst(u32 instenc, bool flags, Reg Rd, Reg Rn, Reg Rm,
                                             ArithOption Option) {
        bool b64Bit = Is64Bit(Rd);

        Rd = DecodeReg(Rd);
        Rn = DecodeReg(Rn);
        Rm = DecodeReg(Rm);
        Write32((b64Bit << 31) | (flags << 29) | (ArithEnc[instenc] << 21) |
                (Option.GetType() == ArithOption::TYPE_EXTENDEDREG ? (1 << 21) : 0) | (Rm << 16) |
                Option.GetData() | (Rn << 5) | Rd);

    }

    void XInstrEmitter::EncodeArithmeticCarryInst(u32 op, bool flags, Reg Rd, Reg Rn, Reg Rm) {
        bool b64Bit = Is64Bit(Rd);

        Rd = DecodeReg(Rd);
        Rm = DecodeReg(Rm);
        Rn = DecodeReg(Rn);
        Write32((b64Bit << 31) | (op << 30) | (flags << 29) | (0xD0 << 21) | (Rm << 16) | (Rn << 5) |
                Rd);
    }

    void XInstrEmitter::EncodeCondCompareImmInst(u32 op, Reg Rn, u32 imm, u32 nzcv, CCFlags cond) {
        bool b64Bit = Is64Bit(Rn);

        ASSERT_MSG(!(imm & ~0x1F), "%s: too large immediate: %d", __func__, imm);
        ASSERT_MSG(!(nzcv & ~0xF), "%s: Flags out of range: %d", __func__, nzcv);

        Rn = DecodeReg(Rn);
        Write32((b64Bit << 31) | (op << 30) | (1 << 29) | (0xD2 << 21) | (imm << 16) | (cond << 12) |
                (1 << 11) | (Rn << 5) | nzcv);
    }

    void XInstrEmitter::EncodeCondCompareRegInst(u32 op, Reg Rn, Reg Rm, u32 nzcv, CCFlags cond) {
        bool b64Bit = Is64Bit(Rm);

        ASSERT_MSG(!(nzcv & ~0xF), "%s: Flags out of range: %d", __func__, nzcv);

        Rm = DecodeReg(Rm);
        Rn = DecodeReg(Rn);
        Write32((b64Bit << 31) | (op << 30) | (1 << 29) | (0xD2 << 21) | (Rm << 16) | (cond << 12) |
                (Rn << 5) | nzcv);
    }

    void XInstrEmitter::EncodeCondSelectInst(u32 instenc, Reg Rd, Reg Rn, Reg Rm, CCFlags cond) {
        bool b64Bit = Is64Bit(Rd);

        Rd = DecodeReg(Rd);
        Rm = DecodeReg(Rm);
        Rn = DecodeReg(Rn);
        Write32((b64Bit << 31) | (CondSelectEnc[instenc][0] << 30) | (0xD4 << 21) | (Rm << 16) |
                (cond << 12) | (CondSelectEnc[instenc][1] << 10) | (Rn << 5) | Rd);
    }


    void XInstrEmitter::ADD(Reg Rd, Reg Rn, Reg Rm) {
        ADD(Rd, Rn, Rm, ArithOption(Rd, ST_LSL, 0));
    }

    void XInstrEmitter::ADD(Reg Rd, Reg Rn, Reg Rm,
                                                  Dynarmic::BackendA64::ArithOption Option) {
        EncodeArithmeticInst(0, false, Rd, Rn, Rm, Option);
    }

    void XInstrEmitter::ADDS(Reg Rd, Reg Rn, Reg Rm) {
        EncodeArithmeticInst(0, true, Rd, Rn, Rm, ArithOption(Rd, ST_LSL, 0));
    }

    void XInstrEmitter::ADDS(Reg Rd, Reg Rn, Reg Rm,
                                                   Dynarmic::BackendA64::ArithOption Option) {
        EncodeArithmeticInst(0, true, Rd, Rn, Rm, Option);
    }

    void XInstrEmitter::SUB(Reg Rd, Reg Rn, Reg Rm) {
        SUB(Rd, Rn, Rm, ArithOption(Rd, ST_LSL, 0));
    }

    void XInstrEmitter::SUB(Reg Rd, Reg Rn, Reg Rm,
                                                  Dynarmic::BackendA64::ArithOption Option) {
        EncodeArithmeticInst(1, false, Rd, Rn, Rm, Option);
    }

    void XInstrEmitter::SUBS(Reg Rd, Reg Rn, Reg Rm) {
        EncodeArithmeticInst(1, true, Rd, Rn, Rm, ArithOption(Rd, ST_LSL, 0));
    }

    void XInstrEmitter::SUBS(Reg Rd, Reg Rn, Reg Rm,
                                                   Dynarmic::BackendA64::ArithOption Option) {
        EncodeArithmeticInst(1, true, Rd, Rn, Rm, Option);
    }

    void XInstrEmitter::CMN(Reg Rn, Reg Rm) {
        CMN(Rn, Rm, ArithOption(Rn, ST_LSL, 0));
    }

    void XInstrEmitter::CMN(Reg Rn, Reg Rm, Dynarmic::BackendA64::ArithOption Option) {
        EncodeArithmeticInst(0, true, Is64Bit(Rn) ? ZR : WZR, Rn, Rm, Option);
    }

    void XInstrEmitter::CMP(Reg Rn, Reg Rm) {
        CMP(Rn, Rm, ArithOption(Rn, ST_LSL, 0));
    }

    void XInstrEmitter::CMP(Reg Rn, Reg Rm, Dynarmic::BackendA64::ArithOption Option) {
        EncodeArithmeticInst(1, true, Is64Bit(Rn) ? ZR : WZR, Rn, Rm, Option);
    }

    void XInstrEmitter::Write32(u32 value) {
        std::memcpy(m_code, &value, sizeof(u32));
        m_code += sizeof(u32);
    }

    void XInstrEmitter::ADC(Reg Rd, Reg Rn, Reg Rm) {
        EncodeArithmeticCarryInst(0, false, Rd, Rn, Rm);
    }

    void XInstrEmitter::ADCS(Reg Rd, Reg Rn, Reg Rm) {
        EncodeArithmeticCarryInst(0, true, Rd, Rn, Rm);
    }

    void XInstrEmitter::SBC(Reg Rd, Reg Rn, Reg Rm) {
        EncodeArithmeticCarryInst(1, false, Rd, Rn, Rm);
    }

    void XInstrEmitter::SBCS(Reg Rd, Reg Rn, Reg Rm) {
        EncodeArithmeticCarryInst(1, true, Rd, Rn, Rm);
    }

    void XInstrEmitter::CCMN(Reg Rn, u32 imm, u32 nzcv, CCFlags cond) {
        EncodeCondCompareImmInst(0, Rn, imm, nzcv, cond);
    }

    void XInstrEmitter::CCMP(Reg Rn, u32 imm, u32 nzcv, CCFlags cond) {
        EncodeCondCompareImmInst(1, Rn, imm, nzcv, cond);
    }

    void XInstrEmitter::CCMN(Reg Rn, Reg Rm, u32 nzcv, CCFlags cond) {
        EncodeCondCompareRegInst(0, Rn, Rm, nzcv, cond);
    }

    void XInstrEmitter::CCMP(Reg Rn, Reg Rm, u32 nzcv, CCFlags cond) {
        EncodeCondCompareRegInst(1, Rn, Rm, nzcv, cond);
    }

    void XInstrEmitter::CSEL(Reg Rd, Reg Rn, Reg Rm, CCFlags cond) {
        EncodeCondSelectInst(0, Rd, Rn, Rm, cond);
    }

    void XInstrEmitter::CSINC(Reg Rd, Reg Rn, Reg Rm, CCFlags cond) {
        EncodeCondSelectInst(1, Rd, Rn, Rm, cond);
    }

    void XInstrEmitter::CSINV(Reg Rd, Reg Rn, Reg Rm, CCFlags cond) {
        EncodeCondSelectInst(2, Rd, Rn, Rm, cond);
    }

    void XInstrEmitter::CSNEG(Reg Rd, Reg Rn, Reg Rm, CCFlags cond) {
        EncodeCondSelectInst(3, Rd, Rn, Rm, cond);
    }

    void XInstrEmitter::RBIT(Reg Rd, Reg Rn) {

    }

    void XInstrEmitter::REV16(Reg Rd, Reg Rn) {

    }

    void XInstrEmitter::REV32(Reg Rd, Reg Rn) {

    }

    void XInstrEmitter::REV64(Reg Rd, Reg Rn) {

    }

    void XInstrEmitter::CLZ(Reg Rd, Reg Rn) {

    }

    void XInstrEmitter::CLS(Reg Rd, Reg Rn) {

    }

    void XInstrEmitter::UDIV(Reg Rd, Reg Rn, Reg Rm) {

    }

    void XInstrEmitter::SDIV(Reg Rd, Reg Rn, Reg Rm) {

    }

    void XInstrEmitter::LSLV(Reg Rd, Reg Rn, Reg Rm) {

    }

    void XInstrEmitter::LSRV(Reg Rd, Reg Rn, Reg Rm) {

    }

    void XInstrEmitter::ASRV(Reg Rd, Reg Rn, Reg Rm) {

    }


};
