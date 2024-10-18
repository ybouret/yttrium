
#include "y/lingo/pattern/char-db.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Lingo;



Y_UTEST(chardb)
{
    CharDB fc;

    for(unsigned i=0;i<256;++i)
    {
        fc.free();
        fc.set(i);
        Y_ASSERT(fc.has(i));
        fc.clr(i);
        Y_ASSERT(!fc.has(i));
    }


}
Y_UDONE()
