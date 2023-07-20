
#include "y/container/interface.hpp"

namespace Yttrium
{

    Container:: ~Container() noexcept
    {
    }

    Container:: Container() noexcept
    {
        
    }

    size_t Container:: available() const noexcept
    {
        assert(size()<=capacity());
        return capacity()-size();
    }

}
