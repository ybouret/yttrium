#include "y/concurrent/thread/proc.hpp"
#include "y/concurrent/thread/handle.hpp"
#include "y/text/base64/encode.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        namespace Nucleus
        {

#if defined(Y_BSD)
#define Y_THREAD   pthread_t
#define Y_LAUNCH() void * launch(void *param)

#endif

#if defined(Y_WIN)
#define Y_THREAD   HANDLE
#define Y_LAUNCH() DWORD WINAPI launch(LPVOID param)
#endif

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
                explicit Thread(ThreadProc proc, void *args, ThreadHandle &uuid) :
                thr(0),
#if defined(Y_WIN)
                tid(0),
#endif
                myCore(Y_INVALID_THREAD_CORE),
                myProc(proc),
                myArgs(args)
                {
                    assert(0 != proc);
                    Y_GIANT_LOCK();

#if defined(Y_BSD)
                    const int err = pthread_create(&thr, NULL, launch, this);
                    if (0 != err) throw Libc::Exception(err, "pthread_create");
                    const ThreadHandle h(thr);
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
                    const ThreadHandle h(tid);
#endif
                    uuid = h;
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
                    if (0 != err)
                        Libc::CriticalError(err, "pthread_join");
#endif

#if defined(Y_WIN)
                    if (WAIT_OBJECT_0 != WaitForSingleObject(thr, INFINITE))
                        Win32::CriticalError(::GetLastError(), "WaitForSingleObject");
#endif

                }

                bool assign(const size_t j) noexcept;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Y_THREAD         thr;
#if defined(Y_WIN)
                DWORD            tid;
#endif
                const ThreadCore myCore;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Thread);
                ThreadProc  const myProc;
                void      * const myArgs;

                //______________________________________________________________
                //
                //
                //! forwarded call
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

                static inline Y_LAUNCH() noexcept
                {
                    assert(0 != param);
                    static_cast<Thread *>(param)->call();
                    return 0;
                }

            };


            static inline
            void DisplayThreadWarning(const Exception &excp)
            {
                if(true)
                {
                    Y_GIANT_LOCK();
                    std::cerr << "*** " << excp.when() << " : " << excp.what() << std::endl;
                }
            }

#if defined Y_THREAD_AFFINITY
#error "Y_THREAD_AFFINITY shouldn't be defined"
#endif

#if defined(Y_Darwin)
#define Y_THREAD_AFFINITY 1
#include <mach/thread_policy.h>
#include <mach/thread_act.h>

            bool Thread::assign(const size_t j) noexcept
            {
                thread_affinity_policy_data_t policy_data = { int(j) };
                mach_port_t                   mach_thread = pthread_mach_thread_np(thr);
                const int                     mach_result = thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY, (thread_policy_t)&policy_data, THREAD_AFFINITY_POLICY_COUNT);
                if (KERN_SUCCESS != mach_result)
                {
                    const Mach::Exception excp(mach_result, "thread_policy_set");
                    DisplayThreadWarning(excp);
                    return false;
                }
                else
                {
                    return true;
                }

            }

#endif

#if defined(Y_Linux)|| defined(Y_FreeBSD)
#   define Y_THREAD_AFFINITY 1
#   if defined(Y_FreeBSD)
#      include <pthread_np.h>
#      define Y_CPU_SET cpuset_t
#   else
#      define Y_CPU_SET cpu_set_t
#   endif

            bool   Thread::assign(const size_t j) noexcept
            {
                Y_CPU_SET the_cpu_set;
                CPU_ZERO(&the_cpu_set);
                CPU_SET(j, &the_cpu_set);
                const int err = pthread_setaffinity_np(thr, sizeof(Y_CPU_SET), &the_cpu_set);
                if (err != 0)
                {
                    const Libc::Exception excp(err, "pthread_setaffinity_np");
                    DisplayThreadWarning(excp);
                    return false;
                }
                else
                {
                    return true;
                }
            }

#endif

#if defined(Y_WIN)
#   define Y_THREAD_AFFINITY 1
            bool Thread::assign(const size_t j) noexcept
            {
                const DWORD_PTR mask = DWORD_PTR(1) << j;
                if (!::SetThreadAffinityMask(thr, mask))
                {
                    const DWORD err = ::GetLastError();
                    const Win32::Exception excp(err, "::SetThreadAffinityMask");
                    DisplayThreadWarning(excp);
                    return false;
                }
                else
                {
                    return true;
                }
            }
#endif

#if defined(Y_SunOS)
#   define Y_THREAD_AFFINITY 1
#include <sys/types.h>
#include <sys/processor.h>
#include <sys/procset.h>

            bool Thread::assign(const size_t j) noexcept
            {
                const int res = processor_bind(P_LWPID, idtype_t(thr), j, NULL);
                if (0 != res) 
                {
                    const Exception excp("processor_bind failure");
                    DisplayThreadWarning(excp);
                    return false;
                }
                else
                {
                    return true;
                }
            }

#endif


#if ! defined(Y_THREAD_AFFINITY)
#warning "No Thread Affinity"
            bool Thread::assign(const size_t) noexcept
            {
                const Exception excp("No Thread Affinity");
                DisplayThreadWarning(excp);
                return false;
            }
#endif


            
        }


    }

}
