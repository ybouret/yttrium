

#include "y/ink/image/format/tiff.hpp"

namespace Yttrium
{
    namespace Ink
    {
        FormatTIFF::  FormatTIFF() : Format(CallSign,"(tiff|tif)&") {}
        FormatTIFF:: ~FormatTIFF() noexcept {}

        const char * const FormatTIFF:: CallSign = "TIFF";


    }
}


#include "y/ink/image/format/tiff++.hpp"
#include "y/system/exception.hpp"
#include "tiffio.h"

namespace Yttrium
{
    namespace Ink
    {

        size_t FormatTIFF:: CountDirectories(const String &filename)
        {
            itiff tif(filename);
            return tif.count_directories();
        }


        Codec::Image FormatTIFF:: load(const String &filename, const FormatOptions *) const
        {
            itiff tif(filename);
            const unit_t w = tif.width();  if(w<=0) throw Specific::Exception(CallSign,"w=0 in '%s'",filename());
            const unit_t h = tif.height(); if(h<=0) throw Specific::Exception(CallSign,"h=0 in '%s'",filename());
            Image img(w,h);
            if(!tif.load(img)) throw Specific::Exception(CallSign,"can't ReadRGBAImage '%s'",filename());
            return img;
        }

        class tiff_output : public otiff
        {
        public:
            inline explicit tiff_output(const String &fn) : otiff(fn) {}
            inline virtual ~tiff_output() noexcept {}

            inline TIFF * operator*() noexcept { assert(0!=handle); return static_cast<TIFF *>(handle); }
        private:
            Y_DISABLE_COPY_AND_ASSIGN(tiff_output);
        };

        void FormatTIFF:: save(const Image &img, const String &filename, const FormatOptions *) const
        {
            tiff_output tif(filename);
            int compression = COMPRESSION_NONE;
            TIFFSetField(*tif, TIFFTAG_IMAGEWIDTH,  (uint32_t) img.w);
            TIFFSetField(*tif, TIFFTAG_IMAGELENGTH, (uint32_t) img.h);
            TIFFSetField(*tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
            TIFFSetField(*tif, TIFFTAG_COMPRESSION, compression);
            TIFFSetField(*tif, TIFFTAG_SAMPLESPERPIXEL, 4);
            TIFFSetField(*tif, TIFFTAG_BITSPERSAMPLE,   8);
            TIFFSetField(*tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
            TIFFSetField(*tif, TIFFTAG_PHOTOMETRIC,  PHOTOMETRIC_RGB);

            for(unit_t j=0;j<img.h;++j)
            {
                if( TIFFWriteScanline(*tif,(void*)(&img(j)(0)),static_cast<uint32_t>(j),0) < 0 )
                {
                    throw Specific::Exception(CallSign,"Error in TiffWriteScanLine");
                }
            }

        }



    }

}
