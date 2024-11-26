


#include "y/chemical/library.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(library)
{
    Library lib;

    const Species &h = lib("H^+");
    const Species &w = lib("HO^-");

    std::cerr << "h=" << h << std::endl;
    std::cerr << "w=" << w << std::endl;

    std::cerr << "lib=" << lib << std::endl;

}
Y_UDONE()
