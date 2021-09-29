#include "isa.h"

int inst_is_load(inst_t inst) {
    return inst.i_inst_t.opcode == 0x3;
}


int inst_is_load_uart(inst_t inst, qemu_regs_t *regs) {
    if (inst_is_load(inst)) {
        uint32_t rs1 = inst.i_inst_t.rs1;
        uint64_t addr = inst.i_inst_t.imm + regs->gpr[rs1];
        printf("[DEBUG] load addr: 0x%016lx\n", addr);
        return 1;
    } else {
        return 0;
    }
}
