
#include "y/utest/run.hpp"
#include "y/information/entropic/alphabet.hpp"
#include "y/information/entropic/model/huffman.hpp"

using namespace Yttrium;
using namespace Information;

#include "y/string.hpp"

Y_UTEST(info_pack)
{

    const char     Ch[] = { 'a', 'e', 'i', 'o', 'u' };
    const unsigned Nc[] = { 12, 42, 9, 30, 7 };

    Entropic::Alphabet multiplex(true);
    Entropic::Alphabet blockwise(false);
    Entropic::Huffman  huff;
    StreamBits         io;

    multiplex.display(std::cerr);
    blockwise.display(std::cerr);

    for(unsigned i=0;i<sizeof(Ch)/sizeof(Ch[0]);++i)
    {
        const char c = Ch[i];
        for(unsigned j=Nc[i];j>0;--j)
        {
            multiplex.write_(io,c);
            blockwise.write_(io,c);
        }
    }

    multiplex.display(std::cerr);
    huff.build(multiplex.used);
    multiplex.display(std::cerr);

    blockwise.display(std::cerr);
    huff.build(blockwise.used);
    blockwise.display(std::cerr);

    Y_SIZEOF(Entropic::Alphabet);
    Y_SIZEOF(Entropic::Huffman);


}
Y_UDONE()
