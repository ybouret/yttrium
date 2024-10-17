//! \file

#ifndef Y_Memory_RO_Buffer_Included
#define Y_Memory_RO_Buffer_Included 1

#include "y/config/starting.hpp"
#include <iosfwd>

namespace Yttrium
{
    class OutputStream;

    namespace Memory
    {

        //______________________________________________________________________
        //
        //
        //! Read-Only Buffer Interface
        //
        //______________________________________________________________________
        class ReadOnlyBuffer
        {
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            explicit ReadOnlyBuffer() noexcept; //!< setup
        public:
            virtual ~ReadOnlyBuffer() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const void *ro_addr() const noexcept = 0; //!< readable memory
            virtual size_t      measure() const noexcept = 0; //!< number of readable bytes

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            bool     hasSameContentThan(const ReadOnlyBuffer &) const noexcept; //!< compare byte-wise contents
            size_t   maxSameContentThan(const ReadOnlyBuffer &) const noexcept; //!< maximum byte-wise same content
            uint32_t crc32() const noexcept; //!< fast crc32

            //__________________________________________________________________
            //
            //
            // Ouput helpers
            //
            //__________________________________________________________________
            std::ostream & displayHexadecimal(std::ostream &) const; //!< helper
            OutputStream & displayHexadecimal(OutputStream &) const; //!< helper


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ReadOnlyBuffer);
        };

    }
}

#endif
