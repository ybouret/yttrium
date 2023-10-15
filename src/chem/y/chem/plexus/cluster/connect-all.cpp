#include "y/chem/plexus/cluster.hpp"
#include "y/chem/algebraic.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        static inline String MakeName(const EqArray &                 eqs,
                                      const Algebraic::Coefficients & cof)
        {

            return String();
        }

        void Cluster:: connectAll(XMLog &      xml,
                                  Equilibria & all)
        {
            Y_XML_SECTION(xml, "ConnectAll");
            Chemical::Algebraic::Weight::List W;
            Chemical::Algebraic::Compute(W,Nu,xml);

            for(const Algebraic::Weight *w=W.head;w;w=w->next)
            {
                std::cerr << *w << std::endl;
                const String eid = MakeName(*edb,*w);

            }

        }
    }

}

