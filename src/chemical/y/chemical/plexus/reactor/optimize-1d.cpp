
#include "y/chemical/plexus/reactor.hpp"
#include "y/mkl/opt/minimize.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;


        xreal_t Reactor:: optimize1D(const xreal_t Sini)
        {
            Reactor &self = *this;
            XTriplet U    = { 0,    0,  1 };
            XTriplet S    = { Sini, 0,  score(Cend,SubLevel) };
            const xreal_t Uopt = Minimize<xreal_t>::Locate(Minimizing::Inside,self,U,S);
            const xreal_t Sopt = self(Uopt);
            return Sopt;
        }

    }

}

#include "y/stream/libc/output.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        namespace
        {

            struct Wander
            {
                const Outcome &out0;
                const Outcome &out1;
                Reactor       &reactor;

                xreal_t operator()(const xreal_t u)
                {
                    const xreal_t v = 1.0 - u;
                    for(const SNode *sn=reactor.cluster->species->head;sn;sn=sn->next)
                    {
                        const Species &sp = **sn;
                        const xreal_t C0 = sp(out0.cc,out0.lv);
                        const xreal_t C1 = sp(out1.cc,out1.lv);
                        xreal_t cmin = C0, cmax = C1;
                        if(cmax<cmin) Swap(cmin,cmax);
                        assert(cmin<=cmax);
                        sp(reactor.Ctry,SubLevel) = Clamp(cmin,C0*v+C1*u,cmax);
                    }
                    return reactor.score(reactor.Ctry,SubLevel);
                }


            };

        }

        xreal_t Reactor:: zHierarchy(XMLog &xml, const xreal_t S0)
        {

            Y_XML_SECTION_OPT(xml, "zHierarchy", "|tighten|=" << tighten.size);

            assert(tighten.isSortedBy(ByIncreasingSC));

            if(tighten.size>=2)
            {
                //Wander wander = { **(tighten.tail->prev), **(tighten.tail), *this };
                Wander wander = { **(tighten.head), **(tighten.head->next), *this };

                {
                    OutputFile fp("wander.dat");
                    const size_t np = 1000;
                    for(size_t i=0;i<=np;++i)
                    {
                        double u = double(i)/np;
                        fp("%.15g %.15g\n",u, double( wander(u) ) );
                    }
                }
            }

            throw Exception("need to finish");

            return S0;
        }
    }

}

