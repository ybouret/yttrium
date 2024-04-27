

#include "y/ink/image/codecs.hpp"
#include "y/ink/image/format/bmp.hpp"
#include "y/ink/image/format/jpeg.hpp"
#include "y/ink/image/format/png.hpp"
#include "y/ink/image/format/tiff.hpp"




namespace Yttrium
{
    namespace Ink
    {

        Codecs & Codecs:: std()
        {
            Y_LOCK(access);
            use<FormatBMP>();
            use<FormatPNG>();
            use<FormatJPEG>();
            use<FormatTIFF>();
            return *this;
        }


        Codecs & Codecs:: Std()
        {
            Codecs &self = Instance();
            return self.std();
        }

    }

}


