
#include "y/chemical/plexus/warden.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        size_t Warden:: roamingGather(ERepo       &target,
                                       const EList &source) const
        {
            assert(0==target.size);
            for(const ENode *en=source.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                for(const SNode *sn=wobbly.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    if( eq.query(sp) )
                    {
                        target << eq;
                        break;
                    }
                }
            }
            return target.size;
        }



        static inline
        void Regularize(XWritable      &cc,
                        const Actors   &ac,
                        const Frontier &ff) noexcept
        {
            const xreal_t xi = ff.xi;
            assert(xi.mantissa>0);
            for(const Actor *a=ac->head;a;a=a->next)
            {
                const Species &sp = a->sp;
                cc[ sp.indx[SubLevel] ] += xi * a->xn;
            }
        }

        void   Warden:: roamingTrades(const ENode *       en,
                                      const XReadable &   C,
                                      const Level         L,
                                      XMLog               &xml)
        {
            for(;0!=en;en=en->next)
            {
                //--------------------------------------------------------------
                //
                //
                // get equilibrium and select way
                //
                //
                //--------------------------------------------------------------

                const Equilibrium &eq = **en;
                const Actors  *    ac  = 0;
                bool               ro  = false;

                switch(eq.kind)
                {
                    case Nebulous:
                    case Standard:
                        throw Specific::Exception(CallSign, "no possible roaming trade for '%s", eq.name.c_str());
                   
                    case ReacOnly:
                        ac = &eq.reac;
                        ro = true;
                        break;

                    case ProdOnly:
                        ac = &eq.prod;
                        ro = false;
                        break;
                }

                //--------------------------------------------------------------
                //
                //
                // build sorted frontiers
                //
                //
                //--------------------------------------------------------------
                Frontiers      F(fund);
                for(const Actor *a=(*ac)->head;a;a=a->next)
                {
                    const Species &sp = a->sp;
                    const xreal_t  cc = C[ sp.indx[L] ];
                    if(cc.mantissa<0)
                    {
                        F(-cc/a->xn,sp);
                    }
                }

                if(F.size<=0) 
                {
                    Y_XMLOG(xml, "(-) " << eq);
                    continue;
                }

                //--------------------------------------------------------------
                //
                //
                // select highest frontier
                //
                //
                //--------------------------------------------------------------
                const Frontier &ff = **F.tail;
                const size_t    ii = trades.size() + 1;
                XWritable      &cc = mine.transfer(ctrade[ii],SubLevel,C,L);
                XWritable      &dc = dtrade[ii].ld(0);
                const xreal_t   xi = ff.xi;

                //--------------------------------------------------------------
                //
                //
                // generate cc with vanishing species as well
                //
                //
                //--------------------------------------------------------------
                if(ro)
                {
                    assert(eq.reac->size>0);
                    Regularize(cc,eq.reac,ff);
                }
                else
                {
                    assert(eq.prod->size>0);
                    Regularize(cc,eq.prod,ff);
                }




                // generate dc and gain
                xadd.free();

                Y_XMLOG(xml, (ro ? "(<)" : "(>)") << ' '  << eq << " @" << F << " ( <-- " << ff << " )");


            }

        }


        void Warden:: finalize(XWritable &C, const Level L, XMLog &xml)
        {
            // collect wobbly unbounded species
            wobbly.free();
            for(const SNode *sn = mine.unbounded.list.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                if( C[ sp.indx[L]].mantissa < 0 )
                {
                    wobbly << sp;
                }

            }

            if(wobbly.size<=0) return;

            Y_XML_SECTION_OPT(xml, "unbounded", wobbly);

            // collect concerned eqs
            ERepo  reacOnly(fund.ebank);
            ERepo  prodOnly(fund.ebank);
            size_t reachable = roamingGather(reacOnly, mine.roaming.reacOnly);
            reachable       += roamingGather(prodOnly, mine.roaming.prodOnly);

            Y_XMLOG(xml, "reacOnly : " << reacOnly);
            Y_XMLOG(xml, "prodOnly : " << prodOnly);

            if(!reachable) throw Specific::Exception(CallSign, "no reachable roaming equilbrium!");


            trades.free();
            roamingTrades(reacOnly.head,C,L,xml);
            roamingTrades(prodOnly.head,C,L,xml);


        }


    }
}

