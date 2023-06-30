
#include "y/memory/pages.hpp"
#include "y/utest/run.hpp"
#include "../alea.hpp"


using namespace Yttrium;


Y_UTEST(memory_pages)
{
    alea_seed();

    Memory::Pages pages(10);
    
    std::cerr << "PageSize=" << Memory::Page::QueryOsBytes() << std::endl;
    std::cerr << "used: " << pages.bytes << std::endl;
    

}
Y_UDONE()
