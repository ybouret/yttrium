
#include "y/chem/reactive/equilibrium.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Equilibrium:: ~Equilibrium() noexcept {}

        xreal Equilibrium:: K(double t)
        {
            const xreal k = getK(t);
            if(k.mantissa<=0) throw Specific:: Exception( name.c_str(), " K(%g)<=0",t);
            return getK(t);
        }


    }

}

namespace Yttrium
{
    namespace Chemical
    {

        ConstEquilibrium:: ~ConstEquilibrium() noexcept {}

        xreal ConstEquilibrium:: getK(double)
        {
            return K_;
        }


    }

}
