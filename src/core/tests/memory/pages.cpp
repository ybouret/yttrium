
#include "y/memory/pages.hpp"
#include "y/utest/run.hpp"
#include "../alea.hpp"
#include "y/lockable.hpp"

using namespace Yttrium;


Y_UTEST(memory_pages)
{
    alea_seed();

    std::cerr << "Memory::Pages::MinBytes = " << Memory::Pages::MinBytes << std::endl;
    std::cerr << "Memory::Pages::MaxBytes = " << Memory::Pages::MaxBytes << std::endl;
    std::cerr << "Memory::Pages::Number   = " << Memory::Pages::Number   << std::endl;
    Y_SIZEOF(Memory::Pages);

    Memory::Pages pages(10, Lockable::Giant() );
    
    std::cerr << "GetPageSize = " << Memory::Page::QueryOsBytes() << std::endl;
    std::cerr << "MyPageSize  = " << pages.bytes << std::endl;
    
    pages.reserve(5);

}
Y_UDONE()
