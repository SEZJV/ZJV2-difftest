#include "dut.h"
#include <iostream>

VTileForVerilator *dut;

void dut_reset(int cycle, VerilatedVcdC *vfp, VerilatedContext *context) {
    for (int i = 0; i < cycle; i++) {
        dut->reset = 1;
        dut->clock = 0;
        dut->eval();
        context->timeInc(1);
        vfp->dump(context->time());
        dut->clock = 1;
        dut->eval();
        dut->reset = 0;
        context->timeInc(1);
        vfp->dump(context->time());
    }
}

int dut_commit() {
    int commit = (dut->io_difftest_valids_0 != 0) + (dut->io_difftest_valids_1 != 0) + (dut->io_difftest_valids_2 != 0);

#ifdef TRACE
    std::cout << "DUT commits " << commit << " instructions" << std::endl;
#endif
    
    return commit;
}

void dut_step(int cycle, VerilatedVcdC *vfp, VerilatedContext *context) {

#ifdef TRACE
    std::cout << "DUT one cycle" << std::endl;
#endif

    for (int i = 0; i < cycle; i++) {
        dut->clock = 0;
        dut->eval();
        context->timeInc(1);
        vfp->dump(context->time());
        dut->clock = 1;
        dut->eval();
        context->timeInc(1);
        vfp->dump(context->time());
    }
}

void dut_getpcs(diff_pcs *pcs) {
    pcs->mycpu_pcs[0] = dut->io_difftest_pcs_0;
    pcs->mycpu_pcs[1] = dut->io_difftest_pcs_1;
    pcs->mycpu_pcs[2] = dut->io_difftest_pcs_2;
}

void dut_sync_reg(int saddr, int svalue, bool sync) {
    dut->io_difftest_sync = sync;
    dut->io_difftest_sval = svalue;
    dut->io_difftest_saddr = saddr;
    if (sync) {
        printf("[SYNC] sval: %08lx | saddr: %08lx\n", dut->io_difftest_sval, dut->io_difftest_saddr);
    }
}

void dut_getregs(qemu_regs_t *regs) {
    regs->gpr[ 0] = dut->io_difftest_regs_0;  // TODO
    regs->gpr[ 1] = dut->io_difftest_regs_1;
    regs->gpr[ 2] = dut->io_difftest_regs_2;
    regs->gpr[ 3] = dut->io_difftest_regs_3;
    regs->gpr[ 4] = dut->io_difftest_regs_4;
    regs->gpr[ 5] = dut->io_difftest_regs_5;
    regs->gpr[ 6] = dut->io_difftest_regs_6;
    regs->gpr[ 7] = dut->io_difftest_regs_7;
    regs->gpr[ 8] = dut->io_difftest_regs_8;
    regs->gpr[ 9] = dut->io_difftest_regs_9;
    regs->gpr[10] = dut->io_difftest_regs_10;
    regs->gpr[11] = dut->io_difftest_regs_11;
    regs->gpr[12] = dut->io_difftest_regs_12;
    regs->gpr[13] = dut->io_difftest_regs_13;
    regs->gpr[14] = dut->io_difftest_regs_14;
    regs->gpr[15] = dut->io_difftest_regs_15;
    regs->gpr[16] = dut->io_difftest_regs_16;
    regs->gpr[17] = dut->io_difftest_regs_17;
    regs->gpr[18] = dut->io_difftest_regs_18;
    regs->gpr[19] = dut->io_difftest_regs_19;
    regs->gpr[20] = dut->io_difftest_regs_20;
    regs->gpr[21] = dut->io_difftest_regs_21;
    regs->gpr[22] = dut->io_difftest_regs_22;
    regs->gpr[23] = dut->io_difftest_regs_23;
    regs->gpr[24] = dut->io_difftest_regs_24;
    regs->gpr[25] = dut->io_difftest_regs_25;
    regs->gpr[26] = dut->io_difftest_regs_26;
    regs->gpr[27] = dut->io_difftest_regs_27;
    regs->gpr[28] = dut->io_difftest_regs_28;
    regs->gpr[29] = dut->io_difftest_regs_29;
    regs->gpr[30] = dut->io_difftest_regs_30;
    regs->gpr[31] = dut->io_difftest_regs_31;
}

void dut_write_counter(int value) {
    // TODO have to write the GPR and the counter (timer interrupt), GPR for mfc0, counter for every step to keep up with qemu
}
