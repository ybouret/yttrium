

#include "y/field/3d.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "../main.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;

Y_UTEST(field3d)
{

    Random::Rand     ran;

    Field::Format3D L = new Field::Layout3D( Field::Coord3D(-2,-4,-1), Field::Coord3D(3,5,7) );
    std::cerr << L << std::endl;

    Field::In3D<String,Memory::Dyadic> F("f3",L);
    Vector<String>                    provided;

    std::cerr << "ram: " << F.ram() << std::endl;

    std::cerr << "field=" << F.key() << std::endl;

    for(unit_t k=F->lower.z;k<=F->upper.z;++k)
    {
        std::cerr << "->" << F[k].key();
        for(unit_t j=F->lower.y;j<=F->upper.y;++j)
        {
            std::cerr << "/" << F[k][j].key();
            for(unit_t i=F->lower.x;i<=F->upper.x;++i)
            {
                const String s = Bring<String>::Get(ran);
                provided << s;
                F[k][j][i]= s;
            }
        }
        std::cerr << std::endl;
    }

    const LightArray<String> content( &F[F->lower.z][F->lower.y][F->lower.x], F->items);
    std::cerr << content << std::endl;
    Y_CHECK(content==provided);


}
Y_UDONE()

