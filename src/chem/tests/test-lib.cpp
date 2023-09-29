
#include "y/chem/species/library.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;



Y_UTEST(lib)
{
    Chemical::Library lib;

    std::cerr << lib << std::endl;

    const Chemical::Species & proton = lib("H+",1);
    std::cerr << proton << std::endl;
    std::cerr << lib << std::endl;
    const Chemical::Species & hydroxy = lib("HO-",-1);
    std::cerr << hydroxy << std::endl;
    std::cerr << lib << std::endl;
}
Y_UDONE()
