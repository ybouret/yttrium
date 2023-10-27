
namespace Yttrium
{
    namespace Concurrent
    {

        namespace Nucleus
        {

            //------------------------------------------------------------------
            //
            //
            // <Mutex>
            //
            //
            //------------------------------------------------------------------
#if defined(Y_BSD)
            class MutexAttribute
            {
            public:
                inline explicit MutexAttribute() : attr()
                {
                    {
                        const int  res = pthread_mutexattr_init(&attr);
                        if (res != 0) throw Libc::Exception(res, "pthread_mutexattr_init");
                    }

                    {
                        const int res = pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
                        if (res != 0)
                        {
                            pthread_mutexattr_destroy(&attr);
                            throw Libc::Exception(res, "pthread_mutexattr_settype(RECURSIVE)");
                        }
                    }
                }

                inline virtual ~MutexAttribute() noexcept
                {
                    (void) pthread_mutexattr_destroy(&attr);
                }

                pthread_mutexattr_t attr;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(MutexAttribute);
            };
#endif

#if         defined(Y_WIN)
            struct MutexAttribute {};
#endif

            class Mutex
            {
            public:
                inline Mutex(MutexAttribute &param) : m()
                {

#if                 defined(Y_BSD)
                    const int res = pthread_mutex_init(&m,&param.attr);
                    if( res != 0 ) throw Libc::Exception(res,"pthread_mutex_init");

#endif

#if                 defined(Y_WIN)
                    (void)param;
                    ::InitializeCriticalSection(&m);
#endif

                }

                inline ~Mutex() noexcept
                {
#if                 defined(Y_BSD)
                    const int res = pthread_mutex_destroy(&m);
                    if(res!=0) Libc::CriticalError(res,"pthread_mutex_destroy");

#endif

#if                 defined(Y_WIN)
                    ::DeleteCriticalSection(&m);
#endif
                }

                inline void lock() noexcept
                {
#if defined(Y_BSD)
                    const int res = pthread_mutex_lock(&m);
                    if( res != 0 ) Libc::CriticalError(res,"pthread_mutex_lock");
#endif

#if defined(Y_WIN)
                    ::EnterCriticalSection(&m);
#endif
                }

                inline void unlock() noexcept
                {
#if defined(Y_BSD)
                    const int res = pthread_mutex_unlock(&m);
                    if( res != 0 ) Libc::CriticalError(res,"pthread_mutex_unlock");
#endif

#if defined(Y_WIN)
                    ::LeaveCriticalSection(&m);
#endif
                }

                inline bool tryLock() noexcept
                {
#if defined(Y_WIN)
                    return ::TryEnterCriticalSection(&m) == TRUE;
#endif

#if defined(Y_BSD)
                    return pthread_mutex_trylock(&m) == 0;
#endif
                }

                //! syntax helper for conditions
                inline MutexType * operator*() noexcept { return &m; }

                MutexType m;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Mutex);
            };

            //------------------------------------------------------------------
            //
            //
            // <Mutex/>
            //
            //
            //------------------------------------------------------------------

        }
    }
}
