#include "y/object.hpp"
#include "y/memory/grains.hpp"

#include <cstring>

namespace Yttrium
{
    Object:: ~Object() noexcept
    {
    }

    Object:: Object() noexcept
    {
    }

    void * Object:: operator new(const size_t blockSize)
    {
        static Memory::Grains &grains = Memory::Grains::Instance();
        assert(blockSize>0);
        return grains.acquire(blockSize);
    }

    void Object:: operator delete(void *blockAddr, const size_t blockSize) noexcept
    {
        assert(blockSize>0);
        if(blockAddr)
        {
            static Memory::Grains &grains = Memory::Grains::Location();
            grains.release(blockAddr,blockSize);
        }
    }

    void * Object:: operator new [](const size_t blockSize)
    {
        static Memory::Grains &grains = Memory::Grains::Instance();
        assert(blockSize>0);
        return grains.acquire(blockSize);
    }

    void Object:: operator delete [](void *blockAddr, const size_t blockSize) noexcept
    {
        assert(blockSize>0);
        if(blockAddr)
        {
            static Memory::Grains &grains = Memory::Grains::Location();
            grains.release(blockAddr,blockSize);
        }
    }


    void * Object::operator new(size_t blockSize, void *addr) noexcept
    {
        assert(blockSize>0);
        assert(0!=addr);
        memset(addr,0,blockSize);
        return addr;
    }

    void Object:: operator delete(void *, void *) noexcept
    {}

}
