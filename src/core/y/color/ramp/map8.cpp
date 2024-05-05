
#include "y/color/ramp/map8.hpp"
#include "y/color/conv.hpp"

namespace Yttrium
{
    namespace Color
    {

        Map8:: ~Map8() noexcept
        {
        }

        Map8:: Map8(const Ramp &user) noexcept : ramp(user)
        {
        }

        RampColor Map8:: operator()(const uint8_t u) const noexcept
        {
            return ramp( Conv<float>::Unit[u] );
        }
    }

}
