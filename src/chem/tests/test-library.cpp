


#include "y/chemical/library.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(library)
{
    Library lib;

    const Species &h = lib("H^+");
    const Species &w = lib("HO^-");

    std::cerr << h << std::endl;

    std::cerr << "lib=" << lib << std::endl;

}
Y_UDONE()
