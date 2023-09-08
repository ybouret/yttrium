
//! \file

#ifndef Y_Memory_Sentry_Included
#define Y_Memory_Sentry_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //
        //! local sentry to check for modified/untouched memory
        //
        //
        //______________________________________________________________________
        class Sentry
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Sentry(const void  *userAddr,
                            const size_t usersize) noexcept; //!< setup
            virtual ~Sentry()                      noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            bool     isUntouched() const noexcept; //!< test against current code
            bool     wasModified() const noexcept; //!< test against current code
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Sentry);
            const void    *addr;
            const size_t   size;
            const uint32_t code;
        };

        //! create the sentry name
#define Y_SENTRY__(X,Y) X##Y

        //! instantiace the sentry name
#define Y_SENTRY_(ID,ADDR,SIZE) volatile Yttrium::Memory::Sentry Y_SENTRY__(__sentry,ID)(ADDR,SIZE)

        //! create a sentry on a memory region
#define Y_SENTRY(ADDR,SIZE) Y_SENTRY_(__LINE__,ADDR,SIZE)


    }

}

#endif

