
#include "y/memory/out-of-reach.hpp"
#include <cstring>

namespace Yttrium
{

    namespace Memory
    {
        void *OutOfReach:: Addr(void *addr) noexcept
        {
            return addr;
        }

        const void *OutOfReach:: Addr(const void *addr) noexcept
        {
            return addr;
        }

        void *OutOfReach:: Zero(void *addr, const size_t size) noexcept
        {
            assert(Y_Good(addr,size));
            memset(addr,0,size);
            return addr;
        }

        void *OutOfReach:: Move(void *target, const void *source, const size_t size) noexcept
        {
            assert(Y_Good(target,size));
            assert(Y_Good(source,size));
            memmove(target,source,size);
            return target;
        }


        void * OutOfReach:: Swap(void *target, void *source, const size_t size) noexcept
        {
            assert(Y_Good(target,size));
            assert(Y_Good(source,size));

            uint8_t *p = static_cast<uint8_t *>(target);
            uint8_t *q = static_cast<uint8_t *>(source);

            for(size_t i=0;i<size;++i) {
                uint8_t t = p[i];
                p[i]      = q[i];
                q[i]      = t;
            }

            return target;
        }

        void * OutOfReach:: Mov0(void *target, void *source, const size_t size) noexcept
        {
            uint8_t *p = static_cast<uint8_t *>(target);
            uint8_t *q = static_cast<uint8_t *>(source);

            for(size_t i=0;i<size;++i) {
                p[i]      = q[i];
                q[i]      = 0;
            }

            return target;
        }

        bool  OutOfReach:: Are0(const void *addr, const size_t size) noexcept
        {
            assert(Y_Good(addr,size));

            const uint8_t *p = static_cast<const uint8_t *>(addr);
            for(size_t i=0;i<size;++i)
            {
                if( 0 != p[i] ) return false;
            }
            return true;
        }

        ptrdiff_t OutOfReach :: Diff(const void *a, const void *b) noexcept
        {
            const uint8_t *B=static_cast<const uint8_t *>(b);
            const uint8_t *A=static_cast<const uint8_t *>(a);
            return static_cast<ptrdiff_t>(B-A);
        }


    }
}

