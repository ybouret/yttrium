
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


            //! (r,g,b,...) -> gray
            template <typename T, typename COLOR> static inline T Pack(const COLOR &c) noexcept
            {
                return Make<T>(c.r,c.g,c.b);
            }


            //! u -> (u,u,u,...)
            template <typename COLOR> static inline COLOR ByteTo(const uint8_t u) noexcept
            {
                return COLOR(u,u,u);
            }

            //! (r,b,g,...) -> (gray,gray,gray,...)
            template <typename COLOR> static inline COLOR Convert(const COLOR &c) noexcept
            {
                const uint8_t u = Pack<uint8_t,COLOR>(c);
                return ByteTo<COLOR>(u);
            }
        };

    }

}

#endif
