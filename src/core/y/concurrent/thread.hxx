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
                    assert(0!=proc);

#if defined(Y_BSD)
                    const int err = pthread_create(&thr,NULL,launch,this);
                    if(0!=err) throw Libc::Exception(err,"pthread_create");
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
                    const int err = pthread_join(thr,&ret);
                    if(0!=err) Libc::CriticalError(err, "pthread_join");
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
                        assert(0!=myProc);
                        myProc(myArgs);
                    }
                    catch(...)
                    {

                    }

                }

#if defined(Y_BSD)
                static void * launch(void *param) noexcept
                {
                    assert(0!=param);
                    static_cast<Thread *>(param)->call();
                    return 0;
                }
#endif

            };
        }

    }

}
