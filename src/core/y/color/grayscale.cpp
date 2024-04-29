#include "y/color/grayscale.hpp"
#include "y/color/conv.hpp"

namespace Yttrium
{
    namespace Color
    {
#define Y_COLOR_GS(r,g,b) (unsigned(r)+unsigned(g)+unsigned(b))

        template <> float GrayScale::Make<float>(const uint8_t r, const uint8_t g, const uint8_t b) noexcept
        {
            return Conv<float>::Gray[Y_COLOR_GS(r,g,b) ];
        }

        template <> double GrayScale::Make<double>(const uint8_t r, const uint8_t g, const uint8_t b) noexcept
        {
            return Conv<double>::Gray[Y_COLOR_GS(r,g,b)];
        }

        template <> long double GrayScale::Make<long double>(const uint8_t r, const uint8_t g, const uint8_t b) noexcept
        {
            return Conv<long double>::Gray[Y_COLOR_GS(r,g,b) ];
        }

        template <> uint8_t GrayScale::Make<uint8_t>(const uint8_t r, const uint8_t g, const uint8_t b) noexcept
        {
            return Conv8::Gray[Y_COLOR_GS(r,g,b)];
        }
        


    }

}

