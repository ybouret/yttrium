
#include "y/chemical/library.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(lib)
{

    Library        lib;
    const Species &h = lib("H^+");

    std::cerr << "h='" << h << "'" << std::endl;



}
Y_UDONE()
