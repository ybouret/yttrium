
#include "y/ink/image/codecs.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;



Y_UTEST(image)
{

    Ink::Codecs & IMG = Ink::Codecs::Std();

    std::cerr << IMG.callSign() << std::endl;



}
Y_UDONE()

