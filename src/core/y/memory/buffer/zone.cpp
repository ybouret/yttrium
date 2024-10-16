#include "y/memory/buffer/zone.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Memory
    {

        Zone:: Zone(const void *_addr, const size_t _size) noexcept :
        addr(_addr),
        size(_size)
        {
            assert( Good(addr,size) );
        }

        Zone:: ~Zone() noexcept
        {
            assert( Good(addr,size) );
            Coerce(addr) = 0;
            Coerce(size) = 0;
        }

        Zone:: Zone(const char * const text) noexcept :
        addr(text),
        size(text?strlen(text):0)
        {

        }


        const void * Zone:: ro_addr() const noexcept { return addr; }
        size_t       Zone:: measure() const noexcept { return size; }
    }

}

