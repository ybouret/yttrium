
#include "y/text/ascii/embedding.hpp"


namespace Yttrium
{
    namespace ASCII
    {
        const char * const Embedding:: Char[256] =
        {
#include "embedding.hxx"
        };
    }

}

