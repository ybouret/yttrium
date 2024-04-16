#include "y/ink/pixmap.hpp"
#include "y/utest/run.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"


using namespace Yttrium;

namespace Yttrium
{

    namespace Ink
    {
        template <typename T>
        void LoadZero(Slab &slab, Pixmap<T> &target)
        {
            for(size_t k=slab.count();k>0;--k)
            {
                const HSegment               s = slab.hseg[k];
                typename Pixmap<T>::RowType &r = target[s.y];
                for(unit_t i=s.w,x=s.x;i>0;--i,++x) r[x] = 0;
            }
        }

        template <typename T>
        void LoadIndex(Slab &slab, Pixmap<T> &target )
        {
            for(size_t k=slab.count();k>0;--k)
            {
                const HSegment               s = slab.hseg[k];
                typename Pixmap<T>::RowType &r = target[s.y];
                for(unit_t i=s.w,x=s.x;i>0;--i,++x) r[x] = slab.indx;
            }
        }

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

    {
        Ink::Pixmap<int> ipix(2,2);
        seq.split(ipix);
        std::cerr << seq.simt << std::endl;
        par.split(ipix);
        std::cerr << par.simt << std::endl;

        seq(Ink::LoadZero<int>,ipix);
        par(Ink::LoadZero<int>,ipix);

        seq.simt.loosen();
        par.simt.loosen();

        int value = 1;
        seq(Ink::Load<int>,ipix,value);
        std::cerr << ipix << std::endl;
        value = 2;
        par(Ink::Load<int>,ipix,value);
     
        std::cerr << ipix << std::endl;
    }

    {
        Ink::Pixmap<size_t> indx(8,10);
        par.split(indx);
        par(Ink::LoadIndex<size_t>,indx);
        std::cerr << indx << std::endl;
    }

    Y_USHOW(Ink::Slab::Request);


}
Y_UDONE()
