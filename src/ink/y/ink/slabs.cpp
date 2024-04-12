#include "y/ink/slabs.hpp"

namespace Yttrium
{
    namespace Ink
    {
        Slabs:: ~Slabs() noexcept {}

        Slabs:: Slabs(const Concurrent::SharedLoop &csl) : Concurrent::SIMT<Slab>(csl) {}


        void Slabs:: split(const Area &area)
        {
            assign(area.lower(),area.upper());
        }


    }

}
