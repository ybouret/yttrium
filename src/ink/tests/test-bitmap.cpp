#include "y/ink/pixmap.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

namespace Yttrium
{

    namespace Ink
    {
       


    }

}

#include "y/string.hpp"

Y_UTEST(bitmap)
{
    
    {
        int                 pixel[] = { 1, 2, 3, 4 };
        const unit_t        count   = sizeof(pixel)/sizeof(pixel[0]);
        const Ink::ZeroFlux zf(count);
        Ink::BitRow         row_    = { pixel, &zf };
        Ink::PixRow<int>   &row     = Memory::OutOfReach::Conv< Ink::PixRow<int>, Ink::BitRow>(row_);

        std::cerr << " w = " << row.w() << std::endl;


    }

    Ink::Pixmap<String>         pix(100,23);
    const Ink::Pixmap<String> & cst = pix;

    for(unit_t j=0;j<pix.h;++j)
    {
        for(unit_t i=0;i<pix.w;++i)
        {
            pix[j][i] = "Hello";
        }
    }

    for(unit_t j=0;j<pix.h;++j)
    {
        for(unit_t i=0;i<pix.w;++i)
        {
            Y_ASSERT("Hello"==cst[j][i]);
        }
    }

}
Y_UDONE()
