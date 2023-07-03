
#include "y/memory/legacy.hpp"
#include "y/lockable.hpp"
#include "y/system/exception.hpp"
#include "y/system/error.hpp"
#include "y/type/addition.hpp"

#include <cerrno>

namespace Yttrium
{

    namespace Memory
    {


        static uint64_t RAM = 0;

        Legacy:: Legacy() :
        giant( Lockable::Giant() )
        {

        }

        Legacy:: ~Legacy() noexcept
        {

        }



        void * Legacy:: acquire(const size_t blockSize)
        {
            Y_LOCK(giant);
            if( blockSize )
            {
                const uint64_t newRam = Addition::Of(RAM, blockSize, "Legacy::acquire");
                void *         handle = calloc(1,blockSize);
                if(!handle)
                    throw Libc::Exception(ENOMEM,"Legacy::acquire(%lu)", (unsigned long)blockSize);

                RAM  = newRam;
                return handle;
            }
            else
            {
                return 0;
            }
        }

        void Legacy:: release(void *blockAddr, const size_t blockSize) noexcept
        {
            Y_LOCK(giant);
            if(blockSize)
            {
                assert(0!=blockAddr);
                if(blockSize>RAM) Libc::CriticalError(EINVAL, "Legacy::release(blockSize>RAM)");
                RAM -= blockSize;
                free(blockAddr);
            }
            else
            {
                assert(0==blockAddr);
            }

        }

    }

}

