

#include "y/lingo/pattern/basic/single.hpp"
#include "y/lingo/pattern/char-db.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Lingo;



Y_UTEST(pattern)
{
    Pattern::List plist;

    {
        const Single p1('a');
        p1.toBinary("single.dat");
        plist.pushTail( p1.clone() );
    }

}
Y_UDONE()
