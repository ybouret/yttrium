
#include "y/ink/image/format/bmp.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Ink
    {
        FormatBMP::  FormatBMP() : Format(CallSign,"(bmp)&") {}
        FormatBMP:: ~FormatBMP() noexcept {}

        const char * const FormatBMP:: CallSign = "BMP";

    }
}

#include "y/stream/libc/output.hpp"

namespace Yttrium
{
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
                    Coerce(ppl) = 0;
                    memset(bmpfileheader,0,sizeof(bmpfileheader));
                    memset(bmpinfoheader,0,sizeof(bmpinfoheader));
                }


                static inline void Write32(unsigned char * const p,
                                           const uint32_t        i) noexcept
                {
                    assert(0!=p);
                    p[0] = (unsigned char)( i    );
                    p[1] = (unsigned char)( i>> 8);
                    p[2] = (unsigned char)( i>>16);
                    p[3] = (unsigned char)( i>>24);
                }

                static inline uint32_t Read32(const unsigned char * const p) noexcept
                {
                    assert(0!=p);
                    const uint32_t p0 = p[0];
                    const uint32_t p1 = p[1];
                    const uint32_t p2 = p[2];
                    const uint32_t p3 = p[3];
                    return p0 | (p1<<8) | (p2<<16) | (p3<<24);
                }


                static inline uint32_t GetPaddingPerLine(const uint32_t w) noexcept
                {
                    return (4-(w*3)%4)%4;
                }

                //! return pad per line
                inline void initialize(const uint32_t w,
                                       const uint32_t h) noexcept
                {
                    Coerce(ppl)             = GetPaddingPerLine(w);
                    const uint32_t filesize = 54+h*(3*w+ppl);
                    Write32(&bmpfileheader[2],filesize);
                    Write32(&bmpinfoheader[4],w);
                    Write32(&bmpinfoheader[8],h);
                }



                const uint32_t ppl;
                unsigned char  bmpfileheader[FileHeaderSize];
                unsigned char  bmpinfoheader[InfoHeaderSize];

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


        void FormatBMP:: save(const Image   & image,
                              const String  & fileName,
                              const Options * options) const
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

#include "y/stream/libc/input.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Ink
    {

        class InputBMP : public BMP
        {
        public:
            inline explicit InputBMP(InputStream &fp) : BMP(), w(0), h(0)
            {
                if( FileHeaderSize != fp.fetch(bmpfileheader,FileHeaderSize)) throw Specific::Exception(FormatBMP::CallSign,"corrupted file header");
                if( InfoHeaderSize != fp.fetch(bmpinfoheader,InfoHeaderSize)) throw Specific::Exception(FormatBMP::CallSign,"corrupted info header");
                Coerce(w) = Read32(&bmpinfoheader[4]);
                Coerce(h) = Read32(&bmpinfoheader[8]);
                Coerce(ppl)             = GetPaddingPerLine(w);
                const uint32_t filesize = 54+h*(3*w+ppl);
                if( Read32(&bmpfileheader[2]) != filesize ) throw Specific::Exception(FormatBMP::CallSign,"inconsitent filesize");
            }

            inline virtual ~InputBMP() noexcept
            {
                Coerce(w) = 0;
                Coerce(h) = 0;
            }

            inline void skip(InputStream &fp, const String &fn) const
            {
                unsigned char bmppad[4] = { 0, 0, 0, 0 };
                if( fp.fetch(bmppad,ppl) != ppl )
                {
                    throw Specific::Exception(FormatBMP::CallSign,"missing padding in '%s'", fn.c_str());
                }
            }

            const uint32_t w;
            const uint32_t h;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(InputBMP);
        };

        Codec::Image FormatBMP:: load(const String &fileName,
                                      const Options *options) const
        {
            (void)options;

            InputFile fp(fileName);
            InputBMP  op(fp);
            Image     image(op.w,op.h);

            std::cerr << "w=" << op.w << std::endl;
            std::cerr << "h=" << op.h << std::endl;


            for(unit_t j=image.h;j>0;)
            {
                ImageRow  &row  = image[--j];
                for(unit_t i=0;i<image.w;++i)
                {
                    RGBA &c = row[i];
                    if(1!=fp.fetch(c.b)) throw Specific::Exception(CallSign,"missing blue  in '%s'", fileName.c_str());
                    if(1!=fp.fetch(c.g)) throw Specific::Exception(CallSign,"missing green in '%s'", fileName.c_str());
                    if(1!=fp.fetch(c.r)) throw Specific::Exception(CallSign,"missing red   in '%s'", fileName.c_str());
                }
                op.skip(fp,fileName);
            }

            return image;
        }

    }

}
