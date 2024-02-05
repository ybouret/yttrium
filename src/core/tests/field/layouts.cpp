#include "y/field/layout/3d.hpp"
#include "y/field/layout/2d.hpp"
#include "y/field/layout/1d.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(field_layouts)
{
    Field::Layout3D L3( Field::Coord3D(0,0,0), Field::Coord3D(5,6,7) );
    std::cerr << "L3 = " << L3 << std::endl;

    Field::Layout2D L2( Field::SubLayout, L3);
    std::cerr << "L2 = " << L2 << std::endl;

    Field::Layout1D L1( Field::SubLayout, L2);
    std::cerr << "L1 = " << L1 << std::endl;

    Field::Layout1D L1b( Field::SubLayout, L3);
    std::cerr << "L1b= " << L1b << std::endl;

    Y_CHECK(L1b == L1);

    Y_SIZEOF(Field::Layout1D);
    Y_SIZEOF(Field::Layout2D);
    Y_SIZEOF(Field::Layout3D);

}
Y_UDONE()
