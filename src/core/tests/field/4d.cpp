


#include "y/field/4d.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "../main.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;

Y_UTEST(field4d)
{
    Random::Rand          ran;
    const Field::Format4D L4 = new Field::Layout4D( Field::Coord4D(0,0,0,0), Field::Coord4D(5,6,7,2) );
    std::cerr << "L4=" << L4 << std::endl;

    Field::In4D<String,Memory::Dyadic> F("f4",L4);
    Vector<String>                     provided;


    for(unit_t l=F->lower.w;l<=F->upper.w;++l)
    {
        std::cerr << F[l].key() << std::endl;
        for(unit_t k=F->lower.z;k<=F->upper.z;++k)
        {
            std::cerr << "-> " << F[l][k].key() << std::endl;
            for(unit_t j=F->lower.y;j<=F->upper.y;++j)
            {
                std::cerr << "---> " << F[l][k][j].key() << std::endl;
                for(unit_t i=F->lower.x;i<=F->upper.x;++i)
                {
                    const String s = Bring<String>::Get(ran);
                    provided << s;
                    F[l][k][j][i]= s;
                }
            }
        }
    }

    std::cerr << F.key() << ": ram=" << F.ram() << std::endl;
    const LightArray<String> content( &F[F->lower.w][F->lower.z][F->lower.y][F->lower.x], F->items);
    std::cerr << content << std::endl;
    Y_CHECK(content==provided);

}
Y_UDONE()

