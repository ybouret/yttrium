
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


        xReal HybridEquilibrium:: getK(xReal) const
        {
            xmul.free();

            return xmul.product();
        }

        HybridEquilibrium:: HybridEquilibrium(const String &        _name,
                                              const size_t          _indx,
                                              const EList  &        elist,
                                              const Readable<int> & ecoef,
                                              const SList         & slist,
                                              const Readable<int> & scoef) :
        Equilibrium(_name,_indx),
        xmul(),
        eqs(),
        cof()
        {
            // build combination
            for(const ENode *en=elist.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                const int          nu = eq(ecoef,SubLevel);
                if(nu!=0)
                {
                    Coerce(eqs) << eq;
                    Coerce(cof) << nu;
                }
            }

            // record species
            for(const SNode *sn=slist.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                const int      nu = sp(scoef,SubLevel);
                switch(Sign::Of(nu))
                {
                    case __Zero__: continue;
                    case Positive: add(Product,nu,sp);   break;
                    case Negative: add(Reactant,-nu,sp); break;
                }
            }

        }

    }

}

