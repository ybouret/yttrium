
#include "y/concurrent/pipeline/alone.hpp"
#include "y/concurrent/pipeline/queue.hpp"
#include "y/concurrent/frame/punctual.hpp"
#include "y/concurrent/frames.hpp"
#include "y/concurrent/pipeline/task.hpp"
#include "y/random/bits.hpp"
#include "y/system/wtime.hpp"
#include "y/utest/run.hpp"
#include "y/type/binder.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        class MultiplexQ
        {
        public:
            virtual ~MultiplexQ() noexcept { Coerce(Q) = 0; }

            void flush() noexcept { assert(0!=Q); Q->flush(); }
            TaskUUID operator()(Runnable *runnable)
            {
                assert(0!=Q); return Q->run(runnable);
            }

        protected:
            explicit MultiplexQ(const SharedPipeline &sp) noexcept :
            Q( & Coerce(*sp) )
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(MultiplexQ);
            Pipeline * const Q;
        };

        template <typename ENGINE>
        class Multiplex : public Frames<ENGINE>, public MultiplexQ
        {
        public:
            typedef Writable<ENGINE> Engines;

            explicit Multiplex(const SharedPipeline &sp) :
            Frames<ENGINE>(sp),
            MultiplexQ(sp)
            {
            }

            inline virtual ~Multiplex() noexcept {}


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Multiplex);
        };

        template <typename ENGINE, typename TLIST>
        struct Invoke
        {
            typedef Writable<ENGINE> Engines;
            Y_BINDER_ECHO(TLIST);

            template <typename METHOD> static inline
            TaskUUID On(Multiplex<ENGINE> &plex,
                        METHOD             meth)
            {
                return plex( new Job<METHOD>(plex,meth) );
            }

            template <typename METHOD> static inline
            TaskUUID On(Multiplex<ENGINE> &plex,
                          METHOD             meth,
                          Param1             arg1)
            {
                return plex( new Job<METHOD>(plex,meth,arg1) );
            }

            template <typename METHOD> static inline
            TaskUUID On(Multiplex<ENGINE> &plex,
                        METHOD             meth,
                        Param1             arg1,
                        Param2             arg2)
            {
                return plex( new Job<METHOD>(plex,meth,arg1,arg2) );
            }

            template <typename METHOD> static inline
            TaskUUID On(Multiplex<ENGINE> &plex,
                        METHOD             meth,
                        Param1             arg1,
                        Param2             arg2,
                        Param3             arg3)
            {
                return plex( new Job<METHOD>(plex,meth,arg1,arg2,arg3) );
            }


            template <typename METHOD> static inline
            TaskUUID On(Multiplex<ENGINE> &plex,
                        METHOD             meth,
                        Param1             arg1,
                        Param2             arg2,
                        Param3             arg3,
                        Param4             arg4)
            {
                return plex( new Job<METHOD>(plex,meth,arg1,arg2,arg3,arg4) );
            }


            template <typename METHOD> static inline
            TaskUUID On(Multiplex<ENGINE> &plex,
                        METHOD             meth,
                        Param1             arg1,
                        Param2             arg2,
                        Param3             arg3,
                        Param4             arg4,
                        Param5             arg5)
            {
                return plex( new Job<METHOD>(plex,meth,arg1,arg2,arg3,arg4,arg5) );
            }


            template <typename METHOD>
            class Job : public Runnable, public Binder<TLIST>
            {
            public:
                Y_BINDER_ARGS(TLIST);
                inline virtual ~Job() noexcept {}

                inline explicit Job(Engines &eng,
                                    METHOD   mth) noexcept :
                Runnable(), Binder<TLIST>(),
                plex(eng),
                meth(mth)
                {
                }

                inline explicit Job(Engines &eng,
                                    METHOD   mth,
                                    Param1   p1) noexcept :
                Runnable(), Binder<TLIST>(p1),
                plex(eng),
                meth(mth)
                {
                }

                inline explicit Job(Engines &eng,
                                    METHOD   mth,
                                    Param1   p1,
                                    Param2   p2) noexcept :
                Runnable(), Binder<TLIST>(p1,p2),
                plex(eng),
                meth(mth)
                {
                }

                inline explicit Job(Engines &eng,
                                    METHOD   mth,
                                    Param1   p1,
                                    Param2   p2,
                                    Param3   p3) noexcept :
                Runnable(), Binder<TLIST>(p1,p2,p3),
                plex(eng),
                meth(mth)
                {
                }

                inline explicit Job(Engines &eng,
                                    METHOD   mth,
                                    Param1   p1,
                                    Param2   p2,
                                    Param3   p3,
                                    Param4   p4) noexcept :
                Runnable(), Binder<TLIST>(p1,p2,p3,p4),
                plex(eng),
                meth(mth)
                {
                }

                inline explicit Job(Engines &eng,
                                    METHOD   mth,
                                    Param1   p1,
                                    Param2   p2,
                                    Param3   p3,
                                    Param4   p4,
                                    Param5   p5) noexcept :
                Runnable(), Binder<TLIST>(p1,p2,p3,p4,p5),
                plex(eng),
                meth(mth)
                {
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Job);
                Engines &plex;
                METHOD   meth;

                //! interface
                virtual void run(const ThreadContext &ctx)
                {
                    static const typename Binder<TLIST>::ArgsType params =  {};
                    perform(plex[ctx.indx],params);
                }

                inline void perform(ENGINE &host, const Int2Type<0> &) { (host.*meth)(); }
                inline void perform(ENGINE &host, const Int2Type<1> &) { (host.*meth)(arg1); }
                inline void perform(ENGINE &host, const Int2Type<2> &) { (host.*meth)(arg1,arg2); }
                inline void perform(ENGINE &host, const Int2Type<3> &) { (host.*meth)(arg1,arg2,arg3); }
                inline void perform(ENGINE &host, const Int2Type<4> &) { (host.*meth)(arg1,arg2,arg3,arg4); }
                inline void perform(ENGINE &host, const Int2Type<5> &) { (host.*meth)(arg1,arg2,arg3,arg3,arg5); }

            };
        };


    }

}

using namespace Yttrium;

namespace
{


    class Demo : public Concurrent::PunctualFrame
    {
    public:
        inline explicit Demo(const Concurrent::ThreadContext &ctx) noexcept :
        Concurrent::PunctualFrame(ctx), ran(), tmx()
        {}

        void call0()
        {
            {
                Y_LOCK(sync);
                std::cerr << "   (*) demo[" << *this << "]" << std::endl;
            }
            tmx.wait( ran.to<double>() );
        }

        void operator()(void)
        {
            call0();
        }

        void call1(double x)
        {
            {
                Y_LOCK(sync);
                std::cerr << "   (*) demo[" << *this << "].double=" << x << std::endl;
            }
            tmx.wait( ran.to<double>() );
        }

        void call2(double &x, const int a)
        {
            {
                Y_LOCK(sync);
                std::cerr << "   (*) demo[" << *this << "].double=" << x << " .int=" << a << std::endl;
            }
            x += a;
            tmx.wait( ran.to<double>() );
        }


        inline virtual ~Demo() noexcept {}

        Random::Rand ran;
        WallTime     tmx;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo);
    };


}

Y_UTEST(concurrent_frame0d)
{

    const Concurrent::Topology topo;
    Concurrent::SharedPipeline seqEngine = new Concurrent::Alone();
    Concurrent::SharedPipeline parEngine = new Concurrent::Queue(topo);

    Concurrent::Multiplex<Demo> seq(seqEngine);
    Concurrent::Multiplex<Demo> par(parEngine);

    std::cerr << "Empty" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;

    seq.attach();
    par.attach();
    std::cerr << "Attached" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;


    typedef Concurrent::Invoke<Demo,NullType> Invoke0;

    std::cerr << "Sequential/0" << std::endl;
    for(int i=1;i<=2;++i)
    {
        Invoke0::On(seq, & Demo::call0);
    }
    seq.flush();
    std::cerr << std::endl;

    std::cerr << "Parallel/0" << std::endl;
    for(int i=1;i<=4;++i)
    {
        Invoke0::On(par, & Demo::call0);
    }
    par.flush();
    std::cerr << std::endl;

    typedef Concurrent::Invoke<Demo,TL1(double)> Invoke1;

    std::cerr << "Sequential/1" << std::endl;
    for(int i=1;i<=2;++i)
    {
        Invoke1::On(seq, & Demo::call1, i);
    }
    seq.flush();
    std::cerr << std::endl;

    std::cerr << "Parallel/1" << std::endl;
    for(int i=1;i<=4;++i)
    {
        Invoke1::On(par, & Demo::call1, i);
    }
    par.flush();
    std::cerr << std::endl;


    typedef Concurrent::Invoke<Demo,TL2(double &, const int)> Invoke2;

    {
        double       arr[] = { 3, 2, 1, 0 };
        const size_t num = sizeof(arr)/sizeof(arr[0]);

        Core::Display(std::cerr, arr, num) << std::endl;
        for(size_t i=0;i<sizeof(arr)/sizeof(arr[0]);++i)
        {
            Invoke2::On(seq, & Demo::call2, arr[i], int(i) );
        }
        seq.flush();
        Core::Display(std::cerr, arr, num) << std::endl;

        for(size_t i=0;i<sizeof(arr)/sizeof(arr[0]);++i)
        {
            Invoke2::On(par, & Demo::call2, arr[i], int(i) );
        }
        par.flush();
        Core::Display(std::cerr, arr, num) << std::endl;

    }


#if 0
    seq.exec();
    seq.exec();
    seq.flush();

    par.exec();
    par.exec();
    par.flush();


    Worker worker;
    {
        const Duty duty(worker,seq);
        seqEngine->load(duty);
        seqEngine->flush();
    }
#endif


    seq.detach();
    par.detach();
    std::cerr << "Detached" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;


}
Y_UDONE()

