
#include "y/color/ramp/flexible.hpp"

namespace Yttrium
{
    namespace Color
    {

        FlexibleRamp:: ~FlexibleRamp() noexcept
        {
        }

        FlexibleRamp:: FlexibleRamp(const Ramp &user,
                                    const float fmin,
                                    const float fmax) :
        Ramp(),
        ramp(user),
        vmin(fmin),
        vmax(fmax),
        delv(fmax-fmin)
        {
            assert(fmin<=fmax);
            
        }

        RampColor FlexibleRamp:: operator()(const float v) const noexcept
        {
            if(v<=vmin)
                return ramp(0);
            else
            {
                if(v>=vmax)
                    return ramp(1);
                else
                {
                    return ramp((v-vmin)/delv);
                }
            }
        }

    }

}
