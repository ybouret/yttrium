//! \file

#ifndef Y_Memory_RO_Buffer_Included
#define Y_Memory_RO_Buffer_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Memory
    {

        class ReadOnlyBuffer
        {
        protected:
            explicit ReadOnlyBuffer() noexcept;
        public:
            virtual ~ReadOnlyBuffer() noexcept;

            virtual const void *ro_addr() const noexcept = 0;
            virtual size_t      measure() const noexcept = 0;

            bool HasSameContentThan(const ReadOnlyBuffer &) const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ReadOnlyBuffer);
        };

    }
}

#endif
