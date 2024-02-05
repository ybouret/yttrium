

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
    
    Field::Layout1D L( -5, 6);
    std::cerr << L << std::endl;


#if 1
    Field::In1D<String,Memory::Dyadic> F("f1",L);
    Vector<String>                     provided;

    for(unit_t i=F.lower;i<=F.upper;++i)
    {
        const String s = Bring<String>::Get(ran);
        F[i] = s;
        provided << s;
    }

    F.display(std::cerr) << std::endl;
    const LightArray<String> content( &F[F.lower], F.items );
    std::cerr << content << std::endl;
    Y_CHECK(content==provided);
#endif

    Y_SIZEOF( Field::MetaKeyWith<0> );
    Y_SIZEOF( Field::MetaKeyWith<1> );
    Y_SIZEOF( Field::MetaKeyWith<2> );
    Y_SIZEOF( Field::MetaKeyWith<3> );
    Y_SIZEOF( Field::Layout1D       );
    Y_SIZEOF( Field::MemoryBuilder<String> );
    Y_SIZEOF( Field::Interface  );
    Y_SIZEOF( AutoPtr<String>   );

    std::cerr << sizeof(Field::Sub1D<0,String>) << std::endl;




}
Y_UDONE()

