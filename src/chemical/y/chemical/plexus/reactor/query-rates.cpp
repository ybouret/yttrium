
#include "y/chemical/plexus/reactor.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        const char * const Reactor:: QueryRates = "QueryRates";

        xreal_t Reactor:: queryRates(XMLog &xml, const xreal_t S0)
        {
            Y_XML_SECTION(xml,QueryRates);


            //------------------------------------------------------------------
            //
            //
            // Get Maximum Rate to rescale
            //
            //
            //------------------------------------------------------------------
            xreal_t       maxWr = 0;
            for(OutNode *node=tighten.head;node;node=node->next)
            {
                Outcome &out = **node;
                InSituMax(maxWr,out.wr);
            }

            //------------------------------------------------------------------
            //
            //
            // accumulate weighted virtual rates
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

            //------------------------------------------------------------------
            //
            //
            // Optimize with one order of magnitude search
            //
            //
            //------------------------------------------------------------------
            return optimizedC(xml,S0,10.0,"vr");
        }

        



    }

}


