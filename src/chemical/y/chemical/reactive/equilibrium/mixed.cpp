
#include "y/chemical/reactive/equilibrium/mixed.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        MixedEquilibrium:: ~MixedEquilibrium() noexcept {}


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
            const size_t n = cof.size(); assert(cof.size()<=eqs.size);
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
                                            const Readable<int> &cof,
                                            XWritable           &shK) :
        Equilibrium(EmptyName,topLevel),
        xmul(),
        topK(shK),
        coef()
        {
            { String id = MakeName(eqs,cof); Coerce(name).swapWith(id); }

            const size_t n = cof.size();
            for(size_t i=1;i<=n;++i)
            {
                const int cf = cof[i];
                if(0==cf) continue;
                const Equilibrium &eq = (**eqs.fetch(i));
                assert(eq.indx[SubLevel]==i);
                Coerce(sire) << eq;
                Coerce(coef) << cf;
            }


        }

        xreal_t MixedEquilibrium:: getK(const xreal_t)
        {
            const xreal_t one(1);
            xmul.free();
            const ENode       *en = sire.head;
            for(const MixNode *mn = coef.head;mn;mn=mn->next,en=en->next)
            {
                const xreal_t Ki = topK[ (**en).indx[TopLevel] ];
                const int     ni = **mn;
                if(ni>0)
                {
                    xmul.insert(Ki,ni);
                }
                else
                {
                    const xreal_t iK = one/Ki;
                    xmul.insert(iK,-ni);
                }
            }

            return xmul.product();
        }

    }

}

