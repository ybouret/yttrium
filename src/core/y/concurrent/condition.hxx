
namespace Yttrium
{
    namespace Concurrent
    {

        namespace Nucleus
        {

#if defined(Y_BSD)

            //__________________________________________________________________
            //
            //
            //
            //! pthread_condition wrapper
            //
            //
            //__________________________________________________________________
            class Condition
            {
            public:
                inline Condition() : cond()
                {
                    Y_STATIC_ZVAR(cond);
                    const int res = pthread_cond_init(&cond,NULL);
                    if( res != 0 ) throw Libc::Exception(res,"pthread_cond_init");
                }

                inline ~Condition() noexcept
                {
                    const int res = pthread_cond_destroy(&cond);
                    if( res != 0 )  Libc::CriticalError(res,"pthread_cond_destroy");
                }

                inline void wait(Mutex *m) noexcept
                {
                    assert(m);
                    const int res = pthread_cond_wait(&cond,**m);
                    if( res != 0 ) Libc::CriticalError(res, "pthread_cond_wait" );
                }

                inline void signal() noexcept
                {
                    const int res = pthread_cond_signal(&cond);
                    if( res != 0 ) Libc::CriticalError(res, "pthread_cond_signal");
                }

                inline void broadcast() noexcept
                {
                    const int res = pthread_cond_broadcast(&cond);
                    if( res != 0 ) Libc::CriticalError(res, "pthread_cond_broadcast");
                }


                pthread_cond_t cond;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Condition);
            };
#endif

        }

    }

}
