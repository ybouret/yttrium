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
    int32_t                 pixel[] = { 1, 2, 3, 4, 5, 6};

    {
        const unit_t        count   = sizeof(pixel)/sizeof(pixel[0]);
        const Ink::ZeroFlux zf(count);
        Ink::BitRow         row_    = { pixel, &zf };
        Ink::PixRow<int>   &row     = Memory::OutOfReach::Conv< Ink::PixRow<int>, Ink::BitRow>(row_);

        std::cerr << "row=" << row << std::endl;
    }

    {
        Ink::Pixmap<String>         pix(4,3);
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
        std::cerr << pix << std::endl;
    }

    {
        Ink::Pixmap<int32_t> pix(pixel,2,2,3);
        std::cerr << "ipix=" << pix << std::endl;
        Ink::Pixmap<uint32_t> upix(Ink::FromBitmap,pix);
        std::cerr << "upix=" << upix << std::endl;
        Ink::Pixmap<float> fpix(Ink::FromBitmap,pix);
        std::cerr << "fpix=" << fpix << std::endl;
    }

}
Y_UDONE()
