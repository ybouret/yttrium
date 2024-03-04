
#include "y/utest/run.hpp"
#include "y/information/entropic/alphabet.hpp"



using namespace Yttrium;
using namespace Information;

Y_UTEST(info_pack)
{
    Y_SIZEOF(Entropic::Unit);
    Y_SIZEOF(Entropic::Alphabet);

    Entropic::Alphabet alpha(true);
    StreamBits         io;

    alpha.write(io,'a');
    alpha.reset();
    alpha.write(io,'a');
    alpha.write(io,'a');
    alpha.reset();
    alpha.write(io,'a');
    alpha.write(io,'b');



}
Y_UDONE()
