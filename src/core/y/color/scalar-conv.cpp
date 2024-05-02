
#include "y/color/scalar-conv.hpp"
#include "y/color/conv.hpp"

namespace Yttrium
{
    namespace Color
    {

        template <>
        float ScalarConv<float,uint8_t>::From(const uint8_t x) noexcept
        {
            return Conv<float>::Unit[x];
        }

        template <>
        double ScalarConv<double,uint8_t>::From(const uint8_t x) noexcept
        {
            return Conv<double>::Unit[x];
        }

        template <>
        long double ScalarConv<long double,uint8_t>::From(const uint8_t x) noexcept
        {
            return Conv<long double>::Unit[x];
        }

        template <> uint8_t ScalarConv<uint8_t,float>:: From(const float x) noexcept
        {
            return Conv<float>::ToByte(x);
        }

        template <> uint8_t ScalarConv<uint8_t,double>:: From(const double x) noexcept
        {
            return Conv<double>::ToByte(x);
        }

        template <> uint8_t ScalarConv<uint8_t,long double>:: From(const long double x) noexcept
        {
            return Conv<long double>::ToByte(x);
        }

    }

}
