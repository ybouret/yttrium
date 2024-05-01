

#include "y/color/conv.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


template <typename T> static inline
size_t GrayToIndx(const T &f)
{
    static const T half(0.5);
    return floor(f*765+half);
}

Y_UTEST(color_conv)
{

    

#if 0
    {
        size_t k=0;
        for(size_t i=0;i<16;++i)
        {
            for(size_t j=0;j<16;++j)
            {
                std::cerr << "U(" << std::setw(3) << k++ << "), ";
            }
            std::cerr << "\\" << std::endl;
        }
    }
#endif

    {
        std::cerr << "Testing Bytes..." << std::endl;
        size_t k=0;
        for(size_t i=0;i<16;++i)
        {
            for(size_t j=0;j<16;++j)
            {
                Y_ASSERT( k == Color::Conv<float>::ToByte(Color::Conv<float>::Unit[k]) );
                Y_ASSERT( k == Color::Conv<double>::ToByte(Color::Conv<double>::Unit[k]) );
                Y_ASSERT( k == Color::Conv<long double>::ToByte(Color::Conv<long double>::Unit[k]) );
                ++k;
            }
        }
    }

#if 0
    {
        for(size_t i=0;i<=765;++i)
        {
            std::cerr << "G(" << std::setw(3) << i << "), ";
            if( (i+1) % 16 == 0 )
                std::cerr << "\\" << std::endl;
        }

    }
#endif

    std::cerr << "Testing GrayScale..." << std::endl;
    for(size_t k=0;k<=255*3;++k)
    {
        Y_ASSERT( k == GrayToIndx( Color::Conv<float>::Gray[k]) );
        Y_ASSERT( k == GrayToIndx( Color::Conv<double>::Gray[k]) );
        Y_ASSERT( k == GrayToIndx( Color::Conv<long double>::Gray[k]) );
    }


}
Y_UDONE()

