
#include "y/utest/run.hpp"
#include "y/information/entropic/alphabet.hpp"
#include "y/information/entropic/model/huffman.hpp"




using namespace Yttrium;
using namespace Information;


#include "y/string.hpp"

Y_UTEST(info_pack)
{

    Entropic::Alphabet multiplex(true);
    Entropic::Alphabet blockwise(false);

    multiplex.display(std::cerr);
    blockwise.display(std::cerr);


}
Y_UDONE()
