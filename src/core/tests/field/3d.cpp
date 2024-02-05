

#include "y/field/3d.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "../main.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;

Y_UTEST(field3d)
{
#if 0

    Random::Rand     ran;

    Field::Layout3D L( Field::Coord3D(-2,-4,-1), Field::Coord3D(3,5,7) );
    std::cerr << L << std::endl;

    Field::In2D<String,Memory::Dyadic> F("f2",L);
    Vector<String>      provided;

    std::cerr << "field=" << F.key() << std::endl;
    for(unit_t j=F.lower.y;j<=F.upper.y;++j)
    {
        std::cerr << "->" << F[j].key() << std::endl;
    }
#endif
    

}
Y_UDONE()

