#include "y/text/ascii/printable.hpp"


namespace Yttrium
{
    namespace ASCII
    {
        const char * const Printable:: Char[256] =
        {
#include "printable.hxx"
        };
    }

}

