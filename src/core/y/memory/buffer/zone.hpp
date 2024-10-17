
//! \file

#ifndef Y_Memory_Zone_Included
#define Y_Memory_Zone_Included 1

#include "y/memory/buffer/ro.hpp"

namespace Yttrium
{

    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //
        //! Zone on a PERSISTENT memory area
        //
        //
        //______________________________________________________________________
        class Zone : public ReadOnlyBuffer
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Zone(const void * _addr, const size_t _size) noexcept; //!< setup on _addr[_size]
            explicit Zone(const char * const text)                noexcept; //!< setup on _text
            virtual ~Zone() noexcept;                                       //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const void * ro_addr() const noexcept; //!< [ReadOnlyBuffer] addr
            virtual size_t       measure() const noexcept; //!< [ReadOnlyBuffer] size

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Zone);
            const void * const addr;
            const size_t       size;
        };
    }

}

#endif

