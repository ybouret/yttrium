
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
                                      unit_t * const width) noexcept :
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
        }

        bool LayoutMetrics:: validateLayout(const unit_t * const lower,
                                            const unit_t * const upper,
                                            const unit_t * const width) const noexcept
        {
            static const char fn[] = "Field::LayoutMetrics ";
            assert(0!=lower);
            assert(0!=upper);
            assert(0!=width);

            size_t count = 1;
            for(unsigned i=0;i<dimension;++i)
            {
                const unit_t lo = lower[i];
                const unit_t up = upper[i];
                if(up<lo) {
                    std::cerr << fn << "dimension#" << i << " up=" << up << ">lo=" << lo << std::endl;
                    return false;
                }
                const unit_t w = width[i];
                const unit_t n = 1+up-lo;
                if(w != n)
                {
                    std::cerr << fn << "dimension#" << i << " invalid width=" << w << "/" << n << std::endl;
                    return false;
                }
                count *= w;
            }
            if(count!=items)
            {
                std::cerr << fn << " invalid count=" << count << "/" << items << std::endl;
                return false;
            }
            return true;
        }

        LayoutMetrics:: LayoutMetrics(const LayoutMetrics &layout) noexcept :
        dimension(layout.dimension),
        items(layout.items)
        {
        }
        
        LayoutMetrics:: LayoutMetrics(const unsigned d, const size_t n) noexcept :
        dimension(d),
        items(n)
        {
            assert(d>0);
            assert(items>0);
        }

    }

}

