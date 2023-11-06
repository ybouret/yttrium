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

                explicit Thread(ThreadProc proc, void *args) : thr(0)
#if defined(Y_WIN)
                , tid(0)
#endif
                {
                    assert(0!=proc);

                }


                inline ~Thread() noexcept 
                {

                }

#if defined(Y_BSD)
                pthread_t thr;
#endif

#if defined(Y_WIN)
                HANDLE    thr;
                DWORD     tid;
#endif

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Thread);
            };
        }

    }

}
