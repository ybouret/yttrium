#include "y/oversized.hpp"
#include "y/memory/allocator/pooled.hpp"

#include <cstring>

namespace Yttrium
{
    Oversized::  Oversized() noexcept {}
    Oversized:: ~Oversized() noexcept {}


    void * Oversized:: operator new(const size_t blockSize)
    {
        static Memory::Pooled &mgr = Memory::Pooled::Instance();
        assert(blockSize>0);
        return mgr.acquireLegacy(blockSize);
    }

    void Oversized:: operator delete(void *blockAddr, const size_t blockSize) noexcept
    {
        assert(blockSize>0);
        if(blockAddr)
        {
            static Memory::Pooled &mgr = Memory::Pooled::Location();
            mgr.releaseLegacy(blockAddr,blockSize);
        }
    }

    void * Oversized:: operator new[](const size_t blockSize)
    {
        static Memory::Pooled &mgr = Memory::Pooled::Instance();
        assert(blockSize>0);
        return mgr.acquireLegacy(blockSize);
    }

    void Oversized:: operator delete[](void *blockAddr, const size_t blockSize) noexcept
    {
        assert(blockSize>0);
        if(blockAddr)
        {
            static Memory::Pooled &mgr = Memory::Pooled::Location();
            mgr.releaseLegacy(blockAddr,blockSize);
        }
    }


    void * Oversized::operator new(size_t blockSize, void *addr) noexcept
    {
        assert(blockSize>0);
        assert(0!=addr);
        memset(addr,0,blockSize);
        return addr;
    }

    void Oversized:: operator delete(void *, void *) noexcept
    {
        
    }

}


