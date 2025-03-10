
#include "y/chemical/reactive/equilibrium.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        const char * const Equilibrium:: CallSign = "Equilibrium";
        const char * const Equilibrium:: Label = "Label";
        const char * const Equilibrium:: Reac  = "Reac";
        const char * const Equilibrium:: Prod  = "Prod";


        Equilibrium:: ~Equilibrium() noexcept
        {
        }

        xreal_t Equilibrium:: K(xreal_t t)
        {
            const xreal_t zero;
            const xreal_t value = getK(t);
            if(value<=zero) throw Specific::Exception( key().c_str(), "K(%g)=%g", double(t), double(value));
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

        xreal_t ConstEquilibrium:: getK(xreal_t)
        {
            return Konst;
        }
    }

}

