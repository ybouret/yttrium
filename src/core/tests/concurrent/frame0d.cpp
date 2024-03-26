
#include "y/concurrent/pipeline/alone.hpp"
#include "y/concurrent/pipeline/queue.hpp"
#include "y/concurrent/frame/punctual.hpp"
#include "y/concurrent/frames.hpp"
#include "y/concurrent/pipeline/task.hpp"

#include "y/utest/run.hpp"



namespace Yttrium
{
    namespace Concurrent
    {
        template <typename ENGINE>
        class Multiplexer : public Frames<ENGINE>
        {
        public:
            template <typename DERIVED>
            explicit Multiplexer(const ArcPtr<DERIVED> &stc) :
            Frames<ENGINE>(stc)
            {
            }

            inline virtual ~Multiplexer() noexcept
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Multiplexer);
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
        Concurrent::PunctualFrame(ctx)
        {}

        inline virtual ~Demo() noexcept {}
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo);
    };

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


}

Y_UTEST(concurrent_frame0d)
{

    const Concurrent::Topology topo;
    Concurrent::SharedPipeline seqEngine = new Concurrent::Alone();
    Concurrent::SharedPipeline parEngine = new Concurrent::Queue(topo);

    Concurrent::Frames<Demo> seq(seqEngine);
    Concurrent::Frames<Demo> par(parEngine);

    std::cerr << "Empty" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;

    seq.attach();
    par.attach();
    std::cerr << "Attached" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;

    Worker worker;
    {
        const Duty duty(worker,seq);
        seqEngine->load(duty);
        seqEngine->flush();
    }


    seq.detach();
    par.detach();
    std::cerr << "Detached" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;


}
Y_UDONE()

