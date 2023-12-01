#include "y/mkl/tao/level1.hpp"

#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/loop/simd.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"
#include "y/sequence/vector.hpp"


#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
    class TaoRes1D : public Concurrent::Resource1D<size_t>
    {
    public:
        explicit TaoRes1D() noexcept {}
        virtual ~TaoRes1D() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(TaoRes1D);

        virtual void attach(const Concurrent::ThreadContext &)
        {

        }
    };


    class TaoRes2D : public Concurrent::Resource2D<size_t>
    {
    public:
        explicit TaoRes2D() noexcept {}
        virtual ~TaoRes2D() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(TaoRes2D);

        virtual void attach(const Concurrent::ThreadContext &)
        {

        }
    };



#if 0
    template <typename TARGET, typename SOURCE>   inline
    void Load(TARGET &target, SOURCE &source, Concurrent::SIMD &simd)
    {
        assert(target.size()<=source.size());
        struct Work
        {
            void operator()(const Concurrent::Range &range, TARGET &target, SOURCE &source)
            {
                for(size_t i=range.offset,k=range.length;k>0;--k,++i)
                {
                    target[i] = source[i];
                }
            }
        };

        Work todo = {};
        simd(todo,target,source);


        // for(size_t i=target.size();i>0;--i)             target[i] = source[i];
    }
#endif

}

Y_UTEST(tao)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;

    typedef Concurrent::SIMD<size_t,TaoRes1D> Tao1D;
    typedef Concurrent::SIMD<size_t,TaoRes2D> Tao2D;

    Concurrent::SharedLoop seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop parLoop = new Concurrent::Crew(topo);

    Tao1D seq( seqLoop );
    Tao1D par( parLoop );
    
    size_t n = 4;
    size_t m = 5;


    std::cerr << std::endl;
    std::cerr << "seq=" << seq << std::endl;
    seq.dispatch(1,n,1);
    std::cerr << "seq=" << seq << std::endl;

    std::cerr << std::endl;
    std::cerr << "par=" << par << std::endl;
    par.dispatch(1,n,1);
    std::cerr << "par=" << par << std::endl;

    Tao2D seq2d( seqLoop );
    std::cerr << std::endl;
    std::cerr << "seq2d=" << seq2d << std::endl;
    seq2d.dispatch(V2D<size_t>(1,1),V2D<size_t>(n,m));
    std::cerr << "seq2d=" << seq2d << std::endl;




#if 0
    Concurrent::SIMD<size_t> seq( new Concurrent::Mono()     );
    Concurrent::SIMD<size_t> par( new Concurrent::Crew(topo) );



    size_t n = 4;
    seq.dispatch(1,n,1);
    std::cerr << "seq=" << seq << std::endl;
    par.dispatch(1,n,1);
    std::cerr << "par=" << par << std::endl;

    std::cerr << std::endl;
    seq();

    std::cerr << std::endl;
    par();
#endif
    
#if 0
    std::cerr << std::endl;
    seq();

    std::cerr << std::endl;
    par();

    std::cerr << std::endl;

    Vector<double> v(5,0);
    Vector<int>    u(5,0);

    Load(v,u,seq);
    
#endif



}
Y_UDONE()


