#include "y/color/grayscale.hpp"
#include "y/color/conv.hpp"

namespace Yttrium
{
    namespace Color
    {

        template <> float GrayScale::Make<float>(const uint8_t r, const uint8_t g, const uint8_t b) noexcept
        {
            //std::cerr << "(" << int(r) << "," << int(g) << "," << int(b) << ")/";// << std::endl;
            return Conv<float>::Unit[ Conv8::From(r,g,b) ];
        }

        template <> double GrayScale::Make<double>(const uint8_t r, const uint8_t g, const uint8_t b) noexcept
        {
            return Conv<double>::Unit[ Conv8::From(r,g,b) ];
        }

        template <> long double GrayScale::Make<long double>(const uint8_t r, const uint8_t g, const uint8_t b) noexcept
        {
            return Conv<long double>::Unit[ Conv8::From(r,g,b) ];
        }

        template <> uint8_t GrayScale::Make<uint8_t>(const uint8_t r, const uint8_t g, const uint8_t b) noexcept
        {
            return Conv8::From(r,g,b);
        }
        


    }

}

