
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
                assert(0==maxBlocks);
                assert(0==allocated);
            }

            Wad:: Wad(Allocator   &allocator,
                      const size_t numBlocks,
                      const size_t blockSize) noexcept :
            workspace(0),
            maxBlocks(0),
            allocated(0)
            {
                assert(blockSize>0);
                size_t count  = Max<size_t>(numBlocks,1);
                workspace      = allocator.acquire(count,blockSize);
                Coerce(maxBlocks) = (Coerce(allocated) = count) / blockSize;
                assert(maxBlocks>=numBlocks);
            }


            void Wad:: returnTo(Allocator &allocator) noexcept
            {
                assert(0!=workspace);
                allocator.release(workspace, Coerce(allocated) );
                Coerce(maxBlocks) = 0;
            }

        }
    }

}
