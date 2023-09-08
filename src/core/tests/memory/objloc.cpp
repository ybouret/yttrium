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
        uint8_t *small = static_cast<uint8_t *> ( Object:: operator new(48) );
        const Memory::ObjectLocator meminfo(small);
        std::cerr << meminfo << std::endl;

        {
            const Memory::ObjectSentries sentries(small);
            for(size_t i=0;i<48;++i)
            {
                small[i] = 0xff;
            }
        }

        Object::operator delete(small,48);
    }

    {
        uint8_t  *big = static_cast<uint8_t *>(Object:: operator new(301));
        const Memory::ObjectLocator meminfo(big);
        std::cerr << meminfo << std::endl;

        {
            const Memory::ObjectSentries sentries(big);
            for(size_t i=0;i<sentries.width;++i)
            {
                big[i] = 0xff;
            }
            
        }
        Object:: operator delete(big,301);
    }

}
Y_UDONE()


