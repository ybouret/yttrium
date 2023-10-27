#include "y/concurrent/win32-semaphore.hpp"
#include "y/system/exception.hpp"
#include "y/system/error.hpp"

namespace Yttrium  {

    namespace Win32
    {
        Semaphore:: ~Semaphore() noexcept
        {
#if defined(Y_WIN)
            assert(0!=sem_); ::CloseHandle(sem_); sem_ = NULL;
#endif
        }

        Semaphore:: Semaphore()
#if defined(Y_WIN)
        : sem_( :::CreateSemaphore(NULL,lMinCount,lMaxCount,NULL) )
        {
            if( !sem_ ) throw Win32::Exception( ::GetLastError(), "::CreateSemaphore" );
        }
#else
        {
            
        }
#endif


        inline void wait() noexcept
        {
#if defined(Y_WIN)
            assert(0!=sem_)
            const DWORD res = ::WaitForSingleObject(sem_, INFINITE);
            if (res != WAIT_OBJECT_0) CriticalError(::GetLastError(), "::WaitForSingleObject( SEMAPHORE )");
#endif
        }

        inline void post() noexcept
        {
#if defined(Y_WIN)
            assert(0!=sem_);
            if (!::ReleaseSemaphore(sem_, 1, NULL)) CriticalError(::GetLastError(), "::ReleaseSemaphore");
#endif
        }

    }
}

