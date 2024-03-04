
#include "y/utest/run.hpp"
#include "y/information/entropic/alphabet.hpp"



using namespace Yttrium;
using namespace Information;

Y_UTEST(info_pack)
{
    Y_SIZEOF(Entropic::Unit);
    Y_SIZEOF(Entropic::Alphabet);

    Entropic::Alphabet alpha(Entropic::Multiplex,true);
    StreamBits         io;

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


}
Y_UDONE()
