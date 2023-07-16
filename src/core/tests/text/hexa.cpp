


#include "y/text/hexadecimal.hpp"
#include "y/utest/run.hpp"
#include "y/random/bits.hpp"

#include <cstring>

using namespace Yttrium;


Y_UTEST(text_hexa)
{
    Random::Rand ran;
    Y_USHOW(Hexadecimal::CharsPerByte);
    Y_USHOW(Hexadecimal::CharsPerWord);
    Y_USHOW(Hexadecimal::Required);
    Y_USHOW(Hexadecimal::DataSize);

    uint64_t x = 0xbcdef01;

    std::cerr << Hexadecimal(x,Hexadecimal::Default,Hexadecimal::LowerCase) << std::endl;
    std::cerr << Hexadecimal(x,Hexadecimal::Default,Hexadecimal::UpperCase) << std::endl;

    std::cerr << Hexadecimal(x,Hexadecimal::Compact,Hexadecimal::LowerCase) << std::endl;
    std::cerr << Hexadecimal(x,Hexadecimal::Compact,Hexadecimal::UpperCase) << std::endl;


}
Y_UDONE()
