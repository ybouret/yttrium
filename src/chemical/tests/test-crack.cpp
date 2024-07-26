#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/weasel/compiler.hpp"
#include "y/random/park-miller.hpp"
#include "y/utest/run.hpp"



#include "y/chemical/plexus/solver.hpp"



using namespace Yttrium;
using namespace Chemical;


namespace Yttrium
{
    namespace Chemical
    {
#if 1
     
#endif


       

      

       

    }

}


Y_UTEST(crack)
{

    Weasel::Compiler  &weasel = Weasel::Compiler::Instance();
    Library            lib;
    LuaEquilibria      eqs;
    Random::ParkMiller ran;

    for(int i=1;i<argc;++i)
    {
        weasel(lib,eqs,Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool     verbose = true;
    XMLog    xml(verbose);
    Clusters clusters(eqs,xml);
    const XReadable &K = clusters.K(0);

    XVector C0(lib->size(),0);
    Species::Conc(C0,ran,0.3);

    C0.ld(0);

    lib(std::cerr << "C0=","\t[",C0,"]");

    Solver solver(clusters);
    for(const Cluster *cl = clusters->head; cl; cl=cl->next)
    {
        solver.run(*cl, C0, K, xml);
    }



}
Y_UDONE()

