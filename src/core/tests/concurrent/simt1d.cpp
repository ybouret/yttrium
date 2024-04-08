#include "y/concurrent/loop/simt/execute.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/utest/run.hpp"


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
            Y_ASSERT(this->isAssigned());
            {
                Y_LOCK(sync);
                std::cerr << "  (*) Demo1D.call0(" << *this << ")" << std::endl;
            }
            tmx.wait( ran.to<double>() );
        }

        void call1(const int a)
        {
            Y_ASSERT(this->isAssigned());
            {
                Y_LOCK(sync);
                std::cerr << "  (*) Demo1D.call1(" << a << "," << *this << ")" << std::endl;
            }
            tmx.wait( ran.to<double>() );
        }

        void call1bis(double &sum)
        {
            Y_ASSERT(this->isAssigned());
            {
                Y_LOCK(sync);
                std::cerr << "  (*) Demo1D.call1bis(" << sum << "," << *this << ")" << std::endl;
                sum += indx;
            }
            tmx.wait( ran.to<double>() );
        }

        void call2(Writable<double> &sum, const int a)
        {
            Y_ASSERT(this->isAssigned());
            {
                Y_LOCK(sync);
                std::cerr << "  (*) Demo1D.call2(" << sum << "," << a <<", " << *this << ")" << std::endl;
            }

            const Mapping &sub = **this;
            if(sub.length>0)
            {
                Y_ASSERT(0!=trek);
                double &target = sum[indx];
                for(size_t i=trek->offset;i<=trek->latest;i+=trek->update)
                {
                    target += i * a;
                }
            }
            else
            {
                Y_ASSERT(0==trek);
            }
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
#include "y/sequence/vector.hpp"

Y_UTEST(concurrent_simt1d)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");

    const Concurrent::Topology topo;
    Concurrent::SharedLoop     seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop     parLoop = new Concurrent::Crew(topo);


    //--------------------------------------------------------------------------
    //
    // Tests in 1D
    //
    //--------------------------------------------------------------------------
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


    {
        std::cerr << "Execute/2" << std::endl;
        typedef Concurrent::Execute<Demo1D,TL2(Writable<double> &, const int)> Execute2;

        Vector<double> sum;
        sum.adjust(seq.size(),0);
        Execute2::On(seq, &Demo1D::call2, sum, 3);
        std::cerr << "sum.seq=" << sum << std::endl;

        sum.adjust(par.size(),0); sum.ld(0);
        Execute2::On(par, &Demo1D::call2, sum, 3);
        std::cerr << "sum.par=" << sum << std::endl;

    }


}
Y_UDONE()


