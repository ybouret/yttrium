
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
            template <typename T>     static T              Make(const uint8_t r, const uint8_t g, const uint8_t b) noexcept;
            template <typename COLOR> static inline uint8_t Make(const COLOR &c) noexcept
            {
                return Make<uint8_t>(c.r,c.g,c.b);
            }


            //! r,b,g -> gray,gray,gray
            template <typename COLOR> static inline COLOR MakeColor(const COLOR &c) noexcept
            {
                const uint8_t u = Make<COLOR>(c);
                return COLOR(u,u,u);
            }


        };

    }

}

#endif
