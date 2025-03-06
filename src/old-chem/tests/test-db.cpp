
#include "y/chemical/weasel/equilibrium/db.hpp"

#include "y/chemical/reactive/equilibria.hpp"
#include "y/chemical/library.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(db)
{
    for(unsigned i=0;i<EDB::Count;++i)
    {
        Library    lib;
        Equilibria eqs;

        const char * const db = EDB::Table[i];
        std::cerr << "using \"" << db << "\"" << std::endl;
        eqs(lib,db);

        Y_ASSERT(eqs->size()==1);
        const Equilibrium &eq = ** eqs->begin();
        const String       dotFile = Formatted::Get("db%u.dot",i);
        GraphViz::Vizible::DotToPng(dotFile,eq);
        Y_ASSERT(eq.neutral());
    }


}
Y_UDONE()
