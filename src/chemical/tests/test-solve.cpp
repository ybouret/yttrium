#include "y/chemical/plexus/solver/normalizer.hpp"

#include "y/chemical/plexus/clusters.hpp"

#include "y/chemical/weasel/compiler.hpp"
#include "y/random/park-miller.hpp"
#include "y/utest/run.hpp"

#include "y/system/exception.hpp"
#include "y/orthogonal/family.hpp"


#include "y/mkl/opt/minimize.hpp"
#include "y/stream/libc/output.hpp"

#include "y/mkl/algebra/lu.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        

        class Solver : public Proxy<const Normalizer::Set>
        {
        public:
            static const char * const CallSign;
            typedef Normalizer::Set::Iterator      Iterator;
            typedef Normalizer::Set::ConstIterator ConstIterator;

            explicit Solver(const SharedClusters &usr) :
            cls(usr),
            ndb()
            {
                for(const Cluster *cl=(*cls)->head;cl;cl=cl->next)
                {
                    const Normalizer::Ptr ptr = new Normalizer(*cl);
                    if(!ndb.insert(ptr)) throw Specific::Exception(CallSign, "corrupted cluster address");
                }
            }

            virtual ~Solver() noexcept { }

            void run(XWritable       & Ctop,
                     const XReadable & Ktop,
                     XMLog           &  xml)
            {
                size_t n = ndb.size();
                for(Iterator it=ndb.begin();n>0;--n,++it)
                {
                    Normalizer &normalizer = **it; assert( (*cls)->owns( & normalizer.rcl ) );
                    normalizer.run(Ctop,Ktop,xml);
                }
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
            SharedClusters  cls;
            Normalizer::Set ndb;

            virtual ConstInterface & surrogate() const noexcept
            {
                return ndb;
            }
        };


        const char * const Solver:: CallSign = "Chemical::Solver";
    }
}



using namespace Yttrium;
using namespace Chemical;





Y_UTEST(solve)
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

    bool           verbose = true;
    XMLog          xml(verbose);
    SharedClusters   cls = new Clusters(eqs,xml);
    const XReadable &K   = cls->K(0);


    XVector C0(lib->size(),0);

    Solver  solver(cls);


    for(size_t iter=0;iter<1;++iter)
    {
        Species::Conc(C0,ran,0.3);

        //C0.ld(0);

        lib(std::cerr << "C0=","\t[",C0,"]");
        solver.run(C0,K, xml);
        lib(std::cerr << "C1=","\t[",C0,"]");



    }

    Y_SIZEOF(Normalizer);
    Y_SIZEOF(Clusters);
    
}
Y_UDONE()
