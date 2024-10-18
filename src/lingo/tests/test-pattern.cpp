

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
        std::cerr << "single='" << p1.regularExpression() << "'" << std::endl;
    }

    {
        const Range p2('a','z');
        p2.toBinary("range.dat");
        plist.pushTail( p2.clone() );
        std::cerr << "range='" << p2.regularExpression() << "'" << std::endl;

    }

    {
        const Exclude p3('k');
        p3.toBinary("exclude.dat");
        plist.pushTail( p3.clone() );
        std::cerr << "excl='" << p3.regularExpression() << "'" << std::endl;

    }

    Y_SIZEOF(Single);
    Y_SIZEOF(Range);
    Y_SIZEOF(Exclude);

}
Y_UDONE()
