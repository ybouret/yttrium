#include "y/ink/parallel/slabs.hpp"

namespace Yttrium
{
    namespace Ink
    {
        Slabs:: ~Slabs() noexcept {}

        Slabs:: Slabs(const Concurrent::SharedLoop &csl) :
        mill(csl),
        simt(mill,& Slab::honorRequest)
        {
        }



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

#include "y/concurrent/loop/mono.hpp"

namespace Yttrium
{
    namespace Ink
    {
        Slabs:: Slabs(const Sequential_ &):
        mill( new Concurrent::Mono() ),
        simt(mill,& Slab::honorRequest)
        {
        }

    }

}


#include "y/concurrent/loop/crew.hpp"

namespace Yttrium
{
    namespace Ink
    {
        

        Slabs:: Slabs(const InParallel_ &):
        mill( Concurrent::Crew::Create() ),
        simt(mill,& Slab::honorRequest)
        {
        }

    }

}
