
#include "y/memory/wad.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Memory
    {
        namespace Crux
        {
            Wad:: ~Wad() noexcept {
                assert(0==workspace);
                assert(0==capacity);
                assert(0==numBytes);
            }

            Wad:: Wad(Allocator   &allocator,
                      const size_t numBlocks,
                      const size_t blockSize) noexcept :
            workspace(0),
            capacity(0),
            numBytes(0)
            {
                assert(blockSize>0);
                size_t count     = Max<size_t>(numBlocks,1);
                workspace        = allocator.acquire(count,blockSize);
                Coerce(capacity) = (Coerce(numBytes) = count) / blockSize;
                assert(capacity>=numBlocks);
            }


            void Wad:: returnTo(Allocator &allocator) noexcept
            {
                assert(0!=workspace);
                allocator.release(workspace, Coerce(numBytes));
                Coerce(capacity) = 0;
            }

            void *Wad:: WalkDown(void *addr, const size_t blockSize) noexcept
            {
                assert(blockSize>0);
                return static_cast<char *>(addr) - blockSize;
            }
        }
    }

}
