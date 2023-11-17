#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/thread.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

namespace   {

    class Demo
    {
    public:
        explicit Demo() noexcept {}
        virtual ~Demo() noexcept {}

        inline void run(const Concurrent::ThreadContext &ctx)
        {
            {
                Y_LOCK(ctx.sync);
                std::cerr << "In " << ctx.name << std::endl;
            }
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo);
    };

}

Y_UTEST(concurrent_loop)
{
    Concurrent::Thread::Verbose = true;
    Demo                       demo;
    Concurrent::Kernel         kernel(&demo, & Demo::run);
    Concurrent::Mono           mono("mono");
    const Concurrent::Topology topo;
    Concurrent::Crew           crew(topo);
    
    Y_THREAD_MSG("now in main...");

    mono(kernel);
    crew(kernel);


}
Y_UDONE()

