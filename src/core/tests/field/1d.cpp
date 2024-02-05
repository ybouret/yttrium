

#include "y/field/1d.hpp"

#include "y/memory/allocator/dyadic.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "../main.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;

Y_UTEST(field1d)
{

    Random::Rand   ran;
    
    Field::Format1D L = new Field::Layout1D( -5, 6);
    std::cerr << L << std::endl;


    Field::In1D<String,Memory::Dyadic> F("f1",L);
    Vector<String>                     provided;

    std::cerr << "ram: " << F.ram() << std::endl;

    for(unit_t i=F->lower;i<=F->upper;++i)
    {
        const String s = Bring<String>::Get(ran);
        F[i] = s;
        provided << s;
    }

    F.display(std::cerr) << std::endl;
    const LightArray<String> content( &F[F->lower], F->items );
    std::cerr << content << std::endl;
    Y_CHECK(content==provided);



    Y_SIZEOF( Field::Layout1D );
    Y_SIZEOF( Field::MemoryBuilder<String> );

    std::cerr << sizeof(Field::Sub1D<0,String>) << std::endl;
    std::cerr << sizeof(Field::Sub1D<1,String>) << std::endl;
    std::cerr << sizeof(Field::Sub1D<2,String>) << std::endl;
    std::cerr << sizeof(Field::Sub1D<3,String>) << std::endl;




}
Y_UDONE()

