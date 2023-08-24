
#include "y/jive/position.hpp"

namespace Yttrium
{
    namespace Jive
    {
        Position:: ~Position() noexcept
        {

        }

        Position:: Position() noexcept :
        line(1),
        ncol(1)
        {
        }

        void Position:: newChar() noexcept
        {
            ++Coerce(ncol);

        }

        void Position:: newLine() noexcept
        {
            Coerce(line)++;
            Coerce(ncol)=1;
        }
    }

}

