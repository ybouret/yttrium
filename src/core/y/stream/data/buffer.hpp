
//! \file

#ifndef Y_STREAM_DATA_BUFFER_INCLUDED
#define Y_STREAM_DATA_BUFFER_INCLUDED 1

#include "y/memory/buffer/rw.hpp"
#include "y/calculus/align.hpp"
#include <cstring>

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Fixed-Size buffer acting like a short sequence
    //
    //
    //__________________________________________________________________________
    template <size_t N>
    class DinkyBuffer : public Memory::ReadWriteBuffer
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        static const size_t Capacity = N;                             //!< working capcity
        static const size_t Required = Capacity+1;                    //!< add one for C-string like
        static const size_t Internal = Y_ALIGN_TO(void *,Required);   //!< internal length

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        inline explicit DinkyBuffer() noexcept : Memory::ReadWriteBuffer(), size(0), data() { clr(); } //!< setup
        inline virtual ~DinkyBuffer() noexcept   { erase(); }                                          //!< cleanup

        //______________________________________________________________________
        //
        //
        // Interface
        //
        //______________________________________________________________________
        virtual inline const void *ro_addr() const noexcept { return data; }                         //!< internal
        virtual inline size_t      measure() const noexcept { assert(size<=Capacity); return size; } //!< current size

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //________ ______________________________________________________________
        inline void erase()               noexcept { size=0;  clr(); }                      //!< erase content
        inline void adopt(const size_t n) noexcept { assert(n<=Capacity); size=n; clr(); }  //!< prepare for size=n

        //! append char
        inline DinkyBuffer & operator<<(const char c) noexcept {
            assert(size<Capacity);
            data[size++] = c;
            return *this;
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(DinkyBuffer);
        inline void clr() noexcept { memset(data,0,sizeof(data)); }
        size_t size;
        char   data[Internal];
    };

}

#endif

