#include "y/graphviz/vizible.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"
#include <cstdlib>

using namespace Yttrium;

Y_UTEST(graphviz)
{
    Vizible A,B;

    {
        Libc::OutputFile fp("viz.dot");
        Vizible::Enter(fp, "G");

        A.node(fp);
        fp << "[";
        Vizible::Label(fp, "A");
        fp << "]";
        Vizible::Endl(fp);

        Vizible::Endl(Vizible::Label(B.node(fp) << "[","B") << "]");

        Vizible::Endl(A.arrow(fp,&B));

        Vizible::Leave(fp);
    }

    Vizible::Render("viz.png", "viz.dot");
    


}
Y_UDONE()


