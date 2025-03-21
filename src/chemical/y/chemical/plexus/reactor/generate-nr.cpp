
#include "y/chemical/plexus/reactor.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        using namespace MKL;


        xreal_t Reactor:: generateNR(XMLog &xml, const xreal_t S0, const XReadable & K0)
        {
            static const char fn[] = "GenerateNR";
            const size_t      n = basis.size;
            Y_XML_SECTION_OPT(xml, fn, "|basis|=" << n);

            if(n<=0)
            {
                Y_XML_COMMENT(xml, "empty basis");
                return S0;
            }

            const size_t m = cluster->species->size;
            XMatrix J(n,m);
            XArray  A(n);
            size_t  i=1;
            for(const ENode *en=basis.head;en;en=en->next,++i)
            {
                const Equilibrium &eq = **en;
                A[i] = eq.jacobian(J[i], eq(K0,TopLevel), x_score, Cini, SubLevel);
            }


            return S0;

        }

    }

}


