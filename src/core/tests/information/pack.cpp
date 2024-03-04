
#include "y/utest/run.hpp"
#include "y/information/entropic/alphabet.hpp"
#include "y/information/entropic/model/huffman.hpp"




using namespace Yttrium;
using namespace Information;


#include "y/string.hpp"

Y_UTEST(info_pack)
{


    Entropic::Alphabet alpha(Entropic::Multiplex,true);
    StreamBits         io;


    for(int i=0;i<256;++i)
    {
        alpha.write(io,uint8_t(i));
    }

    alpha.reset(Entropic::BlockWise);

    for(int i=0;i<256;++i)
    {
        alpha.write(io,uint8_t(i));
    }

#if 0

    alpha.write(io,'a');
    alpha.reset();
    alpha.write(io,'a');
    alpha.write(io,'a');
    alpha.reset();
    alpha.write(io,'a');
    alpha.write(io,'b');

    for(int i=0;i<256;++i)
    {
        alpha.write(io,uint8_t(i));
    }
    alpha.write(io,'a');

    alpha.reset();
    io.free();

    Y_SIZEOF(Entropic::Unit);
    Y_SIZEOF(Entropic::Alphabet);
    Y_SIZEOF(Entropic::Huffman::Node);
    Y_SIZEOF(Entropic::Huffman::BufferType);
    Y_SIZEOF(Entropic::Huffman);

    Entropic::Huffman huff;
    const String      data = "bananas";
    for(size_t i=1;i<=data.size();++i)
    {
        alpha.write(io,data[i]);
    }
    alpha.display(std::cerr);
    huff.build(alpha.used);
    alpha.display(std::cerr);
#endif


}
Y_UDONE()
