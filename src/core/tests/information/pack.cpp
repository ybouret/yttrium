
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
    StreamBits         io;

    multiplex.display(std::cerr);
    blockwise.display(std::cerr);

    const String data = "a banana";
    for(size_t i=1;i<=data.size();++i)
    {
        multiplex.write(io, data[i]);
        multiplex.display(std::cerr);
    }

    Y_SIZEOF(Entropic::Alphabet);
    Y_SIZEOF(Entropic::Huffman);


}
Y_UDONE()
