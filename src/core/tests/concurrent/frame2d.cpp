
#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/frame/2d.hpp"
#include "y/concurrent/frames.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;

namespace
{
    //__________________________________________________________________
    //
    //
    //
    //! Strip matching Tiling<size_t>::Segment
    //
    //
    //__________________________________________________________________
    struct Strip
    {
        unit_t icol; //!< initial column
        unit_t irow; //!< initial row
        unit_t ncol; //!< number of columns
        unit_t cend; //!< end column

        inline friend std::ostream & operator<<(std::ostream &os, const Strip &S)
        {
            os << "(icol=" << S.icol << ",irow=" << S.irow <<",ncol=" << S.ncol << ",cend=" << S.cend << ")";
            return os;
        }
    };

    class Demo : public Concurrent::Frame2D<unit_t>
    {
    public:
        inline explicit Demo(const Concurrent::ThreadContext &ctx) noexcept :
        Concurrent::Frame2D<unit_t>(ctx),
        strip(0)
        {}

        const Strip * const strip;

        inline virtual ~Demo() noexcept {}


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Demo);
    };
}


Y_UTEST(concurrent_frame2d)
{
    const Concurrent::Topology topo;
    Concurrent::SharedLoop     seqEngine = new Concurrent::Mono();
    Concurrent::SharedLoop     parEngine = new Concurrent::Crew(topo);

    Concurrent::Frames<Demo> seq(seqEngine);
    Concurrent::Frames<Demo> par(parEngine);

    std::cerr << "Empty" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;

    const V2D<unit_t> lower(1,1);
    const V2D<unit_t> upper(10,8);

    seq.assign(lower,upper);
    par.assign(lower,upper);
    std::cerr << "Attached" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;
 
    for(size_t i=1;i<=par.size();++i)
    {
        std::cerr << "sub[" << std::setw(4) << i << "] = " << *par[i] << std::endl;
    }


    seq.loosen();
    par.loosen();
    std::cerr << "Detached" << std::endl;
    std::cerr << "  seq=" << seq << std::endl;
    std::cerr << "  par=" << par << std::endl;
    std::cerr << std::endl;

    std::cerr << "-- Testing transmogrify" << std::endl;
    Concurrent::Tiling<unit_t>::Segment seg[4] =
    { 
        Concurrent::Tiling<unit_t>::Segment(lower,1),
        Concurrent::Tiling<unit_t>::Segment(lower,2),
        Concurrent::Tiling<unit_t>::Segment(lower,3),
        Concurrent::Tiling<unit_t>::Segment(lower,4)
    };

    Core::Display(std::cerr, seg, 4) << std::endl;
    Y_CHECK(sizeof(Strip)==sizeof( Concurrent::Tiling<unit_t>::Segment) );

    const Concurrent::Tiling<unit_t>::Segment *   s = 0 ;
    const Strip * const & S = *(const Strip **) &s;
    std::cerr << "S@" << S << std::endl;
    s = seg;
    std::cerr << "S@" << S << std::endl;
    Core::Display(std::cerr, S, 4) << std::endl;


}
Y_UDONE()

