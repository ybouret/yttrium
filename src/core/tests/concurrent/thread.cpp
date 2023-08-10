



#include "y/concurrent/mutex.hpp"
#include "y/utest/run.hpp"
#include "y/system/exception.hpp"
#include "y/system/error.hpp"
#include <cerrno>

#if defined(Y_BSD)
#include <pthread.h>

using namespace Yttrium;

class Thread : public Concurrent::NucleusMutex
{
public:
    typedef void (*Proc)(void *);




    Thread(Proc userProc, void *userArgs) :
    Concurrent::NucleusMutex(),
    handle(),
    proc(userProc),
    args(userArgs)
    {
        assert(0!=proc);
        Y_LOCK(*this);
        (std::cerr << "creating thread..." << std::endl).flush();
        const int err = pthread_create(&handle,NULL,launch,this);
        if(0!=err) throw Libc::Exception(errno,"pthread_create");
    }

    virtual ~Thread() noexcept
    {
        {
            Y_LOCK(*this);
            (std::cerr << "waiting for thread..." << std::endl).flush();
        }
        void     *ret = 0;
        const int err = pthread_join(handle,&ret);
        if(0!=err) Libc::CriticalError(err, "pthread_join");
    }

    pthread_t handle;


    static void * launch(void *param) noexcept
    {
        assert(0!=param);
        static_cast<Thread *>(param)->call();
        return 0;
    }

    Proc               proc;
    void              *args;

    void call() noexcept
    {
        try
        {

            assert(0!=proc);
            proc(args);
        }
        catch(...)
        {
        }
    }
};

#endif


using namespace Yttrium;



static inline void myProc(void *args)
{
    assert(0!=args);
    Lockable &access = *(Lockable *)args;

    Y_LOCK(access);
    std::cerr << "in thread..." << std::endl;
}


Y_UTEST(concurrent_thread)
{

    Lockable &access = Lockable::Giant();

#if defined(Y_BSD)
    Thread thr(myProc,&access);
#endif

}
Y_UDONE()

