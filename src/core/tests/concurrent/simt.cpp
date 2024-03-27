#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/frames.hpp"
#include "y/utest/run.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        class SIMT_Loop
        {
        public:

            Loop *       operator->()       noexcept { return loop; }
            const Loop * operator->() const noexcept { return loop; }


            virtual ~SIMT_Loop() noexcept { Coerce(loop) = 0; }
        protected:
            explicit SIMT_Loop(const SharedLoop &sl) noexcept :
            loop( & Coerce(*sl) )
            {
            }
        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMT_Loop);
            Loop * const loop;
        };

        template <typename ENGINE>
        class SIMT : public Frames<ENGINE>, public SIMT_Loop
        {
        public:
            inline explicit  SIMT( const SharedLoop &sl ) :
            Frames<ENGINE>(sl),
            SIMT_Loop(sl)
            {
            }

            inline virtual ~SIMT() noexcept {}


        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMT);
        };
    }
}

using namespace Yttrium;

#include "y/concurrent/frame/1d.hpp"

namespace
{
    class Demo1D : public Concurrent::Frame1D<size_t>
    {
    public:
        explicit Demo1D(const Concurrent::ThreadContext &ctx) noexcept :
        Concurrent::Frame1D<size_t>(ctx)
        {
            std::cerr << "  (*) Demo1D@" << name << std::endl;
        }

        virtual ~Demo1D() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo1D);
    };
}

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"

Y_UTEST(concurrent_simt)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");

    const Concurrent::Topology topo;
    Concurrent::SharedLoop     seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop     parLoop = new Concurrent::Crew(topo);

    Concurrent::SIMT<Demo1D> seq(seqLoop);
    Concurrent::SIMT<Demo1D> par(parLoop);

    

}
Y_UDONE()

