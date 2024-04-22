
#include "y/chemical/reactive/conservation/group.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            Group:: ~Group() noexcept
            {
            }

            Group:: Group(const Law &first) :
            Object(),
            clList(),
            next(0),
            prev(0)
            {
                (*this) << first;
            }

        }

    }

}
