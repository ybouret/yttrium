

#include "y/chemical/reactive/equilibrium.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Equilibrium:: ~Equilibrium() noexcept
        {

        }

        xReal Equilibrium:: K(xReal t) const
        {
            const xReal value = getK(t);
            if(value<=0.0) throw Specific::Exception(name.c_str(), "negative constant @%g", real_t(t));
            return value;
        }

    }

}

namespace Yttrium
{
    namespace Chemical
    {
        ConstEquilibrium:: ~ConstEquilibrium() noexcept
        {
        }

        xReal ConstEquilibrium:: getK(xReal) const
        {
            return value;
        }
    }
}
