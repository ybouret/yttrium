
#include "y/concurrent/pipeline/alone.hpp"
#include "y/concurrent/pipeline/queue.hpp"
#include "y/concurrent/frame/punctual.hpp"
#include "y/concurrent/frames.hpp"
#include "y/concurrent/pipeline/task.hpp"
#include "y/random/bits.hpp"
#include "y/system/wtime.hpp"
#include "y/utest/run.hpp"



namespace Yttrium
{
    namespace Concurrent
    {
        template <typename ENGINE>
        class Router : public Frames<ENGINE>
        {
        public:
            typedef Writable<ENGINE> Engines;

           // template <typename DERIVED>
            explicit Router(const SharedPipeline &sp) :
            Frames<ENGINE>(sp),
            queue( Coerce(*sp) )
            {
            }

            inline virtual ~Router() noexcept
            {
            }

            class Job : public Runnable
            {
            public:
                explicit Job(Engines &eng) noexcept : engines(eng) {}
                virtual ~Job() noexcept {}

                virtual void run(const ThreadContext &ctx)
                {
                    ENGINE &engine = engines[ ctx.indx ]; // find engine
                    engine();
                }

            private:
                Engines &engines;
                Y_DISABLE_COPY_AND_ASSIGN(Job);
            };

            class Duty : public Task
            {
            public:
                explicit Duty(Engines &eng) : Task( new Job(eng) ) {}
                virtual ~Duty() noexcept {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Duty);
            };

            void flush() noexcept
            {
                queue.flush();
            }

            TaskUUID exec()
            {
                const Duty task(*this);
                return queue.load(task);
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

        void operator()(void)
        {
            {
                Y_LOCK(sync);
                std::cerr << "   (*) demo[" << *this << "]" << std::endl;
            }
            tmx.wait( ran.to<double>() );
        }

        inline virtual ~Demo() noexcept {}

        Random::Rand ran;
        WallTime     tmx;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo);
    };

#if 0
    class Worker
    {
    public:
        explicit Worker() {}
        virtual ~Worker() noexcept {}

        void call(const Demo &dem)
        {
            {
                Y_LOCK(dem.sync);
                std::cerr << "worker[" << dem << "]" << std::endl;
            }
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Worker);
    };


    class Job : public Concurrent::Runnable
    {
    public:

        explicit Job(Worker         &worker,
                     Writable<Demo> &frames) :
        host( worker ),
        data( frames )
        {

        }

        virtual void run(const Concurrent::ThreadContext &ctx)
        {
            std::cerr << "job[" << ctx.name << "]" << std::endl;
            host.call( data[ ctx.indx ] );
        }

        virtual ~Job() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Job);
        Worker                   &host;
        Writable<Demo>           &data;
    };

    class Duty : public Concurrent::Task
    {
    public:
        explicit Duty(Worker         &worker,
                      Writable<Demo> &frames) :
        Task( new Job(worker,frames) )
        {
        }

        virtual ~Duty() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Duty);
    };
#endif

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

    seq.exec();
    seq.exec();
    seq.flush();

    par.exec();
    par.exec();
    par.flush();


#if 0
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

