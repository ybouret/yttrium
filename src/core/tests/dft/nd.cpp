
#include "y/dft/dftn.hpp"
#include "y/utest/run.hpp"
#include "y/field/2d.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/system/rtti.hpp"

using namespace Yttrium;


namespace  {

    template <typename T>
    static inline
    void Test2D()
    {
        std::cerr << "Testing with " << RTTI::Name<T>() << std::endl;
        for(unit_t nx=1;nx<=16;nx*=2)
        {
            for(unit_t ny=1;ny<=16;ny*=2)
            {
                Field::Format2D  L = new Field::Layout2D( Field::Coord2D(1,1), Field::Coord2D(nx,ny) );

                typedef Field::In2D< Complex<T>, Memory::Dyadic > F2D;
                F2D F("F",L);

                std::cerr << F << std::endl;
                for(unit_t j=F->lower.y;j<=F->upper.y;++j)
                {
                    //std::cerr << "j=" << j << std::endl;
                    for(unit_t i=F->lower.x;i<=F->upper.x;++i)
                    {
                        F[j][i].re = i;
                        F[j][i].im = j;
                    }
                    //Core::Display( std::cerr, &F[j][1].re, 2*F->width.x ) << std::endl;
                }

                const size_t  nn[] = { 0, F->width.y, F->width.x };
                T * const data = (&F[1][1].re) - 1;
                DFTN::Transform(data, nn, 2, 1);
                DFTN::Transform(data, nn, 2, -1);

                //std::cerr << std::endl;
                static const T half = 0.5;
                for(unit_t j=F->lower.y;j<=F->upper.y;++j)
                {
                    //std::cerr << "j=" << j << std::endl;
                    for(unit_t i=F->lower.x;i<=F->upper.x;++i)
                    {
                        F[j][i] /= F->shift.y;
                        const unit_t re = (unit_t)( floor(F[j][i].re+half) );
                        const unit_t im = (unit_t)( floor(F[j][i].im+half) );
                        Y_ASSERT(re==i);
                        Y_ASSERT(im==j);
                    }
                    //Core::Display( std::cerr, &F[j][1].re, F->width.x*2 ) << std::endl;
                }
            }
        }
        std::cerr << std::endl;
    }

}

Y_UTEST(dft_nd)
{
    Test2D<float>();
    Test2D<double>();
    Test2D<long double>();


}
Y_UDONE()

