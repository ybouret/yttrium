#include "y/chemical/weasel/parser.hpp"
#include "y/chemical/formula.hpp"
#include "y/chemical/weasel/equilibrium/db.hpp"
#include "y/chemical/weasel/formula/to-text.hpp"
#include "y/chemical/reactive/equilibria.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(weasel)
{
    Weasel &weasel = Weasel::Instance();
    std::cerr << "Using " << weasel.callSign() << std::endl;
    

    if( argc > 1)
    {
        AutoPtr<XNode> xnode = weasel.parse( Lingo::Module::OpenFile(argv[1]) );
        GraphViz::Vizible::DotToPng("xnode.dot", *xnode);

        Weasel::FormulaToText ftt;
        ftt.policy  = Lingo::Syntax::Permissive;
        ftt.verbose = true;

        for(const XNode *node=xnode->branch().head;node;node=node->next)
        {
            if(node->name() == Formula::CallSign)
            {

                int    z;
                //ftt(*node);
                ArcPtr<const String> name = ftt.get(*node,z);
                std::cerr << name << " with z=" << z << std::endl;
            }
        }

    }
    else
    {
        for(size_t i=0;i<EDB::Count;++i)
        {
            const char * const text = EDB::Table[i];
            std::cerr << "parsing \"" << text << "\"" << std::endl;
            AutoPtr<XNode> xnode = weasel.parse( Lingo::Module::OpenData("db",text));
        }
    }
    Y_SIZEOF(XNode);
    Y_SIZEOF(Formula);
    Y_SIZEOF(Species);
    Y_SIZEOF(Actor);
    Y_SIZEOF(Component);
    Y_SIZEOF(Equilibrium);

}
Y_UDONE()

