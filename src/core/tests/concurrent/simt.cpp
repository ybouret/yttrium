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

#define Y_CONCURRENT_EXEC_DECL(I)                                       \
typedef typename TL::SafeTypeAt<TLIST,I-1,EmptyType>::Result Type##I;   \
typedef typename TypeTraits<Type##I>::ParamType              Param##I;  \
typedef typename TypeTraits<Param##I>::ReferenceType         Ref##I

        template <typename ENGINE,typename TLIST>
        struct Execute
        {
            typedef Writable<ENGINE> Engines; //!< alias
            Y_CONCURRENT_EXEC_DECL(1);
            Y_CONCURRENT_EXEC_DECL(2);
            Y_CONCURRENT_EXEC_DECL(3);
            Y_CONCURRENT_EXEC_DECL(4);
            Y_CONCURRENT_EXEC_DECL(5);

            template <typename METHOD>
            class ModKernel : public Kernel
            {
            public:
                inline virtual ~ModKernel() noexcept {}

            protected:
                inline explicit ModKernel(Engines &eng, METHOD &mth) noexcept :
                method(mth),
                engines(eng)
                {
                }

                inline ENGINE & host(const Context &ctx) noexcept {
                    return engines[ctx.indx];
                }

                METHOD   method;
            private:
                Engines &engines;
                Y_DISABLE_COPY_AND_ASSIGN(ModKernel);
            };



            //__________________________________________________________________
            //
            //! ENGINE.meth()
            //__________________________________________________________________
            template <typename METHOD> static inline
            void On(SIMT<ENGINE> &simt,
                    METHOD        meth)
            {
                class Call : public ModKernel<METHOD>  {
                public:
                    inline explicit Call(Engines &eng, METHOD  &mth) noexcept :
                    ModKernel<METHOD> (eng,mth) {}

                    inline virtual ~Call() noexcept {}

                    inline virtual void operator()(const ThreadContext &ctx)
                    {
                        (this->host(ctx).*(this->method))();
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Call);
                };

                Call me(simt,meth);
                simt->run1(me);
            }

            //__________________________________________________________________
            //
            //! ENGINE.meth(arg1)
            //__________________________________________________________________
            template <typename METHOD> static inline
            void On(SIMT<ENGINE> &simt,
                    METHOD        meth,
                    Param1        p1)
            {
                class Call : public Kernel  {
                public:
                    inline explicit Call(Engines &eng, METHOD  &mth, Ref1 p1) noexcept :
                    engines(eng), method(mth), arg1(p1) {}

                    inline virtual ~Call() noexcept {}

                    inline virtual void operator()(const ThreadContext &ctx)
                    {
                        ENGINE &host = engines[ctx.indx];
                        (host.*method)(arg1);
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Call);
                    Engines &engines;
                    METHOD   method;
                    Ref1     arg1;
                };

                Call me(simt,meth,p1);
                simt->run1(me);
            }

            //__________________________________________________________________
            //
            //! ENGINE.meth(arg1,arg2)
            //__________________________________________________________________
            template <typename METHOD> static inline
            void On(SIMT<ENGINE> &simt,
                    METHOD        meth,
                    Param1        p1,
                    Param2        p2)
            {
                class Call : public Kernel  {
                public:
                    inline explicit Call(Engines &eng, METHOD  &mth, Ref1 p1, Ref2 p2) noexcept :
                    engines(eng), method(mth), arg1(p1), arg2(p2) {}

                    inline virtual ~Call() noexcept {}

                    inline virtual void operator()(const ThreadContext &ctx)
                    {
                        ENGINE &host = engines[ctx.indx];
                        (host.*method)(arg1);
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Call);
                    Engines &engines;
                    METHOD   method;
                    Ref1     arg1;
                    Ref2     arg2;
                };

                Call me(simt,meth,p1,p2);
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

        void call1(const int a)
        {
            assert(this->isAssigned());
            {
                Y_LOCK(sync);
                std::cerr << "  (*) Demo1D.call1(" << a << "," << *this << ")" << std::endl;
            }
            tmx.wait( ran.to<double>() );
        }

        void call1bis(double &sum)
        {
            assert(this->isAssigned());
            {
                Y_LOCK(sync);
                std::cerr << "  (*) Demo1D.call1bis(" << sum << "," << *this << ")" << std::endl;
                sum += indx;
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


    std::cerr << "Assigning..." << std::endl;
    seq.assign(1,10,2);
    par.assign(1,10,2);

    {
        std::cerr << "Execute/0" << std::endl;
        typedef Concurrent::Execute<Demo1D,NullType> Execute0;
        std::cerr << "...seq:" << std::endl;
        Execute0::On(seq, & Demo1D::call0 );
        std::cerr << "...par:" << std::endl;
        Execute0::On(par, & Demo1D::call0 );
    }

    {
        std::cerr << "Execute/1" << std::endl;
        typedef Concurrent::Execute<Demo1D,TL1(int)> Execute1;
        std::cerr << "...seq:" << std::endl;
        Execute1::On(seq, & Demo1D::call1, 12 );
        std::cerr << "...par:" << std::endl;
        Execute1::On(par, & Demo1D::call1, 12 );
    }

    {
        std::cerr << "Execute/1bis" << std::endl;
        typedef Concurrent::Execute<Demo1D,TL1(double &)> Execute1;
        std::cerr << "...seq:" << std::endl;
        double sum = 0;
        Execute1::On(seq, & Demo1D::call1bis, sum );
        std::cerr << "sum=" << sum << std::endl;
        sum = 0;
        std::cerr << "...par:" << std::endl;
        Execute1::On(par, & Demo1D::call1bis, sum );
        std::cerr << "sum=" << sum << std::endl;
    }


}
Y_UDONE()

