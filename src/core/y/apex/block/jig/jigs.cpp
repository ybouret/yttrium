#include "y/apex/block/jig/jigs.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Apex
    {

        Jigs:: Jigs(void * const entry,  const size_t range) noexcept :
        addr(0),
        wksp()
        {
            Coerce(addr) = static_cast<char *>( Memory::OutOfReach::Addr( &wksp[0] ) );
            char  *p = addr;
            new (p)            Jig1(entry,range);
            new (p += JigSize) Jig2(entry,range);
            new (p += JigSize) Jig4(entry,range);
            new (p += JigSize) Jig8(entry,range);
        }

        Jigs:: ~Jigs() noexcept
        {
            memset(wksp,0,sizeof(wksp));
        }

        JigAPI & Jigs:: operator[](const Plan plan) noexcept {
            switch(plan) {
                case Plan1: break;
                case Plan2: return as<Plan2>();
                case Plan4: return as<Plan4>();
                case Plan8: return as<Plan8>();
            }
            return as<Plan1>();
        }


    }

}
