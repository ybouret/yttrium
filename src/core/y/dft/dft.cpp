#include "y/dft/dft.hpp"
#include "y/mkl/numeric.hpp"

namespace Yttrium
{

#define Y_DFT_REP(FIRST) \
FIRST,       Y_DFT(0x01), Y_DFT(0x02), Y_DFT(0x03), Y_DFT(0x04), Y_DFT(0x05), Y_DFT(0x06), Y_DFT(0x07), \
Y_DFT(0x08), Y_DFT(0x09), Y_DFT(0x0a), Y_DFT(0x0b), Y_DFT(0x0c), Y_DFT(0x0d), Y_DFT(0x0e), Y_DFT(0x0f), \
Y_DFT(0x10), Y_DFT(0x11), Y_DFT(0x12), Y_DFT(0x13), Y_DFT(0x14), Y_DFT(0x15), Y_DFT(0x16), Y_DFT(0x17), \
Y_DFT(0x18), Y_DFT(0x19), Y_DFT(0x1a), Y_DFT(0x1b), Y_DFT(0x1c), Y_DFT(0x1d), Y_DFT(0x1e), Y_DFT(0x1f), \
Y_DFT(0x20), Y_DFT(0x21), Y_DFT(0x22), Y_DFT(0x23), Y_DFT(0x24), Y_DFT(0x25), Y_DFT(0x26), Y_DFT(0x27), \
Y_DFT(0x28), Y_DFT(0x29), Y_DFT(0x2a), Y_DFT(0x2b), Y_DFT(0x2c), Y_DFT(0x2d), Y_DFT(0x2e), Y_DFT(0x2f), \
Y_DFT(0x30), Y_DFT(0x31), Y_DFT(0x32), Y_DFT(0x33), Y_DFT(0x34), Y_DFT(0x35), Y_DFT(0x36), Y_DFT(0x37), \
Y_DFT(0x38), Y_DFT(0x39), Y_DFT(0x3a), Y_DFT(0x3b), Y_DFT(0x3c), Y_DFT(0x3d), Y_DFT(0x3e), Y_DFT(0x3f)



#define Y_DFT(i) sin( MKL::Numeric<double>::PI / static_cast<double>( uint64_t(1) << i ) )

    template <>
    const double DFT::Table<double>::PositiveSin[64] =
    {
        Y_DFT_REP(0.0)
    };

#undef Y_DFT

#define Y_DFT(i) -sin( MKL::Numeric<double>::PI / static_cast<double>( uint64_t(1) << i ) )

    template <>
    const double DFT::Table<double>::NegativeSin[64] =
    {
        Y_DFT_REP(0.0)
    };


#undef Y_DFT

#define Y_DFT(i) cos( MKL::Numeric<double>::PI / static_cast<double>( uint64_t(1) << i ) ) - 1.0

    template <>
    const double DFT::Table<double>::CosMinusOne[64] =
    {
        Y_DFT_REP(-2.0)
    };


#undef Y_DFT

#define Y_DFT(i) sinl( MKL::Numeric<long double>::PI / static_cast<long double>( uint64_t(1) << i ) )

    template <>
    const long double DFT::Table<long double>::PositiveSin[64] =
    {
        Y_DFT_REP(0.0L)
    };

#undef Y_DFT

#define Y_DFT(i) -sinl( MKL::Numeric<long double>::PI / static_cast<long double>( uint64_t(1) << i ) )

    template <>
    const long double DFT::Table<long double>::NegativeSin[64] =
    {
        Y_DFT_REP(0.0L)
    };

#undef Y_DFT

#define Y_DFT(i) cosl( MKL::Numeric<long double>::PI / static_cast<long double>( uint64_t(1) << i ) ) - 1.0L

    template <>
    const long double DFT::Table<long double>::CosMinusOne[64] =
    {
        Y_DFT_REP(-2.0L)
    };


}
