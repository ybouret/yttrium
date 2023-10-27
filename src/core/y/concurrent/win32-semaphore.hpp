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

        class Semaphore
        {
        public:
            static const long lMinCount = 0;
            static const long lMaxCount = 65535;

            Semaphore();
            ~Semaphore() noexcept;

            void wait() noexcept;
            void post() noexcept;


        private:
#if defined(Y_WIN)
            HANDLE sem_;
#endif
            Y_DISABLE_COPY_AND_ASSIGN(Semaphore);
        };
    }

}

#endif
