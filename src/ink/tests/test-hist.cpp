

#include "y/ink/image/codecs.hpp"
#include "y/utest/run.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/random/park-miller.hpp"
#include "y/ink/parallel/index-to-rgba.hpp"
#include "y/sequence/vector.hpp"
#include "y/color/grayscale.hpp"
#include <cstring>

namespace Yttrium
{
    namespace  Ink
    {

        template <typename T>
        class Histogram
        {
        public:
            static const unsigned BINS = 256;
            explicit Histogram() noexcept : data()
            {
                ldz();
            }

            virtual ~Histogram() noexcept
            {
                ldz();
            }

            void ldz() noexcept {
                memset(data,0,sizeof(data));
            }

            inline T        & operator[](const uint8_t x)       noexcept { return data[x]; }
            inline const  T & operator[](const uint8_t x) const noexcept { return data[x]; }

            template <typename COLOR, typename PROC> inline
            void make(Slabs &slabs, const Pixmap<COLOR> &img, PROC &ColorToByte, Pixmap<uint8_t> &msk)
            {
                slabs(Make<COLOR,PROC>,img,ColorToByte,msk);
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Histogram);
            T data[BINS];

            template <typename COLOR, typename PROC>
            static inline
            void Make(Slab &slab, const Pixmap<COLOR> &img, PROC &ColorToByte, Pixmap<uint8_t> &msk)
            {
                assert(msk.hasSameSizesThan(img));
                T * const H = slab.ldz().as<size_t>(256);
                for(size_t k=slab.count();k>0;--k)
                {
                    const Ink::HSegment    s = slab.hseg[k];
                    const PixRow<RGBA>    &r = img[s.y];
                    PixRow<uint8_t>       &m = msk[s.y];
                    for(unit_t i=s.w,x=s.x;i>0;--i,++x)
                    {
                        const uint8_t u = ColorToByte(r(x));
                        ++H[ m[x] = u ];
                    }
                }
            }
        };



        
    }
}

using namespace Yttrium;
using namespace Ink;


Y_UTEST(hist)
{

    Codecs & IMG = Ink::Codecs::Std();

    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Slabs                  par( crew );
    Histogram<size_t>      hist;

    if(argc>1)
    {
        Pixmap<RGBA>    img = IMG.Codec::load(argv[1],0);
        Pixmap<uint8_t> msk(img.w,img.h);
        
        hist.make(par,img, Color::GrayScale::From<RGBA>, msk);
        Pixmap<RGBA>    tgt(par, Color::GrayScale::ByteTo<RGBA>, msk);

        
        IMG.save(img, "hist-img.png",0);
        IMG.save(tgt, "hist-msk.png",0);

    }




}
Y_UDONE()

