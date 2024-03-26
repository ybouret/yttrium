
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
        template <typename ENGINE>
        class Router : public Frames<ENGINE>
        {
        public:
            typedef Writable<ENGINE> Engines;

            explicit Router(const SharedPipeline &sp) :
            Frames<ENGINE>(sp),
            queue( Coerce(*sp) )
            {
            }

            inline virtual ~Router() noexcept
            {
            }


            template <typename METHOD, typename TLIST>
            class Job : public Runnable, public Binder<TLIST>
            {
            public:
                Y_BINDER_ECHO(TLIST);
                typedef Binder<TLIST> BinderType;
                
                inline Job(Engines &myEngines,
                           METHOD   userMethod) noexcept :
                Runnable(),
                BinderType(),
                engines(myEngines),
                method(userMethod)
                {

                }

                inline virtual ~Job() noexcept {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Job);
                Engines &engines;
                METHOD   method;

                virtual void run(const ThreadContext &ctx)
                {
                    static const typename BinderType::ArgsType choice =  {};
                    call( engines[ctx.indx], choice);
                }

                inline void call(ENGINE &host, const Int2Type<0> &)
                {
                    (host.*method)();
                }

            };

            class Duty : public Runnable
            {
            public:
                explicit Duty(Engines &eng) noexcept : engines(eng) {}
                virtual ~Duty() noexcept {}

                virtual void run(const ThreadContext &ctx)
                {
                    ENGINE &engine = engines[ ctx.indx ]; // find engine
                    engine();
                }

            private:
                Engines &engines;
                Y_DISABLE_COPY_AND_ASSIGN(Duty);
            };


            void flush() noexcept
            {
                queue.flush();
            }

            template <typename METHOD>
            TaskUUID call(METHOD method)
            {
                return queue.run(new Job<METHOD,NullType>(*this,method) );
            }


            TaskUUID exec()
            {
                return queue.run( new Duty(*this) );
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Router);
            Pipeline &queue;
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

    Concurrent::Router<Demo> seq(seqEngine);
    Concurrent::Router<Demo> par(parEngine);

    std::cerr << "Empty" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;

    seq.attach();
    par.attach();
    std::cerr << "Attached" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;


    seq.call( & Demo::call0 );
    seq.flush();
    
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

