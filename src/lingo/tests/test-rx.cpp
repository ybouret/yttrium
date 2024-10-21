#include "y/lingo/pattern/regexp.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Lingo;

Y_UTEST(rx)
{
    if(argc>1)
    {
        const char * const rx = argv[1];
        {
            const AutoPtr<Pattern> p = Pattern::Exact(rx);
            std::cerr << "Exact(" << rx << ")=" << p->regularExpression() << std::endl;
        }

        {
            const AutoPtr<Pattern> p = Pattern::Among(rx);
            std::cerr << "Among(" << rx << ")=" << p->regularExpression() << std::endl;
        }
    }
}
Y_UDONE()
