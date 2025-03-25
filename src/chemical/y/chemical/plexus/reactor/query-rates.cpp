
#include "y/chemical/plexus/reactor.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        const char * const Reactor:: QueryRates = "QueryRates";

        xreal_t Reactor:: queryRates(XMLog &xml, const xreal_t S0)
        {
            Y_XML_SECTION(xml,QueryRates);

            x_score.free();
            for(const OutNode *node=running.head;node;node=node->next)
            {
                const Outcome       &out = **node; if(out.ax.mantissa<=0) continue;
                x_score << out.wr;
            }
            const xreal_t sumWr = x_score.sum();
            Y_XML_COMMENT(xml, "sum of weights=" << sumWr.str() );

            //------------------------------------------------------------------
            //
            //
            // accumulate virtual rates
            //
            //
            //------------------------------------------------------------------
            rate.forEach( & XAdd::free );
            for(const OutNode *node=running.head;node;node=node->next)
            {
                const Outcome       &out = **node; if(out.ax.mantissa<=0) continue;
                const xreal_t        cof = out.wr/sumWr;
                Y_XMLOG(xml, "weight: " << cof.str() << " @" << out.eq.name);
                increaseRates(out.xi * cof,out.eq);
            }
            return optimizedC(xml,S0,10.0,"vr");
        }

        



    }

}


