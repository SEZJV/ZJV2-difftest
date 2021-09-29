#ifndef ISA_H
#define ISA_H

// instructions
typedef union {
    uint32_t val;

    // R-type
    struct {
        uint32_t funct7: 7;
        uint32_t rs2: 5;
        uint32_t rs1: 5;
        uint32_t funct3: 3;
        uint32_t rd: 5;
        uint32_t opcode: 7;
    } r_inst_t;

    // I-type
    struct{
        uint32_t imm: 12;
        uint32_t rs1: 5;
        uint32_t funct3: 3;
        uint32_t rd: 5;
        uint32_t opcode: 7;
    } i_inst_t;

    // S-type
    struct {
        uint32_t imm7: 7;
        uint32_t rs2: 5;
        uint32_t rs1: 5;
        uint32_t funct3: 3;
        uint32_t imm5: 5;
        uint32_t opcode: 7;
    } s_inst_t;

    // TODO more type
} inst_t; // Instruction



#endif