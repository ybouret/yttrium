
#include "y/ink/image/format/png.hpp"
#include "y/system/exception.hpp"
#include "y/stream/libc/input.hpp"
#include "y/stream/libc/output.hpp"
#include "y/png/png.h"
#include "y/text/ascii/convert.hpp"
#include "y/string/boolean.hpp"

namespace Yttrium
{
    namespace Ink
    {
        FormatPNG::  FormatPNG() : Format(CallSign,"(png)&") {}
        FormatPNG:: ~FormatPNG() noexcept {}

        const char * const FormatPNG:: CallSign = "PNG";

        namespace
        {




            class PNG_Common
            {
            public:
                inline virtual ~PNG_Common() noexcept {}


                inline void Failed(const char *fn) const
                {
                    assert(fn);
                    throw Specific::Exception(FormatPNG::CallSign,"%s failure!",fn);
                }


            protected:
                inline explicit PNG_Common() noexcept {}



            private:
                Y_DISABLE_COPY_AND_ASSIGN(PNG_Common);

            };


            class PNG_Reader : public PNG_Common, public InputFile
            {
            public:
                png_structp png;
                png_infop   info;

                explicit PNG_Reader(const String &filename) :
                PNG_Common(),
                InputFile(filename),
                png(NULL),
                info(NULL)
                {
                    png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
                    if(!png) Failed("png_create_read_struct");

                    info = png_create_info_struct(png);
                    if(!info)
                    {
                        png_destroy_read_struct(&png,NULL,NULL);
                        Failed("png_create_info_struct for reader");
                    }

                }

                virtual ~PNG_Reader() noexcept
                {
                    png_destroy_read_struct(&png, &info, NULL);
                }


                inline Codec::Image   call()
                {
                    png_init_io(png, (FILE *)handle );
                    png_read_info(png, info);

                    const png_uint_32 width      = png_get_image_width(png, info);
                    const png_uint_32 height     = png_get_image_height(png, info);
                    const png_byte    color_type = png_get_color_type(png, info);
                    const png_byte    bit_depth  = png_get_bit_depth(png, info);

                    // Read any color_type into 8bit depth, RGBA format.
                    // See http://www.libpng.org/pub/png/libpng-manual.txt

                    if(bit_depth == 16)
                        png_set_strip_16(png);

                    if(color_type == PNG_COLOR_TYPE_PALETTE)
                        png_set_palette_to_rgb(png);

                    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
                    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
                        png_set_expand_gray_1_2_4_to_8(png);

                    if(png_get_valid(png, info, PNG_INFO_tRNS))
                        png_set_tRNS_to_alpha(png);

                    // These color_type don't have an alpha channel then fill it with 0xff.
                    if(color_type == PNG_COLOR_TYPE_RGB ||
                       color_type == PNG_COLOR_TYPE_GRAY ||
                       color_type == PNG_COLOR_TYPE_PALETTE)
                        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

                    if(color_type == PNG_COLOR_TYPE_GRAY ||
                       color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
                        png_set_gray_to_rgb(png);

                    png_read_update_info(png, info);


                    const size_t bytes_per_row = png_get_rowbytes(png,info);
                    (void) bytes_per_row;
                    //std::cerr << "bytes_per_row=" << bytes_per_row << std::endl;

                    Codec::Image                    pxm(width,height);
                    CxxArray<png_bytep,MemoryModel> row(height);
                    for(size_t i=0,j=1;i<height;++i,++j)
                    {
                        row[j] = (png_byte*)&pxm[i][0];
                    }
                    png_read_image(png,&row[1]);

                    pxm.displayInfo("PNG");
                    return pxm;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(PNG_Reader);
            };


        }



        Codec::Image FormatPNG:: load(const String &filename, const Options *) const
        {
            PNG_Reader *io = new PNG_Reader(filename);
            try
            {
                if (setjmp(png_jmpbuf(io->png)))
                {
                    io->Failed("png_load");
                }

                Image res = io->call();
                delete io;
                return res;
            }
            catch(...)
            {
                delete io;
                throw;
            }

        }

        static inline bool getAlpha(const Options *opts)
        {
            static const char txt[] = "alpha";
            const String *opt = Options::Query(opts,txt);
            if(opt)
            {
                const String &alpha = *opt;
                if( StringToBoolean::MeansTrue(alpha.c_str())  ) return true;
                if( StringToBoolean::MeansFalse(alpha.c_str()) ) return false;
                throw Specific::Exception(FormatPNG::CallSign,"invalid option alpha='%s'", alpha());
            }
            return false;
        }

        static inline int getLevel(const Options *opts)
        {
            const char    txt[] = "level";
            int           res = 6;
            const String *opt = Options::Query(opts,txt);
            if(0!=opt)
            {
                res = Clamp<int>(1, ASCII::Convert::To<int>(*opt,txt),9);
            }
            return res;
        }



        void FormatPNG:: save(const Image &img, const String &filename, const Options *opts) const
        {
            OutputFile fp(filename);

            png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            if(!png)
            {
                throw Specific::Exception(CallSign,"png_create_write_struct");
            }


            png_infop info = png_create_info_struct(png);
            if(!info)
            {
                png_destroy_write_struct(&png, NULL);
                throw Specific::Exception(CallSign,"png_create_info_struct for writer");
            }


            if (setjmp(png_jmpbuf(png)))
            {
                png_destroy_write_struct(&png, &info);
                throw Specific::Exception(CallSign,"png_format::save('%s')",filename());
            }

            png_init_io(png, static_cast<FILE*>(fp.handle) );

            const bool alpha = getAlpha(opts);

            // Output is 8bit depth, RGB(A) format.
            png_set_IHDR(png,
                         info,
                         static_cast<png_uint_32>(img.w),
                         static_cast<png_uint_32>(img.h),
                         8,
                         alpha ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB,
                         PNG_INTERLACE_NONE,
                         PNG_COMPRESSION_TYPE_DEFAULT,
                         PNG_FILTER_TYPE_DEFAULT
                         );

            png_set_compression_level(png,getLevel(opts));

            png_write_info(png,info);

            // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
            // Use png_set_filler().
            if(!alpha)
                png_set_filler(png, 0, PNG_FILLER_AFTER);

            {
                const size_t                    height = static_cast<size_t>(img.h);
                CxxArray<png_bytep,MemoryModel> row(height);
                for(size_t i=0,j=1;i<height;++i,++j)
                {
                    row[j] = (png_byte*)&img(i)(0);
                }
                png_write_image(png, &row[1]);
            }
            png_write_end(png, NULL);

            png_destroy_write_struct(&png, &info);
        }


    }
}
