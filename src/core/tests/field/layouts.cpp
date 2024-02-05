#include "y/field/layout/4d.hpp"
#include "y/field/layout/3d.hpp"
#include "y/field/layout/2d.hpp"
#include "y/field/layout/1d.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(field_layouts)
{

    Field::Layout4D  L4( Field::Coord4D(0,0,0,0), Field::Coord4D(5,6,7,8) );
    std::cerr << "L4 = " << L4 << std::endl;

    Field::Layout3D  L3( Field::SubLayout, L4);
    std::cerr << "L3 = " << L3 << std::endl;

    Field::Layout2D  L2a( Field::SubLayout, L4);
    Field::Layout2D  L2b( Field::SubLayout, L3);
    std::cerr << "L2a = " << L2a << std::endl;
    std::cerr << "L2b = " << L2b << std::endl;
    Y_CHECK(L2a==L2b);

    Field::Layout2D  L1a( Field::SubLayout, L4);
    Field::Layout2D  L1b( Field::SubLayout, L3);
    Field::Layout2D  L1c( Field::SubLayout, L2a);
    std::cerr << "L1a = " << L1a << std::endl;
    std::cerr << "L1b = " << L1b << std::endl;
    std::cerr << "L1c = " << L1c << std::endl;
    Y_CHECK(L1a==L1b);
    Y_CHECK(L1a==L1c);

    
    Y_SIZEOF(Field::Layout1D);
    Y_SIZEOF(Field::Layout2D);
    Y_SIZEOF(Field::Layout3D);
    Y_SIZEOF(Field::Layout4D);

}
Y_UDONE()
