#include "isa.hpp"

auto rfile ( auto name ) {
    std::ifstream file{name, std::ifstream::in};

    file.seekg ( 0, std::ios::end );
    auto size = file.tellg();
    file.seekg ( 0, std::ios::beg );

    std::vector<char> buffer ( size );
    file.read ( buffer.data(), size );
    file.close();

    return buffer;
}



int main() {
    std::string path;
    std::cin >> path;
    auto input = rfile ( path );
    auto begin = input.begin();
    auto end = input.end();
    std::variant<instr_r, instr_i, instr_s> v;
    for ( auto it = 0; begin != end; ++it ) {
        phrase_parse ( begin, end, parser, x3::ascii::space, v );
        std::visit ( encode, isa::f[std::visit ( instr_binding_V<0>, v )], v );
        std::cout << isa::machine[it] << "\n";
    }
}

