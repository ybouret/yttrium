
#include "y/apex/number.hpp"
#include "y/stream/io/chars.hpp"
#include "y/memory/buffer/of.hpp"

namespace Yttrium
{
    namespace Apex
    {

        Number:: ~Number() noexcept
        {
        }

        Number:: Number() noexcept
        {
        }

        void Number:: Display(std::ostream &os, IO::Chars &cache)
        {
            //------------------------------------------------------------------
            // make a C-style buffer
            //------------------------------------------------------------------
            Memory::BufferOf<char,Memory::Pooled> buffer(cache.size+1);
            {
                size_t i = 0;
                while(cache.size) buffer[i++] = cache.pullHead();
                assert(0==buffer[i]);
                assert(0==cache.size);
            }

            //------------------------------------------------------------------
            // done
            //------------------------------------------------------------------
            os << &buffer[0];
        }

    }

}
