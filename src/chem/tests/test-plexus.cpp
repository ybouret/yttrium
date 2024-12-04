#include "y/chemical/reactive/clusters.hpp"
#include "y/chemical/library.hpp"
#include "y/random/park-miller.hpp"

#include "y/chemical/reactive/conservation/law.hpp"


#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(plexus)
{
    Random::ParkMiller ran;
    Library            lib;
    Equilibria         eqs;

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool verbose = true;
    XMLog xml(verbose);
    Clusters clusters(eqs,xml);

    std::cerr << clusters << std::endl;

    std::cerr << "lib=" << lib << std::endl;

    if(lib->size()>=2)
    {
        Actor::List       alist;
        for(Library::ConstIterator it=lib->begin();it!=lib->end();++it)
        {
            alist.pushTail( new Actor( ran.in<unsigned>(1,5), **it) );
        }
        Conservation::Law law(alist);
        std::cerr << law << std::endl;
    }



}
Y_UDONE()
