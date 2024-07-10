
#include "y/chemical/reactive/equilibrium/mixed.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        MixedEquilibrium:: ~MixedEquilibrium() noexcept {}

        xreal_t MixedEquilibrium:: getK(const xreal_t)
        {
            return 1;
        }

        static const char * const EmptyName = "";


        static inline
        String MakePositive(const int w, bool &first)
        {
            String ans;
            assert(w>0);
            if(!first)
                ans += '+';

            if(w>1)
                ans += Formatted::Get("%d*",w);

            first = false;
            return ans;
        }

        static inline
        String MakeNegative(const int w, bool &first)
        {
            String ans;
            assert(w<0);
            if(w < -1)
                ans += Formatted::Get("%d*",w);
            else
                ans += '-';
            first = false;
            return ans;
        }



        static inline
        String MakeName(const EList         &eqs,
                        const Readable<int> &cof)
        {
            const size_t n = eqs.size; assert(cof.size()==eqs.size);
            String       ans;
            bool         first=true;
            for(size_t i=1;i<=n;++i)
            {
                const int w = cof[i];
                switch( Sign::Of(w) )
                {
                    case __Zero__: continue;
                    case Positive: ans += MakePositive(w,first); break;
                    case Negative: ans += MakeNegative(w,first); break;
                }
                const Equilibrium &eq = (**eqs.fetch(i));
                assert(eq.indx[SubLevel]==i);
                ans += eq.name;
            }


            return ans;
        }

        MixedEquilibrium:: MixedEquilibrium(const size_t        topLevel,
                                            const EList         &eqs,
                                            const Readable<int> &cof) :
        Equilibrium(EmptyName,topLevel),
        xmul(),
        source(),
        mixing()
        {
            { String id = MakeName(eqs,cof); Coerce(name).swapWith(id); }
            
            for(const ENode *en=eqs.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                const int          cf = cof[eq.indx[SubLevel]];
                if(0!=cf)
                {
                    Coerce(source) << eq;
                    Coerce(mixing) << cf;
                }
            }


        }

    }

}

