
#include "y/chemical/weasel/compiler.hpp"
#include "y/chemical/plexus/injector.hpp"
#include "y/stream/libc/output.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        class Solver
        {
        public:
            explicit Solver(const Clusters &cls) :
            Ceq(cls.maxEPC,cls.maxSPC),
            dCe(cls.maxEPC,cls.maxSPC)
            {

            }

            virtual ~Solver() noexcept
            {
            }

            XMatrix Ceq;
            XMatrix dCe;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
        };
    }
    
}
using namespace Yttrium;
using namespace Chemical;

Y_UTEST(solve)
{
    Weasel::Compiler &weasel = Weasel::Compiler::Instance();
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
    (void) clusters.K(0);

    clusters.showK(std::cerr);

    const size_t M = lib->size();
    XVector      C0(0,M);
    Species::Conc(C0,ran,0.3);

    for(const Cluster *cl=clusters->head;cl;cl=cl->next)
    {
        for(const ENode *en=cl->head;en;en=en->next)
        {
            const Equilibrium &eq = **en;
            std::cerr << eq << std::endl;
        }
    }


}
Y_UDONE()
