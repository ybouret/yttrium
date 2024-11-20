
#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Assembly:: ~Assembly() noexcept
        {
        }

        Assembly:: Assembly() noexcept : maxNameSize(0)
        {
        }

        Assembly:: Assembly(const Assembly &_) noexcept : maxNameSize(_.maxNameSize)
        {
        }
        
    }
}

