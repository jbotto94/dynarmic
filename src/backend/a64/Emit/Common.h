#include "common/assert.h"
#include "common/common_types.h"
#include "Registers.h"
#include "RegisterOps.h"
#pragma once

namespace Dynarmic::BackendA64 {
    enum CCFlags {
        CC_EQ = 0,     // Equal
        CC_NEQ,        // Not equal
        CC_CS,         // Carry Set
        CC_CC,         // Carry Clear
        CC_MI,         // Minus (Negative)
        CC_PL,         // Plus
        CC_VS,         // Overflow
        CC_VC,         // No Overflow
        CC_HI,         // Unsigned higher
        CC_LS,         // Unsigned lower or same
        CC_GE,         // Signed greater than or equal
        CC_LT,         // Signed less than
        CC_GT,         // Signed greater than
        CC_LE,         // Signed less than or equal
        CC_AL,         // Always (unconditional) 14
        CC_HS = CC_CS, // Alias of CC_CS  Unsigned higher or same
        CC_LO = CC_CC, // Alias of CC_CC  Unsigned lower
    };


    enum ShiftType {
        ST_LSL = 0,
        ST_LSR = 1,
        ST_ASR = 2,
        ST_ROR = 3,
    };


    class ArithOption {
    public:
        enum WidthSpecifier {
            WIDTH_DEFAULT,
            WIDTH_32BIT,
            WIDTH_64BIT,
        };

        enum ExtendSpecifier {
            EXTEND_UXTB = 0x0,
            EXTEND_UXTH = 0x1,
            EXTEND_UXTW = 0x2, /* Also LSL on 32bit width */
            EXTEND_UXTX = 0x3, /* Also LSL on 64bit width */
            EXTEND_SXTB = 0x4,
            EXTEND_SXTH = 0x5,
            EXTEND_SXTW = 0x6,
            EXTEND_SXTX = 0x7,
        };

        enum TypeSpecifier {
            TYPE_EXTENDEDREG,
            TYPE_IMM,
            TYPE_SHIFTEDREG,
        };

    private:
        Reg m_destReg;
        WidthSpecifier m_width;
        ExtendSpecifier m_extend;
        TypeSpecifier m_type;
        ShiftType m_shifttype;
        u32 m_shift;

    public:
        ArithOption(Reg Rd, bool index = false) {
            // Indexed registers are a certain feature of AARch64
            // On Loadstore instructions that use a register offset
            // We can have the register as an index
            // If we are indexing then the offset register will
            // be shifted to the left so we are indexing at intervals
            // of the size of what we are loading
            // 8-bit: Index does nothing
            // 16-bit: Index LSL 1
            // 32-bit: Index LSL 2
            // 64-bit: Index LSL 3
            if (index)
                m_shift = 4;
            else
                m_shift = 0;

            m_destReg = Rd;
            m_type = TYPE_EXTENDEDREG;
            if (Is64Bit(Rd)) {
                m_width = WIDTH_64BIT;
                m_extend = EXTEND_UXTX;
            } else {
                m_width = WIDTH_32BIT;
                m_extend = EXTEND_UXTW;
            }
            m_shifttype = ST_LSL;
        }
        ArithOption(Reg Rd, ShiftType shift_type, u32 shift) {
            m_destReg = Rd;
            m_shift = shift;
            m_shifttype = shift_type;
            m_type = TYPE_SHIFTEDREG;
            if (Is64Bit(Rd)) {
                m_width = WIDTH_64BIT;
                if (shift == 64)
                    m_shift = 0;
            } else {
                m_width = WIDTH_32BIT;
                if (shift == 32)
                    m_shift = 0;
            }
        }
        TypeSpecifier GetType() const {
            return m_type;
        }
        Reg GetReg() const {
            return m_destReg;
        }
        u32 GetData() const {
            switch (m_type) {
                case TYPE_EXTENDEDREG:
                    return (m_extend << 13) | (m_shift << 10);
                    break;
                case TYPE_SHIFTEDREG:
                    return (m_shifttype << 22) | (m_shift << 10);
                    break;
                default:
                    DEBUG_ASSERT_MSG(false, "Invalid type in GetData");
                    break;
            }
            return 0;
        }
    };

    struct FixupBranch {
        u8* ptr;
        // Type defines
        // 0 = CBZ (32bit)
        // 1 = CBNZ (32bit)
        // 2 = B (conditional)
        // 3 = TBZ
        // 4 = TBNZ
        // 5 = B (unconditional)
        // 6 = BL (unconditional)
        u32 type;

        // Used with B.cond
        CCFlags cond;

        // Used with TBZ/TBNZ
        u8 bit;

        // Used with Test/Compare and Branch
        Reg reg;
    };

    static const u32 ArithEnc[] = {
            0x058, // ADD
            0x258, // SUB
    };



} // namespace Dynarmic::BackendA64

