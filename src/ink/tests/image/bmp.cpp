
#include "y/ink/image/format.hpp"
#include "y/utest/run.hpp"
#include "y/text/ops.hpp"
#include "y/stream/libc/output.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/random/bits.hpp"

using namespace Yttrium;

namespace Yttrium
{
    namespace Ink
    {

        static inline void bmp_w32(unsigned char * const p,
                                   const uint32_t i) noexcept
        {

            p[ 0] = (unsigned char)( i    );
            p[ 1] = (unsigned char)( i>> 8);
            p[ 2] = (unsigned char)( i>>16);
            p[ 3] = (unsigned char)( i>>24);
        }

        class FormatBMP : public Format
        {
        public:
            static const char * const CallSign;

            explicit FormatBMP();
            virtual ~FormatBMP() noexcept;
            


            void save(const Image         &image,
                      const String        &fileName,
                      const FormatOptions *options) const
            {
                (void)options;

                // initial headers
                unsigned char       bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
                unsigned char       bmpinfoheader[40] =
                {   40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0,
                    0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,
                    0,  0, 0, 0, 0, 0, 0, 0
                };
                const unsigned char bmppad[4] = {0,0,0,0};

                const uint32_t w        = uint32_t(image.w);
                const uint32_t h        = uint32_t(image.h);
                const uint32_t ppl      = (4-(w*3)%4)%4; //!< pad per line
                const uint32_t filesize = 54+h*(3*w+ppl);

                // fill headers
                bmp_w32(&bmpfileheader[2],filesize);
                bmp_w32(&bmpinfoheader[ 4],w);
                bmp_w32(&bmpinfoheader[ 8],h);

                OutputFile fp(fileName);
                fp.frame(bmpfileheader,14);
                fp.frame(bmpinfoheader,40);


                // write binary data
                for(unit_t j=image.h;j>0;)
                {
                    const ImageRow  &row  = image[--j];
                    //const uint8_t *curr = row.addr();
                    for(unit_t i=0;i<image.w;++i)
                    {
                        const RGBA c = row[i];
                        fp.write(c.b);
                        fp.write(c.g);
                        fp.write(c.r);
                    }
                    fp.frame(bmppad,ppl);
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(FormatBMP);
        };

        FormatBMP::  FormatBMP() : Format(CallSign,"(bmp)&") {}
        FormatBMP:: ~FormatBMP() noexcept {}

        const char * const FormatBMP:: CallSign = "BMP";
    }

    void LoadIndx(Ink::Slab          &slab,
                  Ink::Codec::Image &target,
                  Random::Bits      &ran)
    {

        Ink::RGBA color;
        {
            Y_LOCK(slab.sync);
            color.r = ran.in<uint8_t>(100,256);
            color.g = ran.in<uint8_t>(100,256);
            color.b = ran.in<uint8_t>(100,256);
        }
        for(size_t k=slab.count();k>0;--k)
        {
            const Ink::HSegment    s = slab.hseg[k];
            Ink::Codec::ImageRow &r = target[s.y];
            for(unit_t i=s.w,x=s.x;i>0;--i,++x) 
            {
                r[x] = color;
            }
        }
    }

}

Y_UTEST(format_bmp)
{

    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Ink::Slabs             slabs( crew );
    Ink::FormatOptions     opts;
    Ink::Format::Handle    fmtBMP = new Ink::FormatBMP();

    {
        Y_CHECK(fmtBMP->matches("hello.bmp"));
        Y_CHECK(fmtBMP->matches("Hello.BmP"));
    }

    Ink::Codec::Image img(200,100);
    Random::Rand      ran;
    slabs(LoadIndx,img,ran);


    fmtBMP->save(img, "img.bmp", 0);


    


}
Y_UDONE()

