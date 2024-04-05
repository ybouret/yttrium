
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

#include "y/sequence/vector.hpp"


using namespace Yttrium;


namespace
{

    class XNode : public Object
    {
    public:
        typedef CxxListOf<XNode> List;

        inline explicit XNode() noexcept : Object(),   next(0), prev(0) {}
        inline virtual ~XNode() noexcept {}

        XNode *next;
        XNode *prev;
        WallTime      tmx;
        Random::Rand ran;

        void wait() {
            tmx.wait( 0.1 * ran.to<double>() );
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(XNode);
    };

    class XList : public XNode::List
    {
    public:
        explicit XList() noexcept {}
        virtual ~XList() noexcept {}

        void make(const size_t n)
        {
            while(size>n) delete popTail();
            while(size<n) pushTail( new XNode() );
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(XList);
    };


    class X1D : public Concurrent::Frame1D<size_t>
    {
    public:

        inline explicit X1D(const ThreadContext &ctx) noexcept : 
        Frame1D<size_t>(ctx), node(0)
        {
        }

        inline virtual ~X1D() noexcept
        {
        }

        inline void link(XNode *xn)
        {
            node = xn;
        }

        inline void unlink() noexcept
        {
            node = 0;
        }


        XNode *node;

    private:
        Y_DISABLE_ASSIGN(X1D);
    };

    inline void DoSomething0(X1D &range)
    {
        {
            Y_LOCK(range.sync);
            (std::cerr << "\tDoSomething0(" << X1D::LoopPtr(range.loop) << ")" << std::endl).flush();
        }
    }

    inline void DoSomething1(X1D               &range,
                             const Vector<int> &v)
    {
        {
            Y_LOCK(range.sync);
            (std::cerr << "\tDoSomething1(" << X1D::LoopPtr(range.loop) << ")" << std::endl).flush();
        }
        const Concurrent::ForLoop<size_t> * const loop = range.loop;
        int sum = 0;
        if(loop)
        {
            for(size_t i=loop->offset,j=loop->length;j>0;--j,i+=loop->update)
            {
                sum += v[i];
                range.node->wait();
            }
        }
        {
            {
                Y_LOCK(range.sync);
                (std::cerr << "@" << range.name << ": sum = " << sum << std::endl).flush();
            }
        }
    }

}




Y_UTEST(concurrent_simtx)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");

    const Concurrent::Topology topo;
    Concurrent::SharedLoop     seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop     parLoop = new Concurrent::Crew(topo);

    {

        Concurrent::SIMT<X1D> seq( seqLoop );
        Concurrent::SIMT<X1D> par( parLoop );
        XList                 xls;

        std::cerr << "-- Testing" << std::endl;
        seq();
        par();
        std::cerr << std::endl;

        seq.assign(1,10,2);
        par.assign(1,10,2);
        std::cerr << "-- Testing 0-arg" << std::endl;
        seq(DoSomething0);
        par(DoSomething0);
        std::cerr << std::endl;

        std::cerr << "-- Testing 1-arg" << std::endl;
        Vector<int> v(10,0);
        for(int i=10;i>0;--i) v[i] = i;

        {
            xls.make(seq.size());
            const Concurrent::SIMT<X1D>::AutoUnlink keep(seq);
            seq.link(xls.head);
            seq(DoSomething1,v);
        }

        {
            xls.make(par.size());
            const Concurrent::SIMT<X1D>::AutoUnlink keep(par);
            par.link(xls.head);
            par(DoSomething1,v);
        }

    }

}
Y_UDONE()
