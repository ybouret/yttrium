#include "y/mkl/tao/level1.hpp"
#include "y/mkl/tao/carver.hpp"

#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"
#include "y/sequence/vector.hpp"


#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
#if 0
    class TaoRes1D : public Concurrent::Resource1D<size_t>
    {
    public:
        explicit TaoRes1D() noexcept {}
        virtual ~TaoRes1D() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(TaoRes1D);

        virtual void attach(const Concurrent::ThreadContext &)
        {

        }
    };


    class TaoRes2D : public Concurrent::Resource2D<size_t>
    {
    public:
        explicit TaoRes2D() noexcept {}
        virtual ~TaoRes2D() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(TaoRes2D);

        virtual void attach(const Concurrent::ThreadContext &)
        {

        }
    };
#endif


#if 0
    template <typename TARGET, typename SOURCE>   inline
    void Load(TARGET &target, SOURCE &source, Concurrent::SIMD &simd)
    {
        assert(target.size()<=source.size());
        struct Work
        {
            void operator()(const Concurrent::Range &range, TARGET &target, SOURCE &source)
            {
                for(size_t i=range.offset,k=range.length;k>0;--k,++i)
                {
                    target[i] = source[i];
                }
            }
        };

        Work todo = {};
        simd(todo,target,source);


        // for(size_t i=target.size();i>0;--i)             target[i] = source[i];
    }
#endif

}

Y_UTEST(tao)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;


    Concurrent::SharedLoop seqLoop = new Concurrent::Mono();
    //Concurrent::SharedLoop parLoop = new Concurrent::Crew(topo);

    Tao::Carver seq(seqLoop);




}
Y_UDONE()


