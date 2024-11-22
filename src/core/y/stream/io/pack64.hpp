
//! \file

#ifndef Y_IO_Stream_Pack64_Included
#define Y_IO_Stream_Pack64_Included 1

#include "y/calculus/ilog2.hpp"

namespace Yttrium
{
    class InputStream;
    class OutputStream;

    namespace IO
    {
        //______________________________________________________________________
        //
        //
        //
        //! [Un]Pack64 bits to/from streams
        //
        //
        //______________________________________________________________________
        class Pack64
        {
        public:
            Pack64(const uint64_t qw) noexcept; //!< pack qw
            ~Pack64() noexcept;                 //!< cleanup


            static size_t   Emit(OutputStream &, const uint64_t);         //!< pack and emit, return written bytes
            static uint64_t Read(InputStream  &, const char * const ctx); //!< return unpacked value

            const uint16_t size;      //!< encoded (2 bytes)
            const uint8_t  data[14];  //!< data : need 9 bytes, rounded to 14 => sizeof(Pack64) = 16

        private:
            Y_DISABLE_ASSIGN(Pack64);
            void emit(uint8_t &code, unsigned &bits) noexcept;
            void zset() noexcept;
        };
    }
}

#endif

