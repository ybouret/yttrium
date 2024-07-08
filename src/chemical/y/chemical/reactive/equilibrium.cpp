
#include "y/chemical/reactive/equilibrium.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Equilibrium:: ~Equilibrium() noexcept
        {
        }


        xreal_t Equilibrium:: K(const xreal_t t)
        {
            const xreal_t value = getK(t);
            if(value<=zero) throw Specific::Exception( name.c_str(), "negative K at t=%g", double(real_t(t)));
            return value;
        }
    }

}
