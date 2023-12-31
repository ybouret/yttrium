
#include "y/memory/out-of-reach.hpp"
#include "y/type/div.hpp"

#include <cstring>
#include <cstdlib>

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
            assert(Good(addr,size));
            memset(addr,0,size);
            return addr;
        }

        void *OutOfReach:: Move(void *target, const void *source, const size_t size) noexcept
        {
            assert(Good(target,size));
            assert(Good(source,size));
            memmove(target,source,size);
            return target;
        }

        void *OutOfReach:: Copy(void *target, const void *source, const size_t size) noexcept
        {
            assert(Good(target,size));
            assert(Good(source,size));
            memcpy(target,source,size);
            return target;
        }

        int OutOfReach:: Comp(const void *a, const void *b, const size_t size) noexcept
        {
            assert(Good(a,size));
            assert(Good(b,size));
            return memcmp(a,b,size);
        }

        void * OutOfReach:: Swap(void *target, void *source, const size_t size) noexcept
        {
            assert(Good(target,size));
            assert(Good(source,size));

            uint8_t *p = static_cast<uint8_t *>(target);
            uint8_t *q = static_cast<uint8_t *>(source);

            for(size_t i=0;i<size;++i) {
                const uint8_t t = p[i];
                p[i]            = q[i];
                q[i]            = t;
            }

            return target;
        }

        void * OutOfReach:: Grab(void *target, void *source, const size_t size) noexcept
        {
            assert(Good(target,size));
            assert(Good(source,size));

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
            assert(Good(addr,size));

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


        void * OutOfReach:: Haul(void *addr, ptrdiff_t delta) noexcept
        {
            uint8_t *p = static_cast<uint8_t *>(addr);
            return p+delta;
        }

        void OutOfReach:: Lift(void *addr, size_t size, const size_t blockSize) noexcept
        {
            assert(0!=addr);
            assert(size>0);
            assert(blockSize>0);
            {
                uint8_t *p = static_cast<uint8_t *>(addr)+size;
                uint8_t *q = p+blockSize;
                for(size_t i=size;i>0;--i)
                {
                    *(--q) = *(--p);
                }
            }
            memset(addr,0,blockSize);

        }

        void OutOfReach:: Move(void *a, void *b, const void *c, const size_t size)          noexcept
        {
            assert( Good(a,size) );
            assert( Good(b,size) );
            assert( Good(c,size) );
            uint8_t *       p = static_cast<uint8_t *>(a);
            uint8_t *       q = static_cast<uint8_t *>(b);
            const uint8_t * r = static_cast<const uint8_t *>(c);
            for(size_t i=0;i<size;++i)
            {
                p[i] = q[i];
                q[i] = r[i];
            }


        }

        void OutOfReach:: Fill(void *       tgtAddr,
                               const size_t tgtSize,
                               const void  *srcAddr,
                               const size_t srcSize) noexcept
        {
            assert(Good(tgtAddr,tgtSize));
            assert(Good(srcAddr,srcSize));

            if(srcSize>=tgtSize)
            {
                memcpy(tgtAddr,srcAddr,tgtSize);
            }
            else
            {
                assert(srcSize<tgtSize);
                typedef Div<unit_t> DivAPI;
                static const DivAPI _;
                DivAPI::Type    dv = _.call(tgtSize,srcSize);
                uint8_t *       p  = static_cast<uint8_t *>(tgtAddr);
                const uint8_t * q  = static_cast<const uint8_t *>(srcAddr);
                while( dv.quot-- > 0)
                {
                    memcpy(p,q,srcSize);
                    p += srcSize;
                }
                while( dv.rem-- > 0 )
                {
                    *(p++) = *(q++);
                }
            }
        }

    }
}

