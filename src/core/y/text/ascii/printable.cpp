#include "y/text/ascii/printable.hpp"


namespace Yttrium
{
    namespace ASCII
    {
        const char * const Printable:: Char[256] =
        {
#include "printable.hxx"
        };

        const char * Printable:: Text(const char c) noexcept
        {
            return Char[ static_cast<uint8_t>(c) ];
        }
    }

}

