
//! \file

#ifndef Y_Ink_Gradient_Pixel_Force_Included
#define Y_Ink_Gradient_Pixel_Force_Included 1


namespace Yttrium
{

    namespace Ink
    {
        //______________________________________________________________________
        //
        //
        //
        //! Pixel power from dual threshold
        //
        //
        //______________________________________________________________________
        enum PixelForce
        {
            StrongPixel = 255, //!< well defined
            FeeblePixel = 127, //!< ill  defined
            NaughtPixel = 0    //!< vanishing
        };

    }

}

#endif
