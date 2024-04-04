
#include "y/concurrent/loop/simt/execute.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/utest/run.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"

#include "y/concurrent/frame/1d.hpp"
#include "y/concurrent/frame/2d.hpp"
#include "y/system/wtime.hpp"
#include "y/random/bits.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        template <typename T>
        class X1D : public Frame1D<T>
        {
        public:
            using Frame1D<T>::loop;
            using typename Frame1D<T>::LoopPtr;

            typedef void (*Proto)(const ThreadContext &,
                                  const ForLoop<T>   &);

            inline explicit X1D(const ThreadContext &ctx) noexcept : Frame1D<T>(ctx)
            {
            }

            inline virtual ~X1D() noexcept
            {
            }

            void run(Proto proto)
            {
                assert(0!=proto);
                assert(this->isAssigned());
                {
                    Y_LOCK(this->sync);
                    (std::cerr << "run @" << LoopPtr(loop) << std::endl).flush();
                }
                if(loop)
                {
                    proto(*this,*loop);
                }
            }


        private:
            Y_DISABLE_ASSIGN(X1D);
        };

    }
}

using namespace Yttrium;


namespace
{
    void DoSomething(const Concurrent::ThreadContext   &ctx,
                     const Concurrent::ForLoop<size_t> &loop)
    {
        {
            Y_LOCK(ctx.sync);
            (std::cerr << "\tDoSomething@" << loop <<  "/" << ctx.name << std::endl).flush();
        }
    }
}




Y_UTEST(concurrent_simtx)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");

    const Concurrent::Topology topo;
    Concurrent::SharedLoop     seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop     parLoop = new Concurrent::Crew(topo);

    typedef Concurrent::X1D<size_t> Engine1D;

    Concurrent::SIMT<Engine1D> seq( seqLoop );
    Concurrent::SIMT<Engine1D> par( parLoop );

    typedef Concurrent::Execute<Engine1D,TL1(Engine1D::Proto)> Execute1D;
    std::cerr << "running..." << std::endl;

    seq.assign(1,10,2);
    par.assign(1,10,2);
    std::cerr << "-------- Seq --------" << std::endl;
    Execute1D::On(seq,&Engine1D::run,DoSomething);
    std::cerr << "-------- Par --------" << std::endl;
    Execute1D::On(par,&Engine1D::run,DoSomething);




}
Y_UDONE()
