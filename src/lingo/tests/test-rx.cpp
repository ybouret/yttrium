#include "y/lingo/pattern/regexp.hpp"
#include "y/lingo/pattern/basic/range.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Lingo;

Y_UTEST(rx)
{
    if(argc>1)
    {
        const char * const rx = argv[1];
        Dictionary dict;
        dict("INT",new Range('0','1'));

        {
            const AutoPtr<Pattern> p = Pattern::Exact(rx);
            std::cerr << "Exact(" << rx << ")=" << p->regularExpression() << std::endl;
            GraphViz::Vizible::DotToPng("exact.dot",*p);
        }

        {
            const AutoPtr<Pattern> p = Pattern::Among(rx);
            std::cerr << "Among(" << rx << ")=" << p->regularExpression() << std::endl;
            GraphViz::Vizible::DotToPng("among.dot",*p);
        }

        {
            const AutoPtr<Pattern> p = RegExp(rx,0);
            Y_ASSERT(p.isValid());
            //GraphViz::Vizible::DotToPng("regexp.dot",*p);
        }
    }
}
Y_UDONE()
