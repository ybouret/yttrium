

#include "y/memory/solitary/workspace.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


Y_UTEST(memory_workspace)
{
    Memory::Workspace<String> s;
    Memory::Workspace<int>    i;

    std::cerr << "'" << s.build()        << "'" << std::endl;
    std::cerr << "'" << s.build("Hello") << "'"  << std::endl;
    std::cerr << i.build()  << std::endl;
    const int value = 3;
    std::cerr << i.build(value) << std::endl;

}
Y_UDONE()

