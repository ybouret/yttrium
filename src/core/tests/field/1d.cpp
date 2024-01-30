#include "y/field/1d.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;

Y_UTEST(field1d)
{

    

    Memory::Dyadic &mgr = Memory::Dyadic::Instance();

    Field::Layout1D L( -5, 6);
    std::cerr << L << std::endl;

    Field::In1D<String> F(L,mgr);



}
Y_UDONE()

