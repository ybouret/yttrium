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

        Vizible::Node(fp,&A); //A.node(fp);
        fp << "[";
        Vizible::Label(fp, "A");
        fp << "]";
        Vizible::Endl(fp);

        Vizible::Endl(Vizible::Label( Vizible::Node(fp,&B) << "[","B") << "]");

        Vizible::Endl( Vizible::Arrow(fp,&A,&B) );

        Vizible::Leave(fp);
    }

    Vizible::Render("viz.png", "viz.dot");
    


}
Y_UDONE()


