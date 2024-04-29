
//! \file

#ifndef Y_Color_GrayScale_Included
#define Y_Color_GrayScale_Included 1

#include "y/color/rgba.hpp"

namespace Yttrium
{
    namespace Color
    {
        //! helper for gray scales
        struct GrayScale
        {
            //! r,g,b -> gray
            template <typename T>     static inline T     Make(const uint8_t r, const uint8_t g, const uint8_t b) noexcept;
           
            //! r,b,g -> gray,gray,gray
            template <typename COLOR> static inline COLOR Make(const COLOR &c) noexcept
            {
                const uint8_t u = Make<uint8_t>(c.r,c.g,c.b);
                return COLOR(u,u,u);
            }


        };

    }

}

#endif
