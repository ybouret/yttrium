#include "y/graphviz/vizible.hpp"
#include "y/graphviz/color-scheme.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"
#include <cstdlib>

using namespace Yttrium;
using namespace GraphViz;

Y_UTEST(graphviz)
{
    Vizible A,B;

    {
        Libc::OutputFile fp("viz.dot");
        Vizible::Enter(fp, "G");

        Vizible::Node(fp,&A); //A.node(fp);
        fp << "[";
        Vizible::Label(fp, "A");
        Vizible::Color(fp << ", color=", "accent5", 4);
        fp << "]";
        Vizible::Endl(fp);

        Vizible::Endl(Vizible::Label( Vizible::Node(fp,&B) << "[","B") << "]");

        Vizible::Endl( Vizible::Arrow(fp,&A,&B) );

        Vizible::Leave(fp);
    }

    Vizible::Render("viz.png", "viz.dot", true);

    std::cerr << "#ColorScheme: " << ColorScheme::Count << std::endl;


}
Y_UDONE()


