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

            Histogram:: Histogram() noexcept
            {
            }
            
            void Histogram:: ldz_(void *const blocks, const size_t blockSize) const noexcept
            {
                assert(0!=blocks);
                assert(blockSize>0);
                memset(blocks,0,blockSize*BINS);
            }
            
        }

    }

}

