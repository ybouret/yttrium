
#include "y/chemical/reactive/equilibrium/hybrid.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        HybridEquilibrium:: ~HybridEquilibrium() noexcept {}
        

        String HybridEquilibrium:: MakeName(const EList &         primaryList,
                                            const Readable<int> & coefficient)
        {
            assert( primaryList.size == coefficient.size() );
            String res;
            size_t num = 0;
            for(const ENode *en=primaryList.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                const Indexed     &id = eq;
                const int          cf = id(coefficient,SubLevel);
                switch( Sign::Of(cf) )
                {
                    case __Zero__:
                        continue;

                    case Positive:
                        ++num;
                        if(num>1) res += '+';
                        if(cf>1)  res += Formatted::Get("%d",cf);
                        break;

                    case Negative:
                        ++num;
                        if(cf < -1) res += Formatted::Get("%d",cf);
                        else        res += '-';
                        break;
                }
                res += eq.name;

            }
            return res;
        }
    }

}

