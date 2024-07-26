#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/weasel/compiler.hpp"
#include "y/random/park-miller.hpp"
#include "y/utest/run.hpp"

#include "y/data/list/ordered.hpp"
#include "y/data/pool/cxx.hpp"

#include "y/chemical/plexus/solver.hpp"



using namespace Yttrium;
using namespace Chemical;


namespace Yttrium
{
    namespace Chemical
    {

#if 0
        class Vertex : public Object, public XArray
        {
        public:
            typedef CxxPoolOf<Vertex> Pool;
            struct Comparator
            {
                inline SignType operator()(const Vertex * const lhs, const Vertex * const rhs) const noexcept
                {
                    return Comparison::CxxDecreasing(lhs->cost, rhs->cost);
                }
            };


            explicit Vertex(const size_t maxSpecies) :
            XArray(maxSpecies), cost(0), next(0), prev(0)
            {

            }

            virtual ~Vertex() noexcept {}

            void clear() noexcept { ld(cost=0); }



            xreal_t cost;
            Vertex *next;
            Vertex *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Vertex);
        };

        typedef OrderedList<Vertex,Vertex::Comparator,OrderedListQueryHead> Vertices;

        class Simplex : public Vertices
        {
        public:
            explicit Simplex(size_t maxEqs, const size_t maxSpecies) :
            Vertices(),
            pool()
            {
                ++maxEqs;
                while(maxEqs-- > 0) pool.store( new Vertex(maxSpecies) );
            }

            void free() noexcept {
                while(size>0) pool.store( popTail() )->clear();
            }

            Vertex::Pool pool;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Simplex);
        };
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

