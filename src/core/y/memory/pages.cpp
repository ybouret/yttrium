
#include "y/memory/pages.hpp"
#include "y/type/utils.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{

    namespace Memory
    {
        Pages:: ~Pages() noexcept
        {

        }

        static size_t checkShift(const size_t userShift)
        {
            if(userShift<Pages::MinShift)
                return Pages::MinShift;

            if(userShift>Pages::MaxShift)
                throw Specific::Exception("Memory::Pages","2^%lu exceeds capacity",(unsigned long)userShift);

            return userShift;
        }

        Pages:: Pages(const size_t userShift) :
        ListOf<Page>(),
        shift( checkShift(userShift) ),
        bytes( Base2<size_t>::One << shift )
        {
        }
        
    }

}
