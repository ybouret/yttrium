#include "y/color/gradation.hpp"
#include "y/type/utils.hpp"
#include "y/color/conv.hpp"


namespace Yttrium
{
    namespace Color
    {
        Gradation:: ~Gradation() noexcept {}
        Gradation::  Gradation(const RampColor * const entry,
                               const size_t            count) noexcept :
        cmin(entry),
        size(count),
        imax(size-1),
        cmax(cmin+imax)
        {
            assert(0!=cmin);
            assert(0!=cmax);
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
                return cmin[0];
            else
            {
                if(x>=1.0f)
                {
                    return cmax[0];
                }
                else
                {
                    const float     scaling = imax*x;
                    const float     nearest = floorf(scaling+0.5f);
                    const size_t    inear   = static_cast<size_t>(nearest);
                    const size_t    upper   = Max(imax,(nearest>=scaling) ? inear : inear+1);
                    const size_t    lower   = upper-1;
                    const RampColor cLo     = cmin[lower];
                    const RampColor cUp     = cmin[upper];
                    const float     wLo     = Clamp<float>(0.0f,static_cast<float>(upper)-scaling,1.0f);
                    const float     wUp     = 1.0f-wLo;
                    return Mix(cLo,wLo,cUp,wUp);
                }
            }
        }

    }
}
