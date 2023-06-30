
#include "y/memory/pages.hpp"
#include "y/utest/run.hpp"
#include "../alea.hpp"


using namespace Yttrium;


Y_UTEST(memory_pages)
{
    alea_seed();

    std::cerr << "Memory::Pages::MinBytes = " << Memory::Pages::MinBytes << std::endl;
    std::cerr << "Memory::Pages::MaxBytes = " << Memory::Pages::MaxBytes << std::endl;

    Memory::Pages pages(10);
    
    std::cerr << "GetPageSize = " << Memory::Page::QueryOsBytes() << std::endl;
    std::cerr << "used        = " << pages.bytes << std::endl;
    
    Y_SIZEOF(Memory::Pages);

    pages.reserve(5);

}
Y_UDONE()
