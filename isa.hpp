#ifndef __ISA__
#define __ISA__

#include "header.hpp"
#include "helper.hpp"
#include "instructions_format.hpp"
#include "symtab.hpp"

static int pc{};

auto incpc() {
    pc += 4;
}

//xN
static auto rule_reg = as<unsigned> ( "rule_reg" ) = 'x' >> uint5;

// add x0, x2, x1
static auto parser_r = as<instr_r> ( "parser_r" ) = mnemonic >> x3::as_parser ( rule_reg ) >> ','
                       >> x3::as_parser ( rule_reg ) >> ','
                       >> x3::as_parser ( rule_reg );
// addi x0, x2, 1
static auto parser_i = as<instr_i> ( "second_parser_i" ) = mnemonic >> rule_reg >> ','
                       >> rule_reg >> ',' >> int_;
// lb x2, 2(x5)
static auto second_parser_i = as<instr_i> ( "parser_i" ) = mnemonic >> x3::as_parser ( rule_reg ) >> ','
                              >> int_ >> '(' >> x3::as_parser ( rule_reg )
                              >> ')';
// sb x2, 2(x5)
static auto parser_s = as<instr_s> ( "parser_s" ) = mnemonic >> x3::as_parser ( rule_reg ) >> ','
                       >> int_ >> '(' >> x3::as_parser ( rule_reg ) >> ')';

static auto parser = as<std::variant<instr_r, instr_i, instr_s>> ( "parser" ) = ( parser_r | parser_i
                     | second_parser_i
                     | parser_s );

static auto rule_label = x3::rule<class rule_label> {} = (
        * ( ~char_ ( ':' ) ) >> ":" );

namespace isa {
using std::array;
using std::tuple;
using std::variant;

inline std::vector<uint32_t> machine ( 16384 );
inline auto ctxt = []() {
    static auto index = 0;
    index++;
    return &machine[index - 1];
};

constexpr auto f =
array<variant<decltype ( ADD ), decltype ( ADDI ), decltype ( SLLI ) >, 25>{
    ADD, AND, XOR, SLL, SRL, OR, SLT, SLTU, SLLI, SRLI, SRAI, SRA, ADDI,
    ANDI, ORI, XORI, LB, LH, LW, LBU, LHU, SB, SH, SW, JALR
};
}

template <typename X1, typename X2, size_t... N>
constexpr auto fold ( std::index_sequence<N...> I, auto &&f, X1 &&t, X2 &&t1 ) {
    ( f ( std::get<N> ( t ), std::get<N> ( t1 ) ), ... );
}

constexpr auto zip_with ( auto &&f, auto &&... xs ) {
    auto a = std::get<0> ( std::tuple{xs...} );
    constexpr auto size = std::tuple_size_v<decltype ( a ) >;
    fold ( std::make_index_sequence<size> {}, f, xs... );
}

constexpr auto encode = overload {
    []<typename T, typename U>
    requires ( ( same<T, decltype ( SRAI ) > && same<U, instr_i> ) ||
               ( same<T, decltype ( ADD ) > && same<U, instr_r> ) )
    ( T x, U i ) {
        SHIFT ( std::tuple{25, 12, 0}, std::tuple{7, 15, 20} );
        auto addr = isa::ctxt();
        auto c = instr_binding_V<1> ( i );

        auto encode = [&] ( auto &&v, auto &&s ) {
            *addr |= v << s;
        };

        zip_with ( encode, x, std::get<0> ( shift ) );
        zip_with ( encode, c, std::get<1> ( shift ) );
    },
    []<typename T, typename U>
    requires ( same<T, decltype ( ADDI ) > && same<U, instr_i> )
    ( T x, U i ) {
        SHIFT ( std::tuple{12, 0}, std::tuple{7, 15, 20} );
        auto addr = isa::ctxt();
        auto c = instr_binding_V<1> ( i );

        auto encode = [&] ( auto &&v, auto &&s ) {
            *addr |= v << s;
        };

        zip_with ( encode, x, std::get<0> ( shift ) );
        zip_with ( encode, c, std::get<1> ( shift ) );
    },
    []<typename T, typename U>
    requires ( same<T, decltype ( SW ) > && same<U, instr_s> )
    ( T x, U i ) {
        SHIFT ( std::tuple{12, 0}, std::tuple{20, 15} );
        auto addr = isa::ctxt();
        auto c = instr_binding_V<1> ( i );
        auto &tmp = std::get<1> ( c );
        if ( tmp & 0b1111'111'111 ) {
            *addr |= tmp & 0b11111;
            *addr <<= 7;
            *addr |= ( ( tmp >> 5 ) & 0b1111111 ) << 25;
        } else {
            throw std::runtime_error ( "" );
        }

        auto mod = std::tuple{i.xs2, i.imm2};;

        auto encode = [&] ( auto &&v, auto &&s ) {
            *addr |= v << s;
        };

        zip_with ( encode, x, std::get<0> ( shift ) );
        zip_with ( encode, mod, std::get<1> ( shift ) );
    },
    [] ( auto, auto ) {
    },
};

#endif
