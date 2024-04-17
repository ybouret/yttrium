
#include "y/ink/image/format/bmp.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Ink
    {
        FormatBMP::  FormatBMP() : Format(CallSign,"(bmp)&") {}
        FormatBMP:: ~FormatBMP() noexcept {}

        const char * const FormatBMP:: CallSign = "BMP";

    }


    namespace Ink
    {

        namespace {


            class BMP
            {
            public:
                static const size_t        FileHeaderSize = 14;
                static const unsigned char FileHeaderInit[FileHeaderSize];

                static const size_t        InfoHeaderSize = 40;
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
                    ppl = 0;
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

            const unsigned char BMP::FileHeaderInit[BMP::FileHeaderSize] = {'B','M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0 };
            const unsigned char BMP::InfoHeaderInit[BMP::InfoHeaderSize] = {               
                /**/ 40, 0, 0, 0,  0, 0, 0, 0, 0, 0,
                /**/  0, 0, 1, 0, 24, 0, 0, 0, 0, 0,
                /**/  0, 0, 0, 0,  0, 0, 0, 0, 0, 0,
                /**/  0, 0, 0,  0, 0, 0, 0, 0, 0, 0
            };

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


        void FormatBMP:: save(const Image         &image,
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

    }
}
