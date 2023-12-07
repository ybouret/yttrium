

#include "y/concurrent/engine/in2d.hpp"
#include "y/utest/run.hpp"

#include "y/concurrent/loop/mono.hpp"

using namespace Yttrium;

namespace
{
    template <typename T>
    class MyEngine2D : public Concurrent::Engine2D<T>
    {
    public:
        inline explicit MyEngine2D() noexcept : Concurrent::Engine2D<T>() {}
        inline virtual ~MyEngine2D() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(MyEngine2D);
        virtual void activate(const Concurrent::ThreadContext &)
        {

        }

        virtual void shutdown() noexcept {}

    };
}

Y_UTEST(concurrent_engine2d)
{
    MyEngine2D<int> engine;
    typedef V2D<int> Vertex;

    Concurrent::Mono mono;
    engine.start(mono[1],Vertex(1,1), Vertex(10,20) );
   
    std::cerr << engine << std::endl;


}
Y_UDONE()


