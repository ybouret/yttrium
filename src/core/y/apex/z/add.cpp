#include "y/apex/integer.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Integer Integer:: operator+() const
        {
            return Integer(*this);
        }
    }

}

