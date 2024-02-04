
#include "y/field/2d.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "../main.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;

Y_UTEST(field2d)
{

    Random::Rand     ran;

    Field::Layout2D L( Field::Coord2D(-2,-4), Field::Coord2D(3,5) );
    std::cerr << L << std::endl;

    Field::In2D<String,Memory::Dyadic> F("f2",L);
    Vector<String>      provided;

    std::cerr << "field=" << F.key() << std::endl;
    for(unit_t j=F.lower.y;j<=F.upper.y;++j)
    {
        std::cerr << "->" << F[j].key() << std::endl;
    }

#if 1
    for(unit_t j=F.lower.y;j<=F.upper.y;++j)
    {
        for(unit_t i=F.lower.x;i<=F.upper.x;++i)
        {
            const String s = Bring<String>::Get(ran);
            provided << s;
            F[j][i] = s;
        }
    }

    const LightArray<String> content( &F[F.lower.y][F.lower.x], F.items);
    std::cerr << content << std::endl;
    Y_CHECK(content==provided);
#endif
    
}
Y_UDONE()

