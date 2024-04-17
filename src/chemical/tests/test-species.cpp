#include "y/chemical/species.hpp"
#include "y/utest/run.hpp"

#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(species)
{
    Species proton("H+",1,1);
    {
        OutputFile fp("species.dot");
        GraphViz::Vizible::Enter(fp, "G");
        proton.viz(fp);
        GraphViz::Vizible::Leave(fp);

    }
}
Y_UDONE()

