
#include "y/dft/dftn.hpp"
#include "y/utest/run.hpp"
#include "y/field/2d.hpp"
#include "y/memory/allocator/dyadic.hpp"

using namespace Yttrium;

Y_UTEST(dft_nd)
{
    //Field::In2D<float,Memory::> field;
    Field::Format2D  L = new Field::Layout2D( Field::Coord2D(1,1), Field::Coord2D(4,8) );

    typedef Field::In2D< Complex<float>, Memory::Dyadic > F2D;
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
        Core::Display( std::cerr, &F[j][1].re, 2*F->width.x ) << std::endl;
    }

    const size_t  nn[] = { 0, F->width.y, F->width.x };
    float * const data = (&F[1][1].re) - 1;
    DFTN::fourn(data, nn, 2, 1);
    DFTN::fourn( (&F[1][1].re) - 1, nn, 2, -1);

    std::cerr << std::endl;
    for(unit_t j=F->lower.y;j<=F->upper.y;++j)
    {
        //std::cerr << "j=" << j << std::endl;
        for(unit_t i=F->lower.x;i<=F->upper.x;++i)
        {
            F[j][i] /= F->shift.y;
        }
        Core::Display( std::cerr, &F[j][1].re, F->width.x*2 ) << std::endl;
    }

    std::cerr << "width=" << F->width << std::endl;
    std::cerr << "shift=" << F->shift << std::endl;


}
Y_UDONE()

