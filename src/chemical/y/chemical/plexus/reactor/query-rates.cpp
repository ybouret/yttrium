
#include "y/chemical/plexus/reactor.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        xreal_t Reactor:: queryRates(XMLog &xml, const xreal_t S0)
        {
            static const char fn[] = "QueryRates";
            Y_XML_SECTION(xml,fn);

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
                increaseRates(out.xi,out.eq);
            }
            return optimizedC(xml,S0,10.0,"vr");
        }


      


    }

}


