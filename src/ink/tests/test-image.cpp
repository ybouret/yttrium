
#include "y/ink/image/codecs.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;



Y_UTEST(image)
{

    Ink::Codecs & IMG = Ink::Codecs::Std();

    Vector<Ink::Format::Handle> formats;
    IMG.fetch(formats);
    std::cerr << IMG.callSign() << " : " << formats << std::endl;



}
Y_UDONE()

