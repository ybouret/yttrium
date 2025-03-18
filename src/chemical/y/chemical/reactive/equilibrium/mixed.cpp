
#include "y/chemical/reactive/equilibrium/mixed.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        MixedEquilibrium:: MixedEquilibrium(WList               &wl,
                                            EList               &el,
                                            const SList         &sl,
                                            const Readable<int> &st,
                                            const size_t         ii,
                                            XWritable           &KK) :
        Equilibrium( MakeName(wl,el), ii),
        topK(KK),
        wlist(),
        elist(),
        xmul(),
        one(1)
        {
            // steal data
            Coerce(wlist).swapWith(wl);
            Coerce(elist).swapWith(el);

            // fill
            for(const SNode *sn=sl.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                const int      cf = sp(st,SubLevel);
                switch( Sign::Of(cf) )
                {
                    case __Zero__: continue;
                    case Positive: use(Product,  cf,sp); break;
                    case Negative: use(Reactant,-cf,sp); break;
                }
            }

            // done
            latch();
        }


        MixedEquilibrium:: ~MixedEquilibrium() noexcept
        {
        }

        String MixedEquilibrium:: FirstName(const int cf, const String &id)
        {
            assert(0!=cf);
            if(1==cf)                 return id;
            if(-1==cf)          return '-' + id;
            return Formatted::Get("%d*",cf) + id;
        }

        String MixedEquilibrium:: ExtraName(const int cf, const String &id)
        {

            if(1==cf)  return           '+' + id;
            if(-1==cf) return           '-' + id;
            return Formatted::Get("%+d*",cf) + id;
        }


        const String *MixedEquilibrium:: MakeName(const WList &wl,
                                                  const EList &el)
        {
            assert(wl.size==el.size);
            assert(wl.size>=2);
            const WNode *wn = wl.head;
            const ENode *en = el.head;

            AutoPtr<String> ans = new String();
            *ans += FirstName(**wn,*(**en).name);
            for(wn=wn->next,en=en->next;wn;wn=wn->next,en=en->next)
            {
                *ans += ExtraName(**wn,*(**en).name);
            }


            return ans.yield();
        }


        xreal_t MixedEquilibrium:: getK(xreal_t)
        {
            xmul.free();
            xmul << one;

            assert(wlist.size==elist.size);
            assert(elist.size>0);

            const WNode     *wn = wlist.head;
            for(const ENode *en = elist.head;en;en=en->next,wn=wn->next)
            {
                const int          cf = **wn;
                const Equilibrium &eq = **en;
                const xreal_t      eK = eq(topK,TopLevel);
                if(cf>0)
                {
                    xmul.insert(eK,cf);
                }
                else
                {
                    assert(cf<0);
                    const xreal_t iK = one/eK;
                    xmul.insert(iK,-cf);
                }
            }


            return xmul.product();
        }

    }

}


