
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
            for(const OutNode *node=tighten.head;node;node=node->next)
            {
                const Outcome       &out = **node;
                assert(out.sc<S0);
                x_score << out.wr;
            }
            const xreal_t sumWr = x_score.sum();
            Y_XML_COMMENT(xml, "sum of weights=" << sumWr.str() );

            xreal_t maxWr = 0;
            for(OutNode *node=tighten.head;node;node=node->next)
            {
                Outcome &out = **node;
                InSituMax(maxWr,out.wr /= sumWr);
            }

            //------------------------------------------------------------------
            //
            //
            // accumulate virtual rates
            //
            //
            //------------------------------------------------------------------
            rate.forEach( & XAdd::free );
            for(const OutNode *node=tighten.head;node;node=node->next)
            {
                const Outcome       &out = **node;
                const xreal_t        cof = out.wr/maxWr;
                Y_XMLOG(xml, "weight= " << std::setw(22) << cof.str() << " // " << out.eq.name);
                increaseRates(out.xi * cof,out.eq);
            }
            return optimizedC(xml,S0,10.0,"vr");
        }

        



    }

}


