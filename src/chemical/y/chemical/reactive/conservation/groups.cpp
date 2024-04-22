
#include "y/chemical/reactive/conservation/groups.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            Groups:: Groups() : cgl()
            {

            }

            Groups:: ~Groups() noexcept
            {
            }

            Groups::ConstInterface & Groups::surrogate() const noexcept
            {
                return cgl;
            }

        }

    }

}
