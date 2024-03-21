
#include "y/utest/run.hpp"
#include "y/information/entropic/alphabet.hpp"
#include "y/information/entropic/model/huffman.hpp"

using namespace Yttrium;
using namespace Information;

#include "y/string.hpp"

Y_UTEST(info_pack)
{

    const char     Ch[] = { 'a', 'e', 'i', 'o', 'u' };
    const unsigned Nc[] = { 12,   42,   9,  30,  7  };

    Entropic::Alphabet alpha(Entropic::Alphabet::SingleStream);

    alpha.display(std::cerr);



}
Y_UDONE()
