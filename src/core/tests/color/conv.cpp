

#include "y/color/conv.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

#if 0

namespace Yttrium
{
    namespace Color
    {





    }
}
#endif


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
            std::cerr << "\\" << "std::endl;
        }
    }
#endif

    {
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



}
Y_UDONE()

