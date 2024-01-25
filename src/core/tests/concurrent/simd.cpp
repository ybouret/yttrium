#include "y/concurrent/loop/simd.hpp"

#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/loop/crew.hpp"

#include "y/concurrent/resource/in1d.hpp"
#include "y/concurrent/resource/in2d.hpp"

#include "y/concurrent/topology.hpp"
#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"

#include "y/container/matrix.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;

namespace Yttrium
{
    class Tao1D : public Concurrent::Resource1D<size_t>
    {
    public:
        explicit Tao1D(const Concurrent::ThreadContext &ctx) :
        Concurrent::Resource1D<size_t>(ctx)
        {}
        virtual ~Tao1D() noexcept {}



    private:
        Y_DISABLE_COPY_AND_ASSIGN(Tao1D);

        virtual void activate( ) {}

        virtual void shutdown() noexcept {}

    };

    inline void DoSomething(Tao1D &range)
    {
        Y_LOCK(range.sync);
        (std::cerr << "DoSomething(" << range << ")" << std::endl).flush();
    }

    class Working
    {
    public:

        explicit Working() noexcept {}
        virtual ~Working() noexcept {}

        inline void operator()(Tao1D &range, const int a)
        {
            Y_LOCK(range.sync);
            (std::cerr << "Working(" << range << "," << a << ")" << std::endl).flush();
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Working);
    };


    template <typename TARGET, typename SOURCE>
    inline void Load(Tao1D &range, TARGET &target, SOURCE &source)
    {
        {
            Y_LOCK(range.sync);
            (std::cerr << "Load(" << range << ")" << std::endl).flush();
            Tao1D::Type i = range.offset;
            for(Tao1D::Size j=range.length;j>0;--j,++i)
            {
                target[i] = source[i];
            }
        }

    }


    class Tao2D : public Concurrent::Resource2D<size_t>
    {
    public:
        explicit Tao2D(const Concurrent::ThreadContext &cntx) : Concurrent::Resource2D<size_t>(cntx) {}
        virtual ~Tao2D() noexcept {}


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Tao2D);
        virtual void activate()
        {
        }

        virtual void shutdown() noexcept {}

    };

    inline void DoSomething2D(Tao2D &range)
    {

        {
            Y_LOCK(range.sync);
            (std::cerr << "DoSomething2D(" << range << ")" << std::endl).flush();

            if(range.isEmpty()) return;

            //const Concurrent::Tiling<size_t>::Tile &tile = *range;
            const Tao2D::Tile &tile = *range;
            for(size_t j=1;j<=tile.size;++j)
            {
                const Tao2D::Segment &s = tile[j];
                size_t       x = s.start.x;
                const size_t y = s.start.y;
                std::cerr << "y=" << y << ":";
                for(size_t i=s.width;i>0;--i,++x)
                {
                    std::cerr << " " << x;
                }
                std::cerr << std::endl;
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
        std::cerr << "(-) seq=" << seq << std::endl;
        std::cerr << "(-) par=" << par << std::endl;
        seq.init(1, 10, 1);
        par.init(1, 10, 1);
        std::cerr << "(+) seq=" << seq << std::endl;
        std::cerr << "(+) par=" << par << std::endl;
        std::cerr << "---- seq()" << std::endl;
        seq();
        std::cerr << "---- par()" << std::endl;
        par();
        std::cerr << "---- seq(DoSometing)" << std::endl;
        seq(DoSomething);
        std::cerr << "---- par(DoSometing)" << std::endl;
        par(DoSomething);




        Working   w;
        const int n = 3;
        std::cerr << "---- seq(working,n)" << std::endl;
        seq(w,n);
        std::cerr << "---- par(working,n)" << std::endl;
        par(w,n);

        CxxArray<double> target(30);
        CxxArray<int>    source(20);
        for(size_t i=1;i<=source.size();++i)
        {
            source[i] = int(i);
        }

        std::cerr << "---- seq(Load)" << std::endl;
        seq.init(1,source.size(),1);
        seq(Load< Writable<double>,const Readable<int> >,target,source);
        std::cerr << "target=" << target << std::endl;

        std::cerr << "---- par(Load)" << std::endl;
        memset( &target[1], 0, sizeof(double)*target.size() );
        std::cerr << "target=" << target << std::endl;
        par.init(1,source.size(),1);
        par(Load< Writable<double>,const Readable<int> >,target,source);
        std::cerr << "target=" << target << std::endl;
    }

    std::cerr << std::endl;
    {
        Concurrent::SIMD<Tao2D>    seq( seqLoop );
        Concurrent::SIMD<Tao2D>    par( parLoop );

        const size_t rows = 5;
        const size_t cols = 4;
        V2D<size_t>  lower(1,1);
        V2D<size_t>  upper(cols,rows);

        seq.init(lower,upper);
        par.init(lower,upper);
        seq(DoSomething2D);
        par(DoSomething2D);

    }

}
Y_UDONE()
