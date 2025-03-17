
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
        xmul()
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
            String ans;
            assert(cf!=0);
            ans += Formatted::Get("%+d",cf);
            ans += id;
            return ans;
        }

        String MixedEquilibrium:: ExtraName(const int cf, const String &id)
        {
            String ans;
            assert(cf>0);
            ans += Formatted::Get("%+d",cf);
            ans += id;
            return ans;
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
                *ans += FirstName(**wn,*(**en).name);
            }


            return ans.yield();
        }
    }

}


