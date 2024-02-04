#include "y/field/idb.hpp"


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
    Memory::Dyadic &mgr = Memory::Dyadic::Instance();

    Field::Layout1D L( -5, 6);
    std::cerr << L << std::endl;

    Field::IndexDataBase & idb = Field::IndexDataBase::Instance();

    {
        const String &z = idb[0];
        std::cerr << "z='" << z << "', quantity=" << z.quantity() << std::endl;
    }

    std::cerr << idb[-1] << idb[3] << std::endl;


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
    
    

}
Y_UDONE()

