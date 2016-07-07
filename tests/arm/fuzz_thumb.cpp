/* This file is part of the dynarmic project.
 * Copyright (c) 2016 MerryMage
 * This software may be used and distributed according to the terms of the GNU
 * General Public License version 2 or any later version.
 */

#include <catch.hpp>

#include "common/common_types.h"
#include "interface/interface.h"
#include "skyeye_interpreter/dyncom/arm_dyncom_interpreter.h"
#include "skyeye_interpreter/skyeye_common/armstate.h"

std::array<u16, 1024> code_mem{};

u32 MemoryRead32(u32 vaddr);
void InterpreterFallback(u32 pc, Dynarmic::Jit* jit);
Dynarmic::UserCallbacks GetUserCallbacks();

u32 MemoryRead32(u32 vaddr) {
    if (vaddr < code_mem.size() * sizeof(u16)) {
        size_t index = vaddr / sizeof(u16);
        return code_mem[index] | (code_mem[index+1] << 16);
    }
    return vaddr;
}

void InterpreterFallback(u32 pc, Dynarmic::Jit* jit) {
    ARMul_State interp_state{USER32MODE};
    interp_state.user_callbacks = GetUserCallbacks();
    interp_state.NumInstrsToExecute = 1;

    interp_state.Reg = jit->Regs();
    interp_state.Cpsr = jit->Cpsr();
    interp_state.Reg[15] = pc;

    InterpreterClearCache();
    InterpreterMainLoop(&interp_state);

    jit->Regs() = interp_state.Reg;
    jit->Cpsr() = interp_state.Cpsr;
}

Dynarmic::UserCallbacks GetUserCallbacks() {
    Dynarmic::UserCallbacks user_callbacks{};
    user_callbacks.MemoryRead32 = &MemoryRead32;
    user_callbacks.InterpreterFallback = &InterpreterFallback;
    return user_callbacks;
}
