
#include "y/ink/pixmap.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/frame/2d.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"


using namespace Yttrium;

namespace Yttrium
{

    namespace Ink
    {
        typedef Concurrent::Frame2D<unit_t> ParallelFrame;

        //__________________________________________________________________
        //
        //
        //
        //! HSegment matching Tiling<unit_t>::Segment
        //
        //
        //__________________________________________________________________
        struct HSegment
        {
            unit_t x;  //!< x
            unit_t y;  //!< y
            unit_t w;  //!M width
            unit_t xt; //!< x top
        };

        class Slab : public ParallelFrame
        {
        public:
            
            explicit Slab(const ThreadContext &ctx) noexcept : 
            ParallelFrame(ctx),
            hseg( *(const HSegment **) Memory::OutOfReach::Addr(&segment) )
            {
            }

            virtual ~Slab() noexcept {}

            const HSegment * const & hseg; //!< transmogrified segments [1..tile->size] if not NULL
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Slab);
        };

        class Slabs : public Concurrent::SIMT<Slab>
        {
        public:
            explicit Slabs(const Concurrent::SharedLoop &csl) : Concurrent::SIMT<Slab>(csl) {}
            virtual ~Slabs() noexcept {}

            void setup(const Area &area)
            {
                assign(area.lower(),area.upper());
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Slabs);
        };
        
        template <typename T>
        void Load(Slab &slab, Pixmap<T> &target, typename Pixmap<T>::ParamType value)
        {
            for(size_t k=slab.count();k>0;--k)
            {
                const HSegment               s = slab.hseg[k];
                typename Pixmap<T>::RowType &r = target[s.y];
                for(unit_t i=s.w,x=s.x;i>0;--i,++x) r[x] = value;
            }
        }


    }

}


Y_UTEST(tess)
{
    Concurrent::Topology   topo;
    Concurrent::SharedLoop mono = new Concurrent::Mono();
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);

    Ink::Slabs seq( mono );
    Ink::Slabs par( crew );

    Ink::Pixmap<int> ipix(2,2);
    seq.setup(ipix);
    std::cerr << seq << std::endl;
    par.setup(ipix);
    std::cerr << par << std::endl;

    int value = 1;
    seq(Ink::Load<int>,ipix,value);
    std::cerr << ipix << std::endl;
    value = 2;
    par(Ink::Load<int>,ipix,value);
    std::cerr << ipix << std::endl;




}
Y_UDONE()
