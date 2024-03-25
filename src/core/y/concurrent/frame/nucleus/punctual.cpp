
#include "y/concurrent/frame/nucleus/punctual.hpp"
#include <iostream>

namespace Yttrium
{
    namespace Concurrent
    {
        namespace Nucleus
        {

            Punctual:: Punctual() noexcept
            {
            }

            Punctual:: ~Punctual() noexcept
            {
            }

            std::ostream & operator<<(std::ostream &os, const Punctual &)
            {
                os << "(punctual)";
                return os;
            }
        }

    }

}

