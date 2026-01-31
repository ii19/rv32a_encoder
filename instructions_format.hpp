#ifndef RISCV_ENCODER_INSTRUCTIONS_FORMAT_H
#define RISCV_ENCODER_INSTRUCTIONS_FORMAT_H

struct instr_r
{
    unsigned opcode;
    unsigned rd;
    unsigned rs1;
    unsigned rs2;
};

struct instr_i
{
    unsigned opcode;
    unsigned rd;
    unsigned rs1;
    unsigned imm;
};

struct instr_s
{
    unsigned imm;
    unsigned xs2;
    unsigned xs1;
    unsigned imm2;
};

BOOST_FUSION_ADAPT_STRUCT(instr_r, opcode, rd, rs1, rs2);

BOOST_FUSION_ADAPT_STRUCT(instr_i, opcode, rd, rs1, imm);

BOOST_FUSION_ADAPT_STRUCT(instr_s, imm, xs2, xs1, imm2);

#endif // RISCV_ENCODER_INSTRUCTIONS_FORMAT_H
