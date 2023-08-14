#include "y/mkl/api.hpp"

// reals
namespace Yttrium
{

    namespace MKL
    {
        template <>
        float API:: Fabs<float>(const float &x)
        {
            return fabsf(x);
        }

        template <>
        double API:: Fabs<double>(const double &x)
        {
            return fabs(x);
        }

        template <>
        long double API:: Fabs<long double>(const long double &x)
        {
            return fabsl(x);
        }

        template <>
        float API:: Sqrt<float>(const float &x)
        {
            return sqrtf(x);
        }

        template <>
        double API:: Sqrt<double>(const double &x)
        {
            return sqrt(x);
        }

        template <>
        long double API:: Sqrt<long double>(const long double &x)
        {
            return sqrtl(x);
        }

    }

}

