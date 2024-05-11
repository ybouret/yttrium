
#include "y/chemical/reactive/plexus/limits.hpp"
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
    LimitsBank   lbank;
    SBank        sbank;
    Limits       limits(lbank,sbank);

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
                limits.reset();
                size_t ii = 1;
                for(const SNode *node=species.head;node;node=node->next,++ii)
                {
                    limits(**node,xi[1+(ii%nx)]);
                }
                std::cerr << "limits=" << limits << std::endl;
            }
        }



    }


#if 0


    limits(proton,0.1);
    limits(hydroxy,0.2);
    
    std::cerr << "limits=" << limits << std::endl;
   
    limits.free();
    limits(hydroxy,0.2);
    limits(proton,0.1);
    std::cerr << "limits=" << limits << std::endl;


    limits.free();
    limits(hydroxy,0.2);
    limits(proton,0.2);
    std::cerr << "limits=" << limits << std::endl;

    limits.free();
    limits(hydroxy,0.1);
    limits(proton,0.2);
    limits(AH,0.0);
    std::cerr << "limits=" << limits << std::endl;

    limits.free();
    limits(hydroxy,0.1);
    limits(proton,0.2);
    limits(AH,0.1);
    std::cerr << "limits=" << limits << std::endl;

    limits.free();
    limits(hydroxy,0.1);
    limits(proton,0.2);
    limits(AH,0.25);
    std::cerr << "limits=" << limits << std::endl;

    limits.free();
    limits(hydroxy,0.1);
    limits(proton,0.2);
    limits(AH,0.2);
    std::cerr << "limits=" << limits << std::endl;

    limits.free();
    limits(hydroxy,0.1);
    limits(proton,0.2);
    limits(AH,0.15);
    std::cerr << "limits=" << limits << std::endl;

#endif


}
Y_UDONE()
