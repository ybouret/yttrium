//! \file
//!
#ifndef Y_Concurrent_WIN32_Semaphore_Included
#define Y_Concurrent_WIN32_Semaphore_Included 1

#include "y/config/starting.hpp"

#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace Yttrium
{

    namespace Win32
    {
        //______________________________________________________________________
        //
        //
        //
        //! Win32 semaphore for legacy condition variables on Windows
        //
        //
        //______________________________________________________________________
        class Semaphore
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const long lMinCount = 0;       //!< alias
            static const long lMaxCount = 65535;   //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Semaphore();            //!< setup
            ~Semaphore() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void wait() noexcept; //!< wait
            void post() noexcept; //!< post


        private:
#if defined(Y_WIN)
            HANDLE sem_;
#endif
            Y_DISABLE_COPY_AND_ASSIGN(Semaphore);
        };
    }

}

#endif
