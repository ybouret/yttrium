#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/frames.hpp"
#include "y/utest/run.hpp"

#include "y/type/binder.hpp"

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

        template <typename ENGINE,typename TLIST>
        struct Execute
        {
            typedef Writable<ENGINE> Engines; //!< alias


            //__________________________________________________________________
            //
            //! ENGINE.meth()
            //__________________________________________________________________
            template <typename METHOD> static inline
            void On(SIMT<ENGINE> &simt,
                    METHOD        meth)
            {
                class Call : public Kernel  {
                public:
                    explicit Call(Engines &eng,
                                  METHOD  &mth) noexcept :
                    engines(eng),
                    method(mth)
                    {
                    }

                    virtual ~Call() noexcept {}

                    virtual void operator()(const ThreadContext &ctx)
                    {
                        ENGINE &host = engines[ctx.indx];
                        (host.*method)();
                    }

                private:
                    Engines &engines;
                    METHOD   method;
                    Y_DISABLE_COPY_AND_ASSIGN(Call);
                };

                Call me(simt,meth);
                simt->run1(me);
            }

        };

    }
}

using namespace Yttrium;

#include "y/concurrent/frame/1d.hpp"
#include "y/system/wtime.hpp"
#include "y/random/bits.hpp"

namespace
{
    class Demo1D : public Concurrent::Frame1D<size_t>
    {
    public:
        explicit Demo1D(const Concurrent::ThreadContext &ctx) noexcept :
        Concurrent::Frame1D<size_t>(ctx),
        tmx(),
        ran()
        {
            std::cerr << "  (*) Demo1D@" << name << std::endl;
        }

        void call0()
        {
            assert(this->isAssigned());
            {
                Y_LOCK(sync);
                std::cerr << "  (*) Demo1D.call0(" << *this << ")" << std::endl;
            }
            tmx.wait( ran.to<double>() );
        }

        virtual ~Demo1D() noexcept {}

        WallTime     tmx;
        Random::Rand ran;

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

    typedef Concurrent::Execute<Demo1D,NullType> Execute0;

    seq.assign(1,10,2);
    par.assign(1,10,2);


    Execute0::On(seq, & Demo1D::call0 );
    Execute0::On(par, & Demo1D::call0 );



}
Y_UDONE()

