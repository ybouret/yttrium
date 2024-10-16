
//! \file

#ifndef Y_Memory_Zone_Included
#define Y_Memory_Zone_Included 1

#include "y/memory/buffer/ro.hpp"

namespace Yttrium
{

    namespace Memory
    {

        //! Zone on a PERSISTENT memory area
        class Zone : public ReadOnlyBuffer
        {
        public:
            explicit Zone(const void * _addr,
                          const size_t _size) noexcept;
            virtual ~Zone() noexcept;
            explicit Zone(const char * const text) noexcept;

            virtual const void * ro_addr() const noexcept;
            virtual size_t       measure() const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Zone);
            const void * const addr;
            const size_t       size;
        };
    }

}

#endif

