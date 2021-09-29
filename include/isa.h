#ifndef ISA_H
#define ISA_H

#include "common.h"

// qemu registers
typedef union {
    struct {
        uint64_t zero, ra, sp, gp, tp, t0, t1, t2, fp,
                s1, a0, a1, a2, a3, a4, a5, a6, a7,
                s2, s3, s4, s5, s6, s7, s8, s9, s10,
                s11, t3, t4, t5, t6;
        uint64_t pc;
    };
    struct {
        uint64_t array[40];
    };
    struct {
        uint64_t gpr[32];
    };
} qemu_regs_t;


// instructions
typedef union {
    uint32_t val;

    // R-type
    // struct {
    //     uint32_t opcode: 7;
    //     uint32_t funct7: 7;
    //     uint32_t rs2: 5;
    //     uint32_t rs1: 5;
    //     uint32_t funct3: 3;
    //     uint32_t rd: 5;
    // } r_inst_t;

    // I-type
    struct{
        uint32_t opcode: 7;
        uint32_t rd: 5;
        uint32_t rs1: 5;
        uint32_t imm: 12;
        uint32_t funct3: 3;
    } i_inst_t;

    // S-type
    // struct {
    //     uint32_t opcode: 7;
    //     uint32_t imm7: 7;
    //     uint32_t rs2: 5;
    //     uint32_t rs1: 5;
    //     uint32_t funct3: 3;
    //     uint32_t imm5: 5;
    // } s_inst_t;

    // TODO more type
} inst_t; // Instruction



int inst_is_load(inst_t inst);

int inst_is_load_uart(inst_t inst, qemu_regs_t *regs);

#endif