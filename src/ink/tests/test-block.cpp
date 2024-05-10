#include "y/ink/pixmap.hpp"
#include "y/utest/run.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/color/channels.hpp"
#include "y/color/grayscale.hpp"
#include "y/color/ramp/gradation.hpp"
#include "y/color/rgb/x11.hpp"

namespace Yttrium
{
    namespace Ink
    {


        template <size_t DeltaX, size_t DeltaY>
        class Block
        {
        public:
            static const unit_t X  = DeltaX;
            static const unit_t Y  = DeltaY;
            static const unit_t X0 = -X;
            static const unit_t Y0 = -Y;
            static const unit_t W  = X*2+1;
            static const unit_t H  = Y*2+1;
            static const size_t N  = W*H;

            explicit Block()   {}
            virtual ~Block() noexcept {}


            template <typename TARGET, typename SOURCE, typename PROC> inline
            void operator()(Slabs &slabs, Pixmap<TARGET> &target, PROC &proc, const Pixmap<SOURCE> &source)
            {
                slabs(Apply<TARGET,PROC,SOURCE>,target,*this,proc,source);
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Block);

            template <typename TARGET, typename PROC, typename SOURCE> static inline
            void Apply(Slab                 &slab,
                       Pixmap<TARGET>       &target,
                       Block                &block,
                       PROC                 &proc,
                       const Pixmap<SOURCE> &source)
            {
                // local memory for all slab operations
                static const size_t Required = N * sizeof(SOURCE);
                static const size_t NumWords = Y_WORDS_GEQ(Required);
                void   *       wksp[NumWords];
                SOURCE * const data = static_cast<SOURCE *>( Memory::OutOfReach::Addr(wksp) );
                for(size_t k=slab.count();k>0;--k)
                {
                    const HSegment  seg = slab[k];
                    const unit_t    y   = seg.y;
                    PixRow<TARGET> &tgt = target[y];
                    Coord           pos(seg.x,y);
                    for(unit_t i=seg.w;i>0;--i,++pos.x)
                    {
                        block.load(data,source,pos);
                        proc(tgt[pos.x],data);
                    }
                }
            }

            template <typename T> inline
            void load(T               *target,
                      const Pixmap<T> &source,
                      const Coord      origin) const
            {
                for(unit_t y=Y;y>=Y0;--y)
                {
                    const PixRow<T> &row = source[origin.y+y];
                    unit_t           x   = origin.x+X0;
                    for(unit_t i=W;i>0;--i,++x)
                    {
                        *(target++) = row[x];
                    }
                }
            }
        };

    }
}

using namespace Yttrium;
using namespace Ink;

template <size_t N, typename T> static inline
void BlockAverage(T &out, const T * const arr)
{
    assert(N>0);
    T sum = 0;
    for(size_t i=0;i<N;++i) sum += arr[i];
    out = sum/N;
}

template <size_t N, typename T> static inline
void BlockMin(T &out, const T * const arr)
{
    assert(N>0);
    T res = arr[0];
    for(size_t i=1;i<N;++i) res = Min(res,arr[i]);
    out = res;
}

template <size_t N, typename T> static inline
void BlockMax(T &out, const T * const arr)
{
    assert(N>0);
    T res = arr[0];
    for(size_t i=1;i<N;++i) res = Max(res,arr[i]);
    out = res;
}

#include "y/sort/nw.hpp"
template <size_t N, typename T> static inline
void BlockMed(T &out,  T * const arr)
{
    assert(N>0);
    T * const tableau = arr-1;
    NetworkSort::Algo<N>::Increasing(tableau);
    out = arr[N>>1];
}


Y_UTEST(block)
{
    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Slabs                  par( crew );
    const Color::RampColor bw[2] = { Y_Black, Y_White };
    const Color::Gradation cr(bw,2);

    Codec &IMG = Codecs::Std();

    typedef Block<1,1> Blk3x3;

    if(argc>1)
    {
        const Pixmap<RGBA>  img = IMG.load(argv[1],0);
        const Pixmap<float> pxf(par,Color::GrayScale::Pack<float,RGBA>,img);
        
        IMG.save(img, "img.png", 0);
        Pixmap<float>       out(img.w,img.h);
        Blk3x3              blk;
        IMG.save(pxf, "pxf.png", 0, par, cr);


        blk(par,out,BlockAverage<Blk3x3::N,float>,pxf);
        IMG.save(out, "ave.png", 0, par, cr);

        blk(par,out,BlockMin<Blk3x3::N,float>,pxf);
        IMG.save(out, "min.png", 0, par, cr);

        blk(par,out,BlockMax<Blk3x3::N,float>,pxf);
        IMG.save(out, "max.png", 0, par, cr);

        blk(par,out,BlockMed<Blk3x3::N,float>,pxf);
        IMG.save(out, "med.png", 0, par, cr);



    }


}
Y_UDONE()
