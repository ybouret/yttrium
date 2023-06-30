
#include "y/memory/arena.hpp"
#include "y/memory/pages.hpp"
#include "y/calculus/align.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{

    namespace Memory
    {

        Arena:: ~Arena() noexcept
        {
        }

        Arena:: Arena(const size_t  userBlockSize,
                      Pages        &userDataPages) :
        ListOf<Chunk>(),
        blockSize(userBlockSize),
        dataPages(userDataPages)
        {

        }


        unsigned Arena:: ComputeShift(const size_t blockSize,
                                      const size_t pageBytes) noexcept
        {
            assert(blockSize>0);
            
            return 0;
        }
    }

}
