#include "y/memory/buffer/zone.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


Y_UTEST(memory_zone)
{

    const char   message[] = "Hello, World!";
    {
        Memory::Zone zone(message,sizeof(message));
        std::cerr << "crc=" << zone.crc32() << std::endl;
        std::cerr << (const char *)zone.ro_addr() << std::endl;
        zone.displayHexadecimal(std::cerr) << std::endl;
    }

    {
        Memory::Zone zone(message);
        zone.displayHexadecimal(std::cerr) << std::endl;
        std::cerr << "crc=" << zone.crc32() << std::endl;

    }
}
Y_UDONE()

