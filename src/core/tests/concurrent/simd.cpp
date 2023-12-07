#include "y/concurrent/loop/simd.hpp"

#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/loop/crew.hpp"

#include "y/concurrent/engine/in1d.hpp"
#include "y/concurrent/engine/in2d.hpp"

#include "y/concurrent/topology.hpp"
#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"

#include "y/container/matrix.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;

namespace Yttrium
{
    class Tao1D : public Concurrent::Engine1D<size_t>
    {
    public:
        explicit Tao1D() noexcept  {}
        virtual ~Tao1D() noexcept {}



    private:
        Y_DISABLE_COPY_AND_ASSIGN(Tao1D);

        virtual void activate(const Concurrent::ThreadContext &cntx)
        {
            std::cerr << "Activating Context " << cntx.name << std::endl;
        }

        virtual void shutdown() noexcept {}

    };

    inline void DoSomething(Tao1D &range)
    {

        Y_LOCK(range.sync());
        (std::cerr << "DoSomething(" << range << ")" << std::endl).flush();
    }

    class Working
    {
    public:

        explicit Working() noexcept {}
        virtual ~Working() noexcept {}

        inline void operator()(Tao1D &range, const int a)
        {
            Y_LOCK(range.sync());
            (std::cerr << "Working(" << range << "," << a << ")" << std::endl).flush();
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Working);
    };


    template <typename TARGET, typename SOURCE>
    inline void Load(Tao1D &range, TARGET &target, SOURCE &source)
    {
        {
            Y_LOCK(range.sync());
            (std::cerr << "Load(" << range << ")" << std::endl).flush();
            Tao1D::Type i = range.offset;
            for(Tao1D::Size j=range.length;j>0;--j,++i)
            {
                target[i] = source[i];
            }
        }

    }


    class Tao2D : public Concurrent::Engine2D<size_t>
    {
    public:
        explicit Tao2D() noexcept {}
        virtual ~Tao2D() noexcept {}


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Tao2D);
        virtual void activate(const Concurrent::ThreadContext &cntx)
        {
            std::cerr << "Activating 2D Context " << cntx.name << ": " << *this << std::endl;
        }

        virtual void shutdown() noexcept {}

    };

    inline void DoSomething2D(Tao2D &range)
    {

        {
            Y_LOCK(range.sync());
            (std::cerr << "DoSomething2D(" << range << ")" << std::endl).flush();


            //const Concurrent::Tiling<size_t>::Tile &tile = *range;
            const Tao2D::Tile &tile = *range;
            for(size_t j=1;j<=tile.size;++j)
            {
                const Tao2D::Segment &s = tile[j];
                size_t       x = s.start.x;
                const size_t y = s.start.y;
                std::cerr << "y=" << y << std::endl;
                for(size_t i=s.width;i>0;--i,++x)
                {
                    std::cerr << "\tx=" << x << std::endl;
                }
            }

            for(Tao2D::Iterator it=range->begin();it!=range->end();++it)
            {

            }

        }
    }



}

#include <cstring>

Y_UTEST(concurrent_simd)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");

    const Concurrent::Topology topo;
    Concurrent::SharedLoop     seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop     parLoop = new Concurrent::Crew(topo);

    {
        Concurrent::SIMD<Tao1D>    seq( seqLoop );
        Concurrent::SIMD<Tao1D>    par( parLoop );

        std::cerr << std::endl;
        std::cerr << "seq=" << seq << std::endl;
        seq.dispatch(1,10,1);
        std::cerr << "seq=" << seq << std::endl;
        seq.call();
        seq.call( DoSomething );


        std::cerr << std::endl;
        std::cerr << "par=" << par << std::endl;
        par.dispatch(1,10,1);
        std::cerr << "par=" << par << std::endl;
        par.call();
        par.call( DoSomething );


        Working   w;
        const int n = 3;
        seq.call(w,n);
        par.call(w,n);

        CxxArray<double> target(30);
        CxxArray<int>    source(20);
        for(size_t i=1;i<=source.size();++i)
        {
            source[i] = int(i);
        }

        seq.dispatch(1,source.size(),1);
        seq.call(Load< Writable<double>,const Readable<int> >,target,source);
        std::cerr << "target=" << target << std::endl;

        memset( &target[1], 0, sizeof(double)*target.size() );
        std::cerr << "target=" << target << std::endl;
        par.dispatch(1,source.size(),1);
        par.call(Load< Writable<double>,const Readable<int> >,target,source);
        std::cerr << "target=" << target << std::endl;
    }

    std::cerr << std::endl;
    {
        Concurrent::SIMD<Tao2D>    seq( seqLoop );
        Concurrent::SIMD<Tao2D>    par( parLoop );

        const size_t rows = 8;
        const size_t cols = 5;
        V2D<size_t>  lower(1,1);
        V2D<size_t>  upper(cols,rows);


        seq.dispatch(lower,upper);
        par.dispatch(lower,upper);
        seq.call(DoSomething2D);
        par.call(DoSomething2D);

    }


}
Y_UDONE()
