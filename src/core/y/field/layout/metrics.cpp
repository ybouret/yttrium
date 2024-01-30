
#include "y/field/layout/metrics.hpp"

namespace Yttrium
{

    namespace Field
    {

        LayoutMetrics:: ~LayoutMetrics() noexcept
        {
        }
        
        LayoutMetrics:: LayoutMetrics(const unsigned d,
                                      unit_t * const lower,
                                      unit_t * const upper,
                                      unit_t * const width) noexcept :
        dimension(d),
        items(1)
        {
            for(unsigned i=0;i<dimension;++i)
            {
                unit_t &lo = lower[i];
                unit_t &up = upper[i];
                if(up<lo) Swap(up,lo);
                Coerce(items) *= (width[i] = 1+up-lo);
            }
        }

        LayoutMetrics:: LayoutMetrics(const LayoutMetrics &layout) noexcept :
        dimension(layout.dimension),
        items(layout.items)
        {
        }
        

    }

}

