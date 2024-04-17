
#include "y/ink/image/format.hpp"
#include "y/utest/run.hpp"
#include "y/text/ops.hpp"
#include "y/stream/libc/output.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/random/park-miller.hpp"
#include <cstring>

using namespace Yttrium;

namespace Yttrium
{
    namespace Ink
    {


        namespace {

#define Y_INK_BMP_FILE_HEADER_SIZE 14
#define Y_INK_BMP_FILE_HEADER_INIT {'B','M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0 }

#define Y_INK_BMP_INFO_HEADER_SIZE 40
#define Y_INK_BMP_INFO_HEADER_INIT   \
{                                    \
/**/ 40, 0, 0, 0,  0, 0, 0, 0, 0, 0, \
/**/  0, 0, 1, 0, 24, 0, 0, 0, 0, 0, \
/**/  0, 0, 0, 0,  0, 0, 0, 0, 0, 0, \
/**/  0, 0, 0,  0, 0, 0, 0, 0, 0, 0  \
}
            class BMP
            {
            public:
                static const size_t        FileHeaderSize = Y_INK_BMP_FILE_HEADER_SIZE;
                static const unsigned char FileHeaderInit[FileHeaderSize];

                static const size_t        InfoHeaderSize = Y_INK_BMP_INFO_HEADER_SIZE;
                static const unsigned char InfoHeaderInit[InfoHeaderSize];

                inline BMP() noexcept :
                ppl(0),
                bmpfileheader(),
                bmpinfoheader()
                {
                    memcpy(bmpfileheader,FileHeaderInit,FileHeaderSize);
                    memcpy(bmpinfoheader,InfoHeaderInit,InfoHeaderSize);
                }

                inline virtual ~BMP() noexcept
                {
                    memset(bmpfileheader,0,sizeof(bmpfileheader));
                    memset(bmpinfoheader,0,sizeof(bmpinfoheader));
                }


                static inline void Write32(unsigned char * const p,
                                           const uint32_t        i) noexcept
                {

                    p[0] = (unsigned char)( i    );
                    p[1] = (unsigned char)( i>> 8);
                    p[2] = (unsigned char)( i>>16);
                    p[3] = (unsigned char)( i>>24);
                }

                static inline uint32_t GetPaddingPerLine(const uint32_t w) noexcept
                {
                    return (4-(w*3)%4)%4;
                }

                //! return pad per line
                inline void initialize(const uint32_t w,
                                       const uint32_t h) noexcept
                {
                    ppl      = GetPaddingPerLine(w);
                    const uint32_t filesize = 54+h*(3*w+ppl);
                    Write32(&bmpfileheader[2],filesize);
                    Write32(&bmpinfoheader[4],w);
                    Write32(&bmpinfoheader[8],h);
                }



                uint32_t      ppl;
                unsigned char bmpfileheader[FileHeaderSize];
                unsigned char bmpinfoheader[InfoHeaderSize];

            private:
                Y_DISABLE_COPY_AND_ASSIGN(BMP);
            };

            const unsigned char BMP::FileHeaderInit[BMP::FileHeaderSize] = Y_INK_BMP_FILE_HEADER_INIT ;
            const unsigned char BMP::InfoHeaderInit[BMP::InfoHeaderSize] = Y_INK_BMP_INFO_HEADER_INIT ;

            class OutputBMP : public BMP
            {
            public:
                explicit OutputBMP(OutputStream &fp, const Bitmap &image) noexcept :
                BMP()
                {
                    initialize( uint32_t(image.w), uint32_t(image.h) );
                    fp.frame(bmpfileheader,FileHeaderSize);
                    fp.frame(bmpinfoheader,InfoHeaderSize);
                }

                inline virtual ~OutputBMP() noexcept
                {
                }

                inline void fill(OutputStream &fp) const
                {
                    static const unsigned char bmppad[4] = {0,0,0,0};
                    fp.frame(bmppad,ppl);
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(OutputBMP);
            };

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

                //--------------------------------------------------------------
                //
                // open file and write formatted header
                //
                //--------------------------------------------------------------
                OutputFile fp(fileName);
                OutputBMP  op(fp,image);

                //--------------------------------------------------------------
                //
                // write each line, filled with 0
                //
                //--------------------------------------------------------------
                for(unit_t j=image.h;j>0;)
                {
                    const ImageRow  &row  = image[--j];
                    for(unit_t i=0;i<image.w;++i)
                    {
                        const RGBA c = row[i];
                        fp.write(c.b);
                        fp.write(c.g);
                        fp.write(c.r);
                    }
                    op.fill(fp);
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
            color.r = ran.in<uint8_t>(100,255);
            color.g = ran.in<uint8_t>(100,255);
            color.b = ran.in<uint8_t>(100,255);
            (std::cerr << "ran @" << (void*)&ran <<", color[" << slab.indx << "]=" << color << std::endl).flush();
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

    Ink::Codec::Image  img(200,100);
    Random::ParkMiller ran;
    std::cerr << "ran @" << (void*)&ran << std::endl;
    slabs(LoadIndx,img,ran);


    fmtBMP->save(img, "img.bmp", 0);


    


}
Y_UDONE()

