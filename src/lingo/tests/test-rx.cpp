#include "y/lingo/pattern/regexp.hpp"
#include "y/lingo/pattern/basic/lump.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/input.hpp"

using namespace Yttrium;
using namespace Lingo;

Y_UTEST(rx)
{
    if(argc>1)
    {
        const char * const rx = argv[1];
        Dictionary dict;
        dict("INT",new Lump('0','9'));

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
            const AutoPtr<Pattern> p = RegExp(rx,&dict);
            Y_ASSERT(p.isValid());
            GraphViz::Vizible::DotToPng("regexp.dot",*p);

            {
                p->toBinary("regexp.dat");
                InputFile fp("regexp.dat");
                const AutoPtr<Pattern> reloaded = Pattern::Read(fp);
                Y_CHECK( *reloaded == *p );
            }



        }
    }
}
Y_UDONE()
