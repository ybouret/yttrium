
//! \file

#ifndef Y_Concurrent_Context_Included
#define Y_Concurrent_Context_Included 1

#include "y/memory/buffer/ro.hpp"

namespace Yttrium
{

    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! Generic parallel context : size, rank, indx, name
        //
        //
        //______________________________________________________________________
        class Context : public Memory::ReadOnlyBuffer
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Context(const size_t sz, const size_t rk) noexcept; //!< size.rank
            explicit Context()                                 noexcept; //!< 1.0
            virtual ~Context()                                 noexcept; //!< cleanup
            Context(const Context &)                           noexcept; //!< copy

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual const void * ro_addr() const noexcept; //!< name
            virtual size_t       measure() const noexcept; //!< len_

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t size;     //!< size
            const size_t rank;     //!< rank
            const size_t indx;     //!< rank+1
            const char   name[48]; //!< size.rank, aligned
            const size_t len_;     //!< strlen(name)

        private:
            Y_DISABLE_ASSIGN(Context);

        };
    }

}

#endif
