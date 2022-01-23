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
    regs->gpr[ 0] = dut->io_difftest_gprs_0;  // TODO
    regs->gpr[ 1] = dut->io_difftest_gprs_1;
    regs->gpr[ 2] = dut->io_difftest_gprs_2;
    regs->gpr[ 3] = dut->io_difftest_gprs_3;
    regs->gpr[ 4] = dut->io_difftest_gprs_4;
    regs->gpr[ 5] = dut->io_difftest_gprs_5;
    regs->gpr[ 6] = dut->io_difftest_gprs_6;
    regs->gpr[ 7] = dut->io_difftest_gprs_7;
    regs->gpr[ 8] = dut->io_difftest_gprs_8;
    regs->gpr[ 9] = dut->io_difftest_gprs_9;
    regs->gpr[10] = dut->io_difftest_gprs_10;
    regs->gpr[11] = dut->io_difftest_gprs_11;
    regs->gpr[12] = dut->io_difftest_gprs_12;
    regs->gpr[13] = dut->io_difftest_gprs_13;
    regs->gpr[14] = dut->io_difftest_gprs_14;
    regs->gpr[15] = dut->io_difftest_gprs_15;
    regs->gpr[16] = dut->io_difftest_gprs_16;
    regs->gpr[17] = dut->io_difftest_gprs_17;
    regs->gpr[18] = dut->io_difftest_gprs_18;
    regs->gpr[19] = dut->io_difftest_gprs_19;
    regs->gpr[20] = dut->io_difftest_gprs_20;
    regs->gpr[21] = dut->io_difftest_gprs_21;
    regs->gpr[22] = dut->io_difftest_gprs_22;
    regs->gpr[23] = dut->io_difftest_gprs_23;
    regs->gpr[24] = dut->io_difftest_gprs_24;
    regs->gpr[25] = dut->io_difftest_gprs_25;
    regs->gpr[26] = dut->io_difftest_gprs_26;
    regs->gpr[27] = dut->io_difftest_gprs_27;
    regs->gpr[28] = dut->io_difftest_gprs_28;
    regs->gpr[29] = dut->io_difftest_gprs_29;
    regs->gpr[30] = dut->io_difftest_gprs_30;
    regs->gpr[31] = dut->io_difftest_gprs_31;
    regs->fpr[33] = dut->io_difftest_fprs_0;
    regs->fpr[34] = dut->io_difftest_fprs_1;
    regs->fpr[35] = dut->io_difftest_fprs_2;
    regs->fpr[36] = dut->io_difftest_fprs_3;
    regs->fpr[37] = dut->io_difftest_fprs_4;
    regs->fpr[38] = dut->io_difftest_fprs_5;
    regs->fpr[39] = dut->io_difftest_fprs_6;
    regs->fpr[40] = dut->io_difftest_fprs_7;
    regs->fpr[41] = dut->io_difftest_fprs_8;
    regs->fpr[42] = dut->io_difftest_fprs_9;
    regs->fpr[43] = dut->io_difftest_fprs_10;
    regs->fpr[44] = dut->io_difftest_fprs_11;
    regs->fpr[45] = dut->io_difftest_fprs_12;
    regs->fpr[46] = dut->io_difftest_fprs_13;
    regs->fpr[47] = dut->io_difftest_fprs_14;
    regs->fpr[48] = dut->io_difftest_fprs_15;
    regs->fpr[49] = dut->io_difftest_fprs_16;
    regs->fpr[50] = dut->io_difftest_fprs_17;
    regs->fpr[51] = dut->io_difftest_fprs_18;
    regs->fpr[52] = dut->io_difftest_fprs_19;
    regs->fpr[53] = dut->io_difftest_fprs_20;
    regs->fpr[54] = dut->io_difftest_fprs_21;
    regs->fpr[55] = dut->io_difftest_fprs_22;
    regs->fpr[56] = dut->io_difftest_fprs_23;
    regs->fpr[57] = dut->io_difftest_fprs_24;
    regs->fpr[58] = dut->io_difftest_fprs_25;
    regs->fpr[59] = dut->io_difftest_fprs_26;
    regs->fpr[60] = dut->io_difftest_fprs_27;
    regs->fpr[61] = dut->io_difftest_fprs_28;
    regs->fpr[62] = dut->io_difftest_fprs_29;
    regs->fpr[63] = dut->io_difftest_fprs_30;
    regs->fpr[64] = dut->io_difftest_fprs_31;
    // m mode CSR
    regs->array[65] = dut->io_difftest_csrs_mstatus;
    regs->array[66] = dut->io_difftest_csrs_medeleg;
    regs->array[67] = dut->io_difftest_csrs_mideleg;
    regs->array[68] = dut->io_difftest_csrs_mie;
    regs->array[69] = dut->io_difftest_csrs_mip;
    regs->array[70] = dut->io_difftest_csrs_mtvec;
    regs->array[71] = dut->io_difftest_csrs_mscratch;
    regs->array[72] = dut->io_difftest_csrs_mepc;
    regs->array[73] = dut->io_difftest_csrs_mcause;
    regs->array[74] = dut->io_difftest_csrs_mtval;
    // s mode CSR
    regs->array[75] = dut->io_difftest_csrs_sstatus;
    regs->array[77] = dut->io_difftest_csrs_stvec;
    regs->array[78] = dut->io_difftest_csrs_sscratch;
    regs->array[79] = dut->io_difftest_csrs_sepc;
    regs->array[80] = dut->io_difftest_csrs_scause;
    regs->array[81] = dut->io_difftest_csrs_stval;
}

void dut_write_counter(int value) {
    // TODO have to write the GPR and the counter (timer interrupt), GPR for mfc0, counter for every step to keep up with qemu
}
