//! \file

#ifndef Y_Memory_Fixed_Buffer_Included
#define Y_Memory_Fixed_Buffer_Included 1

#include "y/memory/buffer/rw.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{
    namespace Memory
    {

        //______________________________________________________________________
        //
        //
        //! RW Buffer of fixed length
        //
        //______________________________________________________________________
        template <const size_t N>
        class FixedBuffer : public ReadWriteBuffer
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit FixedBuffer() noexcept : data() { Y_STATIC_ZARR(data); } //!< setup empty
            virtual ~FixedBuffer() noexcept { Y_STATIC_ZARR(data); }          //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            inline virtual const void *ro_addr() const noexcept { return data; } //!< data
            inline virtual size_t      measure() const noexcept { return N;    } //!< N
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(FixedBuffer);
            uint8_t data[N];
        };
    }

}

#endif
