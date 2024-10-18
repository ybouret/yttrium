

#include "y/lingo/pattern/all.hpp"
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

    {
        const Range p2('a','z');
        p2.toBinary("range.dat");
        plist.pushTail( p2.clone() );
    }

    Y_SIZEOF(Single);
    Y_SIZEOF(Range);

}
Y_UDONE()
