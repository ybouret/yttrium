
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



        void   Warden:: roamingTrades(const ENode *       en,
                                      const XReadable &   C,
                                      const Level         L,
                                      XMLog               &xml)
        {
            for(;0!=en;en=en->next)
            {
                const Equilibrium &eq     = **en;
                const Actors  *    actors = 0;
                bool               direct = true;

                switch(eq.kind)
                {
                    case Nebulous:
                    case Standard:
                        throw Specific::Exception(CallSign, "no possible roaming trade for '%s", eq.name.c_str());
                    case ReacOnly:
                        actors = &eq.reac;
                        direct = true;
                        break;

                    case ProdOnly:
                        actors = &eq.prod;
                        direct = false;
                        break;
                }

                //SingleFrontier F(fund.sbank);
                Frontiers      F(fund);
                for(const Actor *a=(*actors)->head;a;a=a->next)
                {
                    const Species &sp = a->sp;
                    const xreal_t  cc = C[ sp.indx[L] ];
                    if(cc.mantissa<0)
                    {
                        F(-cc/a->xn,sp);
                    }
                }

                if(F.size>0)
                {
                    Y_XMLOG(xml, "(+) " << eq << " @" << F);
                }

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

