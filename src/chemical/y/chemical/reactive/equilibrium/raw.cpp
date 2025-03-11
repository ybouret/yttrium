
#include "y/chemical/reactive/equilibrium/raw.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        RawEquilibrium:: RawEquilibrium(Library &lib, XNode * const root) :
        Equilibrium(lib,root),
        Kdata( Compile::Data(root) )
        {

        }

        RawEquilibrium:: ~RawEquilibrium() noexcept
        {

        }
    }

}
