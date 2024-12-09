
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



        MixedEquilibrium:: MixedEquilibrium(const String &        _name,
                                            const size_t          _indx,
                                            const EList &         eqs,
                                            const Readable<int> & cof,
                                            const SList &         spc,
                                            const Readable<int> & sto,
                                            XWritable &           shK) :
        Equilibrium(_name,_indx),
        primary(),
        weights(),
        xmul(),
        topK(shK)
        {

            // record primary/weights
            for(const ENode *en=eqs.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                const Indexed     &id = eq;
                const int          nu = id(cof,SubLevel);
                if(0==nu) continue;
                Coerce(primary) << eq;
                Coerce(weights) << nu;
            }
            assert(primary.size>=2);

            // record content
            Components &self = *this;
            for(const SNode *sn=spc.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                const int      nu = sp(sto,SubLevel);
                switch(Sign::Of(nu))
                {
                    case __Zero__: continue;
                    case Positive: self(Product,  nu,sp); break;
                    case Negative: self(Reactant,-nu,sp); break;
                }
            }

        }

        MixedEquilibrium:: ~MixedEquilibrium() noexcept
        {
        }


        xReal MixedEquilibrium:: getK(xReal) const
        {
            xmul.free();
            assert(weights.size==primary.size);

            const WNode *     wn = weights.head;
            for(const ENode * en = primary.head;en;en=en->next,wn=wn->next)
            {
                const int      nu = **wn;
                const Indexed &eq = **en;
                const xReal    eK = eq(topK,TopLevel);
                if(nu<0)
                {
                    const xReal iK = 1.0 / eK;
                    xmul.insert(iK,-nu);
                }
                else
                {
                    assert(nu>0);
                    xmul.insert(eK,nu);
                }
            }
            return xmul.product();
        }
    }

}
