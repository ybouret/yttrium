#include "y/ink/bitmap.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

namespace Yttrium
{

    namespace Ink
    {

        struct BitRow
        {
            void  *entry;
            size_t width;
        };

        template <typename T>
        struct PixRow
        {
            T * const entry;
            const size_t width;
        };

    }

}

Y_UTEST(bitmap)
{
    
    

}
Y_UDONE()
