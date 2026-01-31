#ifndef RISCV_ENCODER_SYMTAB_H
#define RISCV_ENCODER_SYMTAB_H

namespace fusion = boost::fusion;
namespace spirit = boost::spirit;
namespace hana = boost::hana;
namespace ascii = x3::ascii;

using namespace std::literals;
using x3::int_;
using x3::char_;
using x3::phrase_parse;

#define INIT(STR, NUM) (#STR, NUM)

inline struct table_m : x3::symbols<unsigned> {
    table_m() {
        add INIT ( add, 0 ) INIT ( addi, 12 ) INIT ( and, 1 ) INIT ( andi, 13 ) INIT ( xor, 2 ) INIT ( xori, 15 )
        INIT ( or, 5 ) INIT ( ori, 14 ) INIT ( slt, 6 ) INIT ( sltu, 7 ) INIT ( sra, 11 ) INIT ( sll, 3 )
        INIT ( srl, 4 ) INIT ( lb, 16 ) INIT ( lh, 17 ) INIT ( lw, 18 ) INIT ( lbu, 19 ) INIT ( lhu, 20 ) INIT ( sb, 21 ) INIT ( sh, 22 ) INIT ( sw, 23 )
        INIT ( slli, 8 ) INIT ( srli, 9 ) INIT ( srai, 10 ) INIT ( jalr, 24 );
    }
} mnemonic;

#endif // RISCV_ENCODER_SYMTAB_H
