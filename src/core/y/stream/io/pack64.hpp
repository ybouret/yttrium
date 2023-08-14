
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
        //! [Un]Pack64 bits to/from streams
        class Pack64
        {
        public:
            Pack64(const uint64_t qw) noexcept; //!< pack qw
            ~Pack64() noexcept;                 //!< cleanup


            static size_t   Emit(OutputStream &, const uint64_t);  //!< pack an emit, return written bytes
            static uint64_t Read(InputStream  &, const char *ctx); //!< return unpacked value


        private:
            Y_DISABLE_ASSIGN(Pack64);
            uint16_t size;      //!< encoded
            uint8_t  data[14];  //!< data
            void emit(uint8_t &code, unsigned &bits) noexcept;
        };
    }
}

#endif

