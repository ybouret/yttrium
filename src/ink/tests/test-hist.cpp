

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

            //static inline void Make(Slab &slab);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Histogram);
            T data[BINS];
        };




        void MakeHist(Slab &slab, const Pixmap<RGBA> &img)
        {
            size_t * const H = slab.as<size_t>(256);
            memset(H,0,sizeof(size_t)*256);

            for(size_t k=slab.count();k>0;--k)
            {
                const Ink::HSegment    s = slab.hseg[k];
                const PixRow<RGBA>    &r = img[s.y];
                for(unit_t i=s.w,x=s.x;i>0;--i,++x)
                {
                    const uint8_t u = Color::GrayScale::Make(r(x));
                    ++H[u];
                }
            }
        }
    }
}

using namespace Yttrium;
using namespace Ink;


Y_UTEST(hist)
{

    Codecs & IMG = Ink::Codecs::Std();

    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Slabs                  slabs( crew );

    if(argc>1)
    {
        Pixmap<RGBA> img = IMG.Codec::load(argv[1],0);

        slabs(MakeHist,img);
        
        //IMG.save(img, "img.png",0);
    }




}
Y_UDONE()

