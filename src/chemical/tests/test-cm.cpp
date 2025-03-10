

#include "y/chemical/library.hpp"
#include "y/chemical/reactive/components.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(cm)
{

    Library        lib;
    const Species &A = lib("A");
    const Species &B = lib("B");

    Components A2B("A2B");
    std::cerr << A2B << std::endl;
    A2B(Reactant,A);
    std::cerr << A2B << std::endl;
    A2B(Product,2,B);
    std::cerr << A2B << std::endl;



}
Y_UDONE()
