#include "y/color/ramp/gradation.hpp"
#include "y/type/utils.hpp"
#include "y/color/conv.hpp"


namespace Yttrium
{
    namespace Color
    {
        Gradation:: ~Gradation() noexcept {}
        Gradation::  Gradation(const RampColor * const entry,
                               const size_t            count) noexcept :
        head(entry),
        size(count),
        imax(size-1),
        tail(head+imax)
        {
            assert(0!=head);
            assert(0!=tail);
            assert(size>0);
        }

        static inline uint8_t Mix(const uint8_t clo, const float wlo,
                                  const uint8_t cup, const float wup) noexcept
        {
            return Conv<float>:: ToByte( Conv<float>::Unit[clo]*wlo + Conv<float>::Unit[cup] * wup );
        }

        static inline RampColor Mix(const RampColor &clo, const float wlo,
                                    const RampColor &cup, const float wup) noexcept
        {
            return RampColor(Mix(clo.r,wlo,cup.r,wup),
                             Mix(clo.g,wlo,cup.g,wup),
                             Mix(clo.b,wlo,cup.b,wup)
                             );
        }

        RampColor Gradation:: operator()(const float x) const noexcept
        {
            if(x<=0.0f)
                return *head;
            else
            {
                if(x>=1.0f)
                {
                    return *tail;
                }
                else
                {
                    const float  scaling = x * imax; // in [0:imax]
                    const float  nearest = floorf(scaling+0.5f);
                    const size_t closest = static_cast<size_t>(nearest);
                    const size_t upper   = Min(imax,((nearest>=scaling) ? closest : closest+1));
                    const size_t lower   = upper-1;
                    const float  upperWeight = scaling - static_cast<float>(lower);
                    const float  lowerWeight = 1.0f - upperWeight;
                    return Mix(head[lower],lowerWeight,head[upper],upperWeight);
                }
            }
        }

    }
}
