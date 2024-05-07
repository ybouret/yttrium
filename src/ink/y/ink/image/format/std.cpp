

#include "y/ink/image/codecs.hpp"
#include "y/ink/image/format/bmp.hpp"
#include "y/ink/image/format/jpeg.hpp"
#include "y/ink/image/format/png.hpp"
#include "y/ink/image/format/tiff.hpp"
#include "y/ink/image/format/pnm.hpp"




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
            use<FormatPNM>();
            return *this;
        }


        Codecs & Codecs:: Std()
        {
            Codecs &self = Instance();
            return self.std();
        }

    }

}


