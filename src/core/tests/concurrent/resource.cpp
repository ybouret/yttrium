#include "y/concurrent/resource/in0d.hpp"
#include "y/concurrent/resource/in1d.hpp"
#include "y/concurrent/resource/in2d.hpp"

#include "y/concurrent/pipeline/alone.hpp"
#include "y/concurrent/loop/mono.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;

namespace
{


    class Res0 : public Concurrent::Resource0D
    {
    public:
        explicit Res0(const Concurrent::ThreadContext &ctx) : Concurrent::Resource0D(ctx) {}
        virtual ~Res0() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Res0);
    };

    class Res1 : public Concurrent::Resource1D<int>
    {
    public:
        explicit Res1(const Concurrent::ThreadContext &ctx) : Concurrent::Resource1D<int>(ctx) {}
        virtual ~Res1() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Res1);
    };

    class Res2 : public Concurrent::Resource2D<int>
    {
    public:
        explicit Res2(const Concurrent::ThreadContext &ctx) : Concurrent::Resource2D<int>(ctx) {}
        virtual ~Res2() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Res2);
    };



}

Y_UTEST(concurrent_resource)
{
    Concurrent::Mono  mono;
    Concurrent::Alone alone;

    Res0 r0( alone[1] );
    Res1 r1( mono[1]  );
    Res2 r2( mono[1]  );

    std::cerr << "r0: " << r0 << std::endl;
    std::cerr << "r1: " << r1 << std::endl;
    std::cerr << "r2: " << r2 << std::endl;

}
Y_UDONE()

