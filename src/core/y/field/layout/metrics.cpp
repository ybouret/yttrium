
#include "y/field/layout/metrics.hpp"
#include <iostream>

namespace Yttrium
{

    namespace Field
    {

        LayoutMetrics:: ~LayoutMetrics() noexcept
        {
            Y_STATIC_ZVAR(Coerce(items));
            Y_STATIC_ZVAR(Coerce(dimension));
        }
        
        LayoutMetrics:: LayoutMetrics(const unsigned d,
                                      unit_t * const lower,
                                      unit_t * const upper,
                                      unit_t * const width,
                                      unit_t * const shift) noexcept :
        dimension(d),
        items(1)
        {
            assert(0!=lower);
            assert(0!=upper);
            assert(0!=width);

            for(unsigned i=0;i<dimension;++i)
            {
                unit_t &lo = lower[i];
                unit_t &up = upper[i];
                if(up<lo) Swap(up,lo);
                Coerce(items) *= (width[i] = 1+up-lo);
            }

            Coerce(shift[0]) = width[0];
            for(unsigned i=1;i<dimension;++i)
            {
                Coerce(shift[i]) = width[i] * shift[i-1];
            }
        }


        LayoutMetrics:: LayoutMetrics(const LayoutMetrics &layout) noexcept :
        dimension(layout.dimension),
        items(layout.items)
        {
        }
        
        

    }

}

