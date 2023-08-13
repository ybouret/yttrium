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
            // Methods
            //
            //__________________________________________________________________
            virtual const void *ro_addr() const noexcept = 0; //!< readable memory
            virtual size_t      measure() const noexcept = 0; //!< number of readable bytes

            //! compare byte-wise contents
            bool HasSameContentThan(const ReadOnlyBuffer &) const noexcept;

            std::ostream & displayHexadecimal(std::ostream &) const; //!< helper
            OutputStream & displayHexadecimal(OutputStream &) const; //!< helper


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ReadOnlyBuffer);
        };

    }
}

#endif
