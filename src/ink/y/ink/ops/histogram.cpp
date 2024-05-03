#include "y/ink/ops/histogram.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Ink
    {
        namespace Crux
        {
            Histogram:: ~Histogram() noexcept
            {
            }

            Histogram:: Histogram() noexcept : lower(0), upper(0)
            {
            }
            
            void Histogram:: ldz_(void *const blocks, const size_t blockSize) const noexcept
            {
                assert(0!=blocks);
                assert(blockSize>0);
                memset(blocks,0,blockSize*BINS);
                Coerce(lower) = 0;
                Coerce(upper) = BINS-1;
            }
            
        }

    }

}

