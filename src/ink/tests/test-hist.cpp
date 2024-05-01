

#include "y/ink/image/codecs.hpp"
#include "y/utest/run.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/random/park-miller.hpp"
#include "y/ink/parallel/index-to-rgba.hpp"
#include "y/sequence/vector.hpp"
#include "y/color/grayscale.hpp"
#include "y/hashing/sha1.hpp"
#include "y/hashing/md.hpp"
#include "y/memory/digest.hpp"
#include "y/system/wtime.hpp"
#include "y/text/human-readable.hpp"
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

            Digest hashWith(Hashing::Function &H) const
            {
                return Hashing::MD::Of(H,data,sizeof(data));
            }


            inline T        & operator[](const uint8_t x)       noexcept { return data[x]; }
            inline const  T & operator[](const uint8_t x) const noexcept { return data[x]; }

            template <typename COLOR, typename PROC> inline
            void make(Slabs               &slabs,
                      const Pixmap<COLOR> &img,
                      PROC                &ColorToByte,
                      Pixmap<uint8_t>     &msk)
            {
                ldz();
                // compute sub-histograms
                slabs(Make<COLOR,PROC>,img,ColorToByte,msk);
                // collect
                for(size_t i=slabs.simt.size();i>0;--i)
                {
                    const T * const H = slabs.simt[i].as<size_t>(BINS);
                    for(size_t j=0;j<BINS;++j)
                    {
                        data[i] += H[i];
                    }
                }
            }




        private:
            Y_DISABLE_COPY_AND_ASSIGN(Histogram);
            T data[BINS];

            template <typename COLOR, typename PROC>
            static inline
            void Make(Slab                &slab,
                      const Pixmap<COLOR> &img,
                      PROC                &ColorToByte,
                      Pixmap<uint8_t>     &msk)
            {
                assert(msk.hasSameSizesThan(img));
                T * const H = slab.ldz().as<size_t>(BINS);
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
    Hashing::SHA1 hfn;
    WallTime      tmx;
    Codecs &      IMG = Ink::Codecs::Std();

    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Slabs                  par( crew );
    Histogram<size_t>      hist;

    if(argc>1)
    {
        (std::cerr << "Loading " << argv[1] << std::endl).flush();
        Pixmap<RGBA>    img = IMG.Codec::load(argv[1],0);

        Pixmap<uint8_t> msk(img.w,img.h);
        (std::cerr << "Building Histogram..." << std::endl).flush();
        {
            const uint64_t pixels = img.n;
            const uint64_t mark = WallTime::Ticks();
            hist.make(par,img, Color::GrayScale::From<RGBA>, msk);
            const uint64_t ell  = WallTime::Ticks() - mark;
            const long double rate = (static_cast<long double>(pixels) / tmx(ell))*1e-6L;
            std::cerr << "rate=" << rate << " Mpx/s" << std::endl;
        }

        const Digest    hh = hist.hashWith(hfn);
        std::cerr << "hash=" << hh << std::endl;
        Pixmap<RGBA>    tgt(par, Color::GrayScale::ByteTo<RGBA>, msk);

        (std::cerr << "Saving..." << std::endl).flush();
        IMG.save(img, "hist-img.png",0);
        IMG.save(tgt, "hist-msk.png",0);

    }




}
Y_UDONE()

