

#include "y/concurrent/loop/simt2d.hpp"
#include "y/concurrent/thread.hpp"

#include "y/utest/run.hpp"
#include "y/container/matrix.hpp"


#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/loop/crew.hpp"



#include "y/concurrent/thread.hpp"
#include "y/concurrent/topology.hpp"
#include "y/string/env.hpp"

using namespace Yttrium;

namespace
{
    class Tao2D : public Concurrent::Engine2D<size_t>
    {
    public:
        struct Strip
        {
            Type icol;
            Type irow;
            Size ncol;
            Type cend;
            inline friend std::ostream &operator<<(std::ostream &os, const Strip &s)
            {
                os << "@row=" << s.irow << ",col=" << s.icol << " : +" << s.ncol;
                return os;
            }
        };

        explicit Tao2D() noexcept : strip(0) {}
        virtual ~Tao2D() noexcept {}

        inline const Strip & operator()(const size_t indx) const noexcept
        {
            assert(0!=strip);
            assert(indx>0); assert(indx<=(*this)->size);
            return strip[indx];
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Tao2D);
        const Strip *strip;
        virtual void activate(const Concurrent::ThreadContext &cntx)
        {
            std::cerr << "in " << cntx.name << " : " << *this << std::endl;
            strip = (*this)->as<Strip>();
        }

        virtual void shutdown() noexcept 
        {
            strip = 0;
        }

    };

    inline void DoSomething(Tao2D &range)
    {
        Y_LOCK( range.sync() );
        std::cerr << "In " << Concurrent::Thread::CurrentHandle() << std::endl;
        const Tao2D::Tile &tile = *range;
        for(size_t i=range->size;i>0;--i)
        {
            const Tao2D::Segment &S = tile[i];
            const Tao2D::Strip   &s = range(i);
            std::cerr << "\t" << S << " => " << s << std::endl;
        }
    }
}

Y_UTEST(concurrent_simt2d)
{
    Concurrent::Thread::Verbose     = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;
    Concurrent::SharedLoop     seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop     parLoop = new Concurrent::Crew(topo);

    Concurrent::SIMT2D<Tao2D> seq(seqLoop);
    Concurrent::SIMT2D<Tao2D> par(parLoop);

    const size_t nrow = 5; // y
    const size_t ncol = 8; // x

    V2D<size_t> lower(1,1);
    V2D<size_t> upper(ncol,nrow);

    seq.dispatch(lower,upper);
    par.dispatch(lower,upper);

    std::cerr << "seq: " << std::endl;
    seq.call( DoSomething );

    std::cerr << std::endl;
    std::cerr << "par: " << std::endl;
    par.call( DoSomething );

    std::cerr << std::endl;

    std::cerr << "seq=" << seq << std::endl;
    std::cerr << "par=" << par << std::endl;


    seq.turnOff();
    par.turnOff();
    std::cerr << "seq=" << seq << std::endl;
    std::cerr << "par=" << par << std::endl;


}
Y_UDONE()

