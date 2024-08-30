
#include "y/chemical/plexus/warden.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        size_t Warden:: collectRoaming(ERepo       &target,
                                       const EList &source) const
        {
            target.free();
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
            size_t reachable = collectRoaming(reacOnly, mine.roaming.reacOnly);
            reachable       += collectRoaming(prodOnly, mine.roaming.prodOnly);

            Y_XMLOG(xml, "reacOnly : " << reacOnly);
            Y_XMLOG(xml, "prodOnly : " << prodOnly);

            if(!reachable) throw Specific::Exception(CallSign, "no reachable roaming equilbrium!");

            CxxSeries<Frontier> ff(mine.roaming.reacOnly.size+mine.roaming.prodOnly.size);

            for(const ENode *en=reacOnly.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
            }

        }

    }
}

