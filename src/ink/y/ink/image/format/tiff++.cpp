#include "y/ink/image/format/tiff++.hpp"
#include "y/exception.hpp"
#include "tiffio.h"

namespace Yttrium
{
    namespace Ink
    {


        tiffxx:: ~tiffxx() noexcept
        {
            assert(handle);
            TIFFClose( static_cast<TIFF *>(handle) );
            Coerce(handle) = 0;
        }

        tiffxx:: tiffxx(const String &filename, const char *mode) :
        handle( TIFFOpen( filename(), mode ) )
        {
            if(!handle) throw Exception("TIFFOpen(%s,%s)", filename(), mode);
        }



        itiff:: itiff(const String &filename) : tiffxx(filename,"r")
        {
        }

        itiff:: ~itiff() noexcept
        {
        }

        unit_t itiff:: width()
        {
            uint32_t     w=0;
            TIFFGetField(static_cast<TIFF *>(handle), TIFFTAG_IMAGEWIDTH,  &w);
            return w;
        }

        unit_t itiff:: height()
        {
            uint32_t     h=0;
            TIFFGetField(static_cast<TIFF *>(handle), TIFFTAG_IMAGELENGTH,  &h);
            return h;
        }



        unit_t itiff:: width_of(const String &filename)
        {
            itiff fp(filename);
            return fp.width();
        }


        unit_t itiff:: height_of(const String &filename)
        {
            itiff fp(filename);
            return fp.height();
        }

        size_t itiff:: directories_of(const String &filename)
        {
            itiff fp(filename);
            return fp.count_directories();
        }

        unit_t itiff:: width_of(const char *filename)
        {
            const String _(filename); return width_of(_);
        }

        unit_t itiff:: height_of(const char *filename)
        {
            const String _(filename); return height_of(_);
        }

        size_t itiff:: directories_of(const char *filename)
        {
            const String _(filename); return directories_of(_);
        }

        size_t itiff:: count_directories()
        {
            TIFF      *tif = static_cast<TIFF *>(handle);
            size_t     nd = 0;
            do {
                ++nd;
            } while( TIFFReadDirectory(tif) );
            return nd;
        }

        static inline
        void expand(Pixmap<RGBA> &img) noexcept
        {
            const unit_t h   = img.h;
            const unit_t w   = img.w;
            unit_t       jlo = 0;
            unit_t       jhi = h;

            for(unit_t count=h/2;count>0;--count)
            {
                PixRow<RGBA> &rlo = img[jlo++];
                PixRow<RGBA> &rhi = img[--jhi];
                RGBA *lo          = &rlo[0];
                RGBA *hi          = &rhi[0];
                for(size_t i=w;i>0;--i)
                {
                    const uint32_t lo32 = *(uint32_t *)lo;
                    const uint32_t hi32 = *(uint32_t *)hi;
                    *(lo++) = RGBA( TIFFGetR(hi32), TIFFGetG(hi32), TIFFGetB(hi32), TIFFGetA(hi32));
                    *(hi++) = RGBA( TIFFGetR(lo32), TIFFGetG(lo32), TIFFGetB(lo32), TIFFGetA(lo32));
                }
            }
        }

        bool itiff:: load(Pixmap<RGBA> &img)
        {
            //uint32_t    *raster =  coerce_cast<uint32_t>( &img(0)(0) );
            uint32_t    *raster = Memory::OutOfReach::Cast<uint32_t,RGBA>( &img[0][0] );
            if( 1 == TIFFReadRGBAImage(static_cast<TIFF *>(handle), static_cast<uint32_t>(img.w), static_cast<uint32_t>(img.h), raster, 0))
            {
                expand(img);
                return true;
            }
            else
            {
                return false;
            }
        }

        void itiff:: set_directory(const size_t n)
        {
            if( 1 != TIFFSetDirectory(static_cast<TIFF *>(handle),static_cast<tdir_t>(n) ))
            {
                throw Exception("TIFFSetDirectory(n=%u) failure", unsigned(n));
            }
        }


        otiff:: otiff(const String &filename) : tiffxx(filename,"w")
        {
        }

        otiff:: ~otiff() noexcept
        {
        }


    }


}
