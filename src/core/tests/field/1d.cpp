#include "y/field/1d.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(field1d)
{

    {
        Operating<int> op(0,0);
    }

    Memory::Dyadic &mgr = Memory::Dyadic::Instance();
    Field1D<int> F(-5,6,mgr);

    std::cerr << F.lower << "->" << F.upper << " : " << F.width << std::endl;
    

}
Y_UDONE()

