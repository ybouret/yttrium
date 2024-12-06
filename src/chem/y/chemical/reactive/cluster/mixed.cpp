
#include "y/chemical/reactive/cluster/mixed.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        String MixedEquilibrium:: MakeName(const EList               &eqs,
                                           const WOVEn::IntegerArray &cof)
        {
            assert( eqs.size == cof.size() );
            assert( cof.order > 1 );
            String res;
            size_t num = 0;
            for(const ENode *en=eqs.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                const Indexed     &id = eq;
                const apz         &cf = id(cof,SubLevel);
                switch(cf.s)
                {
                    case __Zero__:
                        continue;

                    case Positive:
                        ++num;
                        if(num>1) res += '+';
                        if(cf>1)
                            res += cf.toString();
                        break;

                    case Negative:
                        ++num;
                        if(cf < -1)
                            res += cf.toString();
                        else
                            res += '-';
                        break;
                }
                res += eq.name;

            }
            return res;
        }


    }

}
