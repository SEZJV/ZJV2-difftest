#ifndef DUT_H
#define DUT_H

#include <stdbool.h>
#include "qemu.h"
#include "VTileForVerilator.h"
#include "verilated_vcd_c.h"

typedef struct {
    uint64_t mycpu_pcs[3];
} diff_pcs;

extern VTileForVerilator *dut;

// TODO sync cycle and sync interrupt
void dut_reset(int cycle, VerilatedVcdC *vfp, VerilatedContext *context);  // reset processor and initialize memory
int dut_commit();  // if the processor has new commit
void dut_step(int cycle, VerilatedVcdC *vfp, VerilatedContext *context);
void dut_getregs(qemu_regs_t *regs);
void dut_write_counter(int value);
void dut_getpcs(diff_pcs *pcs);
void dut_sync_reg(int saddr, int svalue, bool sync);

#endif