#include "y/concurrent/thread-proc.hpp"


namespace Yttrium
{
	namespace Concurrent
	{
		namespace Nucleus
		{

			//__________________________________________________________________
			//
			//
			//! native thread wrapper
			//
			//__________________________________________________________________
			class Thread
			{

			public:

				//______________________________________________________________
				//
				//
				//! start thread
				//
				//______________________________________________________________
				explicit Thread(ThreadProc proc, void *args) : thr(0),
#if defined(Y_WIN)
					tid(0),
#endif
					myProc(proc),
					myArgs(args)

				{
					assert(0 != proc);
                    Y_GIANT_LOCK();

#if defined(Y_BSD)
					const int err = pthread_create(&thr, NULL, launch, this);
					if (0 != err) throw Libc::Exception(err, "pthread_create");
#endif

#if defined(Y_WIN)
					thr = CreateThread(
						NULL,        // default security attributes
						0,           // use default stack size  
						launch,      // thread function name
						this,        // argument to thread function 
						0,           // use default creation flags 
						&tid);       // returns the thread identifier 
					if (0 == thr) throw Win32::Exception(::GetLastError(), "::CreateThread");
#endif
				}

				//______________________________________________________________
				//
				//
				//! quit thread
				//
				//______________________________________________________________
				inline ~Thread() noexcept
				{
#if defined(Y_BSD)
					void     *ret = 0;
					const int err = pthread_join(thr, &ret);
					if (0 != err) Libc::CriticalError(err, "pthread_join");
#endif

#if defined(Y_WIN)
					if (WAIT_OBJECT_0 != WaitForSingleObject(thr, INFINITE))
						Win32::CriticalError(::GetLastError(), "WaitForSingleObject");

#endif

				}

				//______________________________________________________________
				//
				//
				// Members
				//
				//______________________________________________________________
#if defined(Y_BSD)
				pthread_t thr;
#endif

#if defined(Y_WIN)
				HANDLE    thr;
				DWORD     tid;
#endif

			private:
				Y_DISABLE_COPY_AND_ASSIGN(Thread);
				ThreadProc  const myProc;
				void      * const myArgs;

				//______________________________________________________________
				//
				//
				//! wrapper
				//
				//______________________________________________________________
				inline void call() noexcept
				{
					try
					{
						assert(0 != myProc);
						myProc(myArgs);
					}
					catch (...)
					{

					}

				}

#if defined(Y_BSD)
				static inline void * launch(void *param) noexcept
				{
					assert(0 != param);
					static_cast<Thread *>(param)->call();
					return 0;
				}
#endif

#if defined(Y_WIN)
				static inline DWORD WINAPI launch(LPVOID param) noexcept
				{
					assert(0 != param);
					static_cast<Thread *>(param)->call();
					return 0;
				}
#endif
			};
		}

	}

}
