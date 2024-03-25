
#include "y/type/binder.hpp"
#include <cstring>

namespace Yttrium
{

    namespace Core
    {
        void InitBlock<true>::At(const void * blockAddr,
                              const size_t blockSize) noexcept
        {
            assert( Good(blockAddr,blockSize) );
            std::cerr << "memset  [" << blockSize << "]" << std::endl;
            memset( (void*) blockAddr, 0, blockSize);
        }

        void InitBlock<false>::At(const void * , const size_t ) noexcept
        {
            //std::cerr << "untouch [" << blockSize << "]" << std::endl;
        }
    }

}

