
//! \file

#ifndef Y_IO_Stream_Pack64_Included
#define Y_IO_Stream_Pack64_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace IO
    {
        class Pack64
        {
        public:
            Pack64(const uint64_t qw) noexcept;
            ~Pack64() noexcept;

            static uint16_t BytesFor(const uint64_t qw) noexcept;

            const uint16_t size;    //!< in
        private:
            uint8_t        data[14];
            Y_DISABLE_ASSIGN(Pack64);
        };
    }
}

#endif

