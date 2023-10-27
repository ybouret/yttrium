#include "y/concurrent/win32-semaphore.hpp"
#include "y/system/exception.hpp"
#include "y/system/error.hpp"

namespace Yttrium {

	namespace Win32
	{
		Semaphore:: ~Semaphore() noexcept
		{
#if defined(Y_WIN)
			assert(0 != sem_);
			::CloseHandle(sem_); 
			sem_ = 0;
#endif
		}

		Semaphore::Semaphore()
#if defined(Y_WIN)
			: sem_(::CreateSemaphore(0, lMinCount, lMaxCount, 0))
		{
			if (!sem_) throw Win32::Exception(::GetLastError(), "::CreateSemaphore");
		}
#else
		{

		}
#endif


		void Semaphore::wait() noexcept
		{
#if defined(Y_WIN)
			assert(0 != sem_);
			const DWORD res = ::WaitForSingleObject(sem_, INFINITE);
			if (res != WAIT_OBJECT_0) CriticalError(::GetLastError(), "::WaitForSingleObject( SEMAPHORE )");
#endif
		}

		void Semaphore::post() noexcept
		{
#if defined(Y_WIN)
			assert(0 != sem_);
			if (!::ReleaseSemaphore(sem_, 1, NULL)) CriticalError(::GetLastError(), "::ReleaseSemaphore");
#endif
		}

	}
}

