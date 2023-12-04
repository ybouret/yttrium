
#include "y/concurrent/engine/in1d.hpp"
#include "y/utest/run.hpp"
#include "y/concurrent/loop/mono.hpp"

using namespace Yttrium;


namespace
{
    template <typename T>
    class MyEngine1D : public Concurrent::Engine1D<T>
    {
    public:
        inline explicit MyEngine1D() noexcept : Concurrent::Engine1D<T>() {}
        inline virtual ~MyEngine1D() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(MyEngine1D);
        virtual void activate(const Concurrent::ThreadContext &)
        {

        }
    };
}

Y_UTEST(concurrent_engine1d)
{
    MyEngine1D<int> engine;

    Concurrent::Mono mono;
    engine.start(mono[1],1,10,1);

    std::cerr << engine << std::endl;

}
Y_UDONE()


