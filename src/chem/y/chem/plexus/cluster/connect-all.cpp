#include "y/chem/plexus/cluster.hpp"
#include "y/chem/algebraic.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        static inline String FirstCof(const int nu)
        {
            switch(nu)
            {
                case -1: return String('-');
                case  1: return String();
            }
            assert(abs(nu)>1);
            return FormatString("%d*",nu);
        }

        static inline String ExtraCof(const int nu)
        {
            switch(nu)
            {
                case -1: return String('-');
                case  1: return String('+');
            }
            assert(abs(nu)>1);
            if(nu < -1)
                return FormatString("%d*",nu);
            assert(nu>1);
            return FormatString("+%d*",nu);
        }

        static inline String MakeName(const EqArray &                 eqs,
                                      const Algebraic::Coefficients & cof)
        {
            assert(eqs.size()>=cof.size());
            String name;
            bool   first = true;
            for(size_t i=1;i<=cof.size();++i)
            {
                const int nu = cof[i];
                if(0==nu) continue;
                if(first)
                {
                    name += FirstCof(nu);
                    first = false;
                }
                else
                {
                    name += ExtraCof(nu);
                }
                name += eqs[i]->name;
            }

            return name;
        }

        void Cluster:: connectAll(XMLog &      xml,
                                  Equilibria & all)
        {
            Y_XML_SECTION(xml, "ConnectAll");
            Chemical::Algebraic::Weight::List W;
            Chemical::Algebraic::Compute(W,Nu,xml);

            for(const Algebraic::Weight *w=W.head;w;w=w->next)
            {

                const String eid = MakeName(*edb,*w);
                std::cerr << *w << " #" << w->nEqs << " => " << eid << std::endl;
            }

        }
    }

}

