#include "y/memory/object-locator.hpp"
#include "y/utest/run.hpp"
#include "y/object.hpp"

using namespace Yttrium;


Y_UTEST(memory_objloc)
{
    int i = 5;
    {
        const Memory::ObjectLocator meminfo(&i);
        std::cerr << meminfo << std::endl;
    }


    {
        void *small = Object:: operator new(48);
        const Memory::ObjectLocator meminfo(small);
        std::cerr << meminfo << std::endl;
        Object::operator delete(small,48);
    }

    {
        void *big = Object:: operator new(301);
        const Memory::ObjectLocator meminfo(big);
        std::cerr << meminfo << std::endl;
        Object:: operator delete(big,301);
    }

}
Y_UDONE()


