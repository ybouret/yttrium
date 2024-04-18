
#include "y/chemical/reactive/equilibrium/constant.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        ConstantEquilibrium::~ConstantEquilibrium() noexcept
        {
        }

        XReal ConstantEquilibrium:: getK(Real) noexcept
        {
            return K_;
        }

    }

}
