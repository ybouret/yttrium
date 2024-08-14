

#include "y/chemical/plexus.hpp"
#include "y/chemical/reactive/aftermath.hpp"

#include "y/utest/run.hpp"

#include "y/system/exception.hpp"
#include "y/sort/heap.hpp"
#include "y/text/boolean.hpp"

#include "y/stream/libc/output.hpp"
#include "y/mkl/opt/minimize.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Prospect
        {
        public:
            typedef CxxSeries<Prospect,XMemory> Series;

            Prospect(const Equilibrium &_eq,
                     const xreal_t      _eK,
                     const XReadable   &_cc,
                     const xreal_t      _xi,
                     const XReadable   &_dc) :
            eq(_eq),
            eK(_eK),
            cc(_cc),
            xi(_xi),
            ax(xi.abs()),
            dc(_dc)
            {
            }

            Prospect(const Prospect &_) noexcept :
            eq(_.eq),
            eK(_.eK),
            cc(_.cc),
            xi(_.xi),
            ax(_.ax),
            dc(_.dc)
            {
            }

            ~Prospect() noexcept {}

            std::ostream & show(std::ostream &os, const Cluster &cl, const bool full) const
            {
                cl.uuid.pad(os << eq.name, eq) << "|xi=" << std::setw(15) << real_t(xi);
                if(full)
                {
                    eq.displayCompact(os << " @", cc, SubLevel);
                }
                return os;
            }

            xreal_t objectiveFunction(XMul            &X,
                                      const XReadable &C,
                                      const Level      L) const
            {
                return eq.affinity(eK,X,C,L);
            }

            static int CompareAX(const Prospect &lhs, const Prospect &rhs) noexcept
            {
                return Comparison::Decreasing(lhs.ax, rhs.ax);
            }

            const Equilibrium &eq;
            const xreal_t      eK;
            const XReadable   &cc;
            const xreal_t      xi;
            const xreal_t      ax;
            const XReadable   &dc;

        private:
            Y_DISABLE_ASSIGN(Prospect);
        };


        using namespace MKL;


    }

}

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(crack)
{
    Plexus plexus;

    for(int i=1;i<argc;++i)
    {
        plexus(Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << plexus.lib << std::endl;
    std::cerr << "eqs=" << plexus.eqs << std::endl;

    const Library   &lib = plexus.lib;
    Clusters        &cls = plexus.assemble();
    const XReadable &K   = cls.K(0);


    XVector C0(lib->size(),0);

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        Species::Conc(C0,plexus.ran,0.3);



        lib(std::cerr << "C0=","\t[",C0,"]");


        lib(std::cerr << "C1=","\t[",C0,"]");

    }

    Y_SIZEOF(Prospect);

}
Y_UDONE()

