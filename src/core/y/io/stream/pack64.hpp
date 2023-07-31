
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
        class Pack64
        {
        public:
            static const unsigned InfoBits = iLog2<64>::Value;
            static const unsigned InfoRoom = 8-InfoBits;
            static const unsigned InfoMask = (1<<InfoRoom) - 1;
            Pack64(const uint64_t qw) noexcept;
            ~Pack64() noexcept;

            static size_t   Emit(OutputStream &, const uint64_t);
            static uint64_t Read(InputStream &, const char *ctx);

            const uint16_t size;    //!< in
            const uint8_t  data[14];
        private:
            Y_DISABLE_ASSIGN(Pack64);
            void emit(uint8_t &code, unsigned &bits) noexcept;
        };
    }
}

#endif

