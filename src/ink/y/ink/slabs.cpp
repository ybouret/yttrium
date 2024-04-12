#include "y/ink/slabs.hpp"

namespace Yttrium
{
    namespace Ink
    {
        Slabs:: ~Slabs() noexcept {}

        Slabs:: Slabs(const Concurrent::SharedLoop &csl) :simt(csl) {}


        void Slabs:: split(const Area &area)
        {
            simt.assign(area.lower(),area.upper());
        }


    }

}
