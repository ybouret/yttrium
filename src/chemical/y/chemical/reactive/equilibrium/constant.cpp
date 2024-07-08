
#include "y/chemical/reactive/equilibrium/constant.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        ConstantEquilibrium:: ~ConstantEquilibrium() noexcept {}
        
        xreal_t ConstantEquilibrium:: getK(const xreal_t) { return value; }
    }
}
