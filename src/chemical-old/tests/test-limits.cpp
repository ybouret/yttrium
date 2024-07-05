
#include "y/chemical/plexus/boundaries.hpp"
#include "y/chemical/species/library.hpp"

#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(limits)
{
    Random::Rand ran;
    Library      lib;
    BBank        bbank;
    SBank        sbank;
    Boundaries   bnd(bbank,sbank);

    for(size_t ns=1;ns<=8;++ns)
    {
        const char id = 'a' + (ns-1);
        lib(id,0);
        Y_ASSERT(lib->size()==ns);
        SList species;
        for(Library::ConstIterator it=lib->begin();it!=lib->end();++it)
            species << **it;
        Y_ASSERT(species.size==ns);
        Random::Shuffle::List(species,ran);
        std::cerr << species << std::endl;
        for(size_t nx=1;nx<=ns;++nx)
        {
            Vector<xreal_t> xi(nx,AsCapacity);
            for(size_t i=0;i<nx;++i)
                xi <<  ran.to<double>();
            Y_ASSERT(xi.size()==nx);
            for(size_t cycle=1;cycle<=4;++cycle)
            {
                Random::Shuffle::Range(xi,ran);
                bnd.reset();
                size_t ii = 1;
                for(const SNode *node=species.head;node;node=node->next,++ii)
                {
                    bnd(**node,xi[1+(ii%nx)]);
                }
                std::cerr << "boundaries=" << bnd << std::endl;
            }
        }



    }


    

}
Y_UDONE()
