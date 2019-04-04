#include "Registers.h"
#pragma once


namespace Dynarmic::BackendA64 {

    constexpr bool Is64Bit(Reg reg) {
        return (reg & 0x20) != 0;
    }

    constexpr bool IsSingle(Reg reg) {
        return (reg & 0xC0) == 0x40;
    }

    constexpr bool IsDouble(Reg reg) {
        return (reg & 0xC0) == 0x80;
    }

    constexpr bool IsScalar(Reg reg) {
        return IsSingle(reg) || IsDouble(reg);
    }

    constexpr bool IsQuad(Reg reg) {
        return (reg & 0xC0) == 0xC0;
    }

    constexpr bool IsVector(Reg reg) {
        return (reg & 0xC0) != 0;
    }

    constexpr bool IsGPR(Reg reg) {
        return static_cast<int>(reg) < 0x40;
    }

    constexpr Reg DecodeReg(Reg reg) {
        return static_cast<Reg>(reg & 0x1F);
    }

    constexpr Reg EncodeRegTo64(Reg reg) {
        return static_cast<Reg>(reg | 0x20);
    }

    constexpr Reg EncodeRegToSingle(Reg reg) {
        return static_cast<Reg>(DecodeReg(reg) + S0);
    }

    constexpr Reg EncodeRegToDouble(Reg reg) {
        return static_cast<Reg>((reg & ~0xC0) | 0x80);
    }

    constexpr Reg EncodeRegToQuad(Reg reg) {
        return static_cast<Reg>(reg | 0xC0);
    }
}