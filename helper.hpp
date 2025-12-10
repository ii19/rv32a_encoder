#ifndef __HELPER__RISCV_ENCODER
#define __HELPER__RISCV_ENCODER

#include "header.hpp"

#define ENCODE_R(MNEMONIC, FUNCT7, FUNCT3, OPCODE) \
    constexpr auto MNEMONIC = std::tuple{FUNCT7, FUNCT3, OPCODE};

#define ENCODE_I(MNEMONIC, FUNCT3, OPCODE) constexpr auto MNEMONIC = std::tuple{FUNCT3, OPCODE};

#define ENCODE_ALTI(MNEMONIC, FUNCT7, FUNCT3, OPCODE) \
    constexpr auto MNEMONIC = alt_i{FUNCT7, FUNCT3, OPCODE};

#define ENCODE_S(MNEMONIC, FUNCT3, OPCODE) constexpr auto MNEMONIC = std::tuple{FUNCT3, OPCODE};

#define SHIFT(args...) constexpr auto shift = std::tuple{args};

template<typename T, typename U>
concept same = std::is_same_v<T, std::remove_cv_t<U>>;

namespace x3 = boost::spirit::x3;
template<typename T>

auto as = [](auto s) { return x3::rule<class tag, T>{s}; };

template<typename... xs>
struct overload : xs...
{
    using xs::operator()...;
};

template<size_t T>
struct dcd_instr
{
    constexpr auto operator()(auto &&instr)
    {
        auto [opcode, ... x] = instr;
        if constexpr (T) {
            return std::tuple{x...};
        } else
            return opcode;
    }
};

template<size_t T>
inline auto dcd_instr_T = dcd_instr<T>{};

template<typename... xs>
struct alt_i : std::tuple<xs...>
{
    constexpr alt_i(xs... x)
        : std::tuple<xs...>(x...)
    {}
};

template<typename... xs>
struct std::tuple_size<alt_i<xs...>> : std::tuple_size<std::tuple<xs...>>
{};

static_assert(std::tuple_size<alt_i<int, int>>::value == 2);
static_assert(std::tuple_size<std::tuple<int, int>>::value == 2);
static_assert(std::tuple_size<alt_i<int, int, long, char>>::value == 4);
static_assert(std::tuple_size<std::tuple<int, int, char *, void *>>::value == 4);

ENCODE_R(ADD, 0b0000000, 0b000, 0b0110011);
ENCODE_R(AND, 0b0000000, 0b111, 0b0110011);
ENCODE_R(XOR, 0b0000000, 0b100, 0b0110011);
ENCODE_R(SLL, 0b0000000, 0b001, 0b0110011);
ENCODE_R(SRL, 0b0000000, 0b101, 0b0110011);
ENCODE_R(OR, 0b0000000, 0b110, 0b0110011);
ENCODE_R(SLT, 0b0000000, 0b010, 0b0110011);
ENCODE_R(SLTU, 0b0000000, 0b011, 0b0110011);
ENCODE_R(SRA, 0b0100000, 0b011, 0b0110011);

ENCODE_I(ADDI, 0b000, 0b0010011);
ENCODE_I(JALR, 0b000, 0b01100011);
ENCODE_I(ANDI, 0b111, 0010011);
ENCODE_I(ORI, 0b110, 0b0010011);
ENCODE_I(XORI, 0b100, 0b0010011);
ENCODE_I(LB, 0b000, 0b0000011);
ENCODE_I(LH, 0b001, 0b0000011);
ENCODE_I(LW, 0b010, 0b0000011);
ENCODE_I(LBU, 0b100, 0b0000011);
ENCODE_I(LHU, 0b101, 0b0000011);

ENCODE_S(SB, 0b000, 0b0100011);
ENCODE_S(SH, 0b001, 0b0100011);
ENCODE_S(SW, 0b010, 0b0100011);

ENCODE_ALTI(SLLI, 0b0000000, 0b001, 0b0010011);
ENCODE_ALTI(SRLI, 0b0000000, 0b101, 0b0010011);
ENCODE_ALTI(SRAI, 0b0100000, 0b101, 0b0010011);

#endif
