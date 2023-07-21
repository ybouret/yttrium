
//! \file

#ifndef Y_Memory_Buffer_Of_Included
#define Y_Memory_Buffer_Of_Included 1

#include "y/memory/buffer/rw.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/memory/wad.hpp"
#include "y/type/args.hpp"

namespace Yttrium
{
    namespace Memory
    {

        //______________________________________________________________________
        //
        //
        //
        //! helper for local memory needs
        //
        //
        //______________________________________________________________________
        template <typename T, typename ALLOCATOR = Pooled>
        class BufferOf : public ReadWriteBuffer, public Wad<T,ALLOCATOR>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_ARGS_EXPOSE(T);                 //!< aliases
            typedef Wad<T,ALLOCATOR> WadType; //!< alias
            using WadType::workspace;
            using WadType::maxBlocks;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! enough memory for T[n]
            explicit BufferOf(const size_t n):
            ReadWriteBuffer(),
            WadType(n),
            wksp( static_cast<MutableType*>(workspace) ),
            wlen( maxBlocks * sizeof(T)      )
            {
            }

            //! cleanup
            virtual ~BufferOf() noexcept { }

            //__________________________________________________________________
            //
            //
            // methods
            //
            //__________________________________________________________________
            inline virtual const void *ro_addr() const noexcept { return wksp;  }
            inline virtual size_t      measure() const noexcept { return wlen;  }

            //! safe access
            inline Type &operator[](const size_t i) noexcept {
                assert(i<maxBlocks);
                return wksp[i];
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(BufferOf);
            MutableType       *wksp;
            const size_t       wlen;
        };
    }

}

#endif
