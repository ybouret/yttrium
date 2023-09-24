#include "y/fft/fft.hpp"

namespace Yttrium
{

    template <> const double      FFT:: Table<double>::      PI = 3.141592653589793238462643383279502884197;
    template <> const long double FFT:: Table<long double>:: PI = 3.141592653589793238462643383279502884197l;

    static const uint64_t _1 = 1;

#define Y_SIN(i) sin(PI/(_1<<i))

#define Y_SIN_TABLE(FIRST,SECOND) \
FIRST,  SECOND,  Y_SIN(2),  Y_SIN(3),  Y_SIN(4),  Y_SIN(5),  Y_SIN(6),  Y_SIN(7),  Y_SIN(8),  Y_SIN(9), \
Y_SIN(10), Y_SIN(11), Y_SIN(12), Y_SIN(13), Y_SIN(14), Y_SIN(15), Y_SIN(16), Y_SIN(17), Y_SIN(18), Y_SIN(19),\
Y_SIN(20), Y_SIN(21), Y_SIN(22), Y_SIN(23), Y_SIN(24), Y_SIN(25), Y_SIN(26), Y_SIN(27), Y_SIN(28), Y_SIN(29),\
Y_SIN(30), Y_SIN(31), Y_SIN(32), Y_SIN(33), Y_SIN(34), Y_SIN(35), Y_SIN(36), Y_SIN(37), Y_SIN(38), Y_SIN(39),\
Y_SIN(40), Y_SIN(41), Y_SIN(42), Y_SIN(43), Y_SIN(44), Y_SIN(45), Y_SIN(46), Y_SIN(47), Y_SIN(48), Y_SIN(49),\
Y_SIN(50), Y_SIN(51), Y_SIN(52), Y_SIN(53), Y_SIN(54), Y_SIN(55), Y_SIN(56), Y_SIN(57), Y_SIN(58), Y_SIN(59),\
Y_SIN(60), Y_SIN(61), Y_SIN(62), Y_SIN(63)


    template <>
    const double FFT:: Table<double> :: PositiveSin[64] =
    {
        Y_SIN_TABLE(0,1)
    };

#undef Y_SIN
#define Y_SIN(i) sin(-PI/(_1<<i))

    template <>
    const double FFT:: Table<double> :: NegativeSin[64] =
    {
        Y_SIN_TABLE(0,-1)
    };


#undef Y_SIN
#define Y_SIN(i) sinl(PI/(_1<<i))

    template <>
    const long double FFT:: Table<long double> :: PositiveSin[64] =
    {
        Y_SIN_TABLE(0,1)
    };

#undef  Y_SIN
#define Y_SIN(i) sinl(-PI/(_1<<i))

    template <>
    const long double FFT:: Table<long double> :: NegativeSin[64] =
    {
        Y_SIN_TABLE(0,-1)
    };

}

