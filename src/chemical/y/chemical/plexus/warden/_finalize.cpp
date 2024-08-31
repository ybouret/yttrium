
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
            assert(ff.xi.mantissa>0);
            assert(ff->size>0);

            // increase species with xi
            {
                const xreal_t xi = ff.xi;
                for(const Actor *a=ac->head;a;a=a->next)
                {
                    const Species &sp = a->sp;
                    cc[ sp.indx[SubLevel] ] += xi * a->xn;
                }
            }

            // vanishing species
            for(const SNode *sn=ff->head;sn;sn=sn->next)
            {
                cc[ (**sn).indx[SubLevel] ].ldz();
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
                const Frontier &ff = **F.tail;                               // highest correction
                const size_t    ii = trades.size() + 1;                      // index of next trade
                XWritable      &cc = mine.transfer(ctrade[ii],SubLevel,C,L); // copy of C
                XWritable      &dc = dtrade[ii].ld(0);                       // zeroed dc
                const xreal_t   xi = ff.xi;

                //--------------------------------------------------------------
                //
                //
                // generate cc with vanishing species as well
                //
                //
                //--------------------------------------------------------------
                Regularize(cc, *ac, ff);

                //--------------------------------------------------------------
                //
                //
                // generate dc and gain
                //
                //
                //--------------------------------------------------------------
                xadd.free();
                for(const Actor *a = (*ac)->head; a; a=a->next)
                {
                    const Species &      sp   = a->sp;
                    const size_t * const id   = sp.indx;
                    const size_t         ii   = id[SubLevel];
                    const size_t         II   = id[L];
                    const xreal_t        cOld = C[II];
                    const xreal_t        cNew = cc[ii];
                    if(cOld.mantissa<0) xadd << -cOld;
                    if(cNew.mantissa<0) xadd <<  cNew;
                    dc[ii] = cNew - cOld;
                }
                const xreal_t gg = xadd.sum();

                Y_XMLOG(xml, (ro ? "(<)" : "(>)") << ' '  << eq << " @" << ff << " => gain = " << real_t(gg) << " /  " << F);

                const Trade tr(eq,cc,gg,dc);
                trades << tr;
            }

        }


        void Warden:: finalize(XWritable &C, const Level L, XMLog &xml)
        {
        FINALIZE:
            //------------------------------------------------------------------
            //
            //
            // collect wobbly unbounded species
            //
            //
            //------------------------------------------------------------------
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

            //------------------------------------------------------------------
            //
            //
            // collect reachable eqs
            //
            //
            //------------------------------------------------------------------
            ERepo  reacOnly(fund.ebank);
            ERepo  prodOnly(fund.ebank);
            size_t reachable = roamingGather(reacOnly, mine.roaming.reacOnly);
            reachable       += roamingGather(prodOnly, mine.roaming.prodOnly);

            Y_XMLOG(xml, "reacOnly : " << reacOnly);
            Y_XMLOG(xml, "prodOnly : " << prodOnly);

            if(!reachable) throw Specific::Exception(CallSign, "no reachable roaming equilbrium!");


            //------------------------------------------------------------------
            //
            //
            // build one-sided trades
            //
            //
            //------------------------------------------------------------------
            trades.free();
            roamingTrades(reacOnly.head,C,L,xml);
            roamingTrades(prodOnly.head,C,L,xml);


            if(trades.size()<=0) throw Specific::Exception(CallSign, "no valid roaming equilbrium!");


            //------------------------------------------------------------------
            //
            //
            // apply most promising trade
            //
            //
            //------------------------------------------------------------------
            HeapSort::Call(trades,Trade::Compare);
            const Trade &tr = trades.head();
            Y_XMLOG(xml, "($) " << tr);
            mine.transfer(C, L, tr.cc, SubLevel);
            goto FINALIZE;
        }


    }
}

