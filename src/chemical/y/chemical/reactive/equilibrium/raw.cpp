
#include "y/chemical/reactive/equilibrium/raw.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        RawEquilibrium:: RawEquilibrium(Library &lib, XNode * const root, const size_t i) :
        Equilibrium(lib,root,i),
        Kdata( Compile::Data(root) )
        {

        }

        RawEquilibrium:: ~RawEquilibrium() noexcept
        {

        }
    }

}
