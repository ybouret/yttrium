
//! \file

#ifndef Y_Memory_Buffer_Of_Included
#define Y_Memory_Buffer_Of_Included 1

#include "y/memory/buffer/rw.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/memory/allocator/pooled.hpp"

namespace Yttrium
{
    namespace Memory
    {

        template <typename T, typename ALLOCATOR = Pooled>
        class BufferOf : public ReadWriteBuffer
        {
        public:

            //! enough memory for T[n]
            explicit BufferOf(const size_t n) :
            items(n),
            bytes(items*sizeof(T)),
            mgr( ALLOCATOR::Instance() ),
            wlen( items ),
            wksp( static_cast<T*>(mgr.acquire(wlen,sizeof(T)) ) )
            {   }

            virtual ~BufferOf() noexcept { mgr.release(*(void **)&wksp,wlen);   }

            inline virtual const void *ro_addr() const noexcept { return wksp;  }
            inline virtual size_t      measure() const noexcept { return bytes;  }

            inline T &operator[](const size_t i) noexcept {
                assert(i<items);
                return wksp[i];
            }

            const size_t       items;
            const size_t       bytes;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(BufferOf);
            Memory::Allocator &mgr;
            size_t             wlen;
            T                 *wksp;
        };
    }

}

#endif
