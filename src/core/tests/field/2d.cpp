
#include "y/field/2d.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;

Y_UTEST(field2d)
{

    
    Memory::Dyadic &mgr = Memory::Dyadic::Instance();

    Field::Layout2D L( Field::Coord2D(-2,-4), Field::Coord2D(3,5) );
    std::cerr << L << std::endl;

    Field::In2D<int> F(L,mgr);
    
}
Y_UDONE()

