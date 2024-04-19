
#include "y/chemical/reactive/equilibrium.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Equilibrium:: ~Equilibrium() noexcept
        {
        }
        
        XReal Equilibrium:: K(Real t)
        {
            static const XReal _0(0);
            const XReal k = getK(t);
            if(k<=_0) throw Specific::Exception("Chemical::Equilibrium", "%s.K=%g at t=%g", name.c_str(), double(k), double(t) );
            return k;
        }

      
    }

}
