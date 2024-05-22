#include "y/ink/parallel/slabs.hpp"

namespace Yttrium
{
    namespace Ink
    {
        Slabs:: ~Slabs() noexcept {}
        

        void Slabs:: split(const Area &area)
        {
            simt.assign(area.lower(),area.upper());
        }


        size_t Slabs:: size() const noexcept { return simt.size(); }

       

        Slab       & Slabs:: operator[](const size_t i)       noexcept { return simt[i]; }
        const Slab & Slabs:: operator[](const size_t i) const noexcept { return simt[i]; }
        const char * Slabs:: callSign()                 const noexcept { return "Ink::Slabs"; }
    }

}

