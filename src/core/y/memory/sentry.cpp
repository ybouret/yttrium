#include "y/memory/sentry.hpp"
#include "y/check/crc32.hpp"

#include <iostream>

namespace Yttrium
{

    namespace Memory
    {

        Sentry:: ~Sentry() noexcept
        {
            if(wasModified())
            {
                std::cerr << "*** (@" << addr << "+" << size << ") was modified!" << std::endl;
            }
        }


        static inline
        uint32_t getCode(const void *addr, const size_t size) noexcept
        {
            assert(Good(addr,size));
            return CRC32::Of(addr,size);
        }

        Sentry:: Sentry(const void  *userAddr,
                        const size_t userSize) noexcept :
        addr( userAddr ),
        size( userSize ),
        code( getCode(addr,size) )
        {

        }

        bool Sentry:: isUntouched() const noexcept
        {
            return getCode(addr,size) == code;
        }

        bool Sentry:: wasModified() const noexcept
        {
            return getCode(addr,size) != code;
        }
    }

}

