
#include "y/chemical/reactive/equilibrium/hybrid.hpp"
#include "y/system/exception.hpp"

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
            assert(eqs.size==cof.size);

            const ENode    *en = eqs.head;
            const CoefNode *cn = cof.head;
            for(size_t i=eqs.size;i>0;--i,en=en->next,cn=cn->next)
            {
                const Equilibrium &eq = **en;
                const int          nu = **cn;
                const xReal        eK = eq(_K_,TopLevel);
                switch( Sign::Of(nu) )
                {
                    case __Zero__: throw Specific::Exception(name.c_str(),"zero coefficient for '%s'", eq.name.c_str()); // never hapen
                    case Positive: xmul.insert(eK,nu);      break;
                    case Negative: xmul.insert(1.0/eK,-nu); break;
                }
            }
            return xmul.product();
        }

        HybridEquilibrium:: HybridEquilibrium(const String &        _name,
                                              const size_t          _indx,
                                              const EList  &        elist,
                                              const Readable<int> & ecoef,
                                              const SList         & slist,
                                              const Readable<int> & scoef,
                                              const XReadable     & top_K) :
        Equilibrium(_name,_indx),
        eqs(),
        cof(),
        _K_(top_K)
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

