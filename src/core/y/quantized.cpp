
#include "y/quantized.hpp"
#include "y/memory/allocator/dyadic.hpp"

#include <cstring>

namespace Yttrium
{
    Quantized::  Quantized() noexcept {}
    Quantized:: ~Quantized() noexcept {}


    void * Quantized:: operator new(const size_t blockSize)
    {
        static Memory::Dyadic &mgr = Memory::Dyadic::Instance();
        assert(blockSize>0);
        return mgr.acquireLegacy(blockSize);
    }

    void Quantized:: operator delete(void *blockAddr, const size_t blockSize) noexcept
    {
        assert(blockSize>0);
        if(blockAddr)
        {
            static Memory::Dyadic &mgr = Memory::Dyadic::Location();
            mgr.releaseLegacy(blockAddr,blockSize);
        }
    }

    void * Quantized:: operator new[](const size_t blockSize)
    {
        static Memory::Dyadic &mgr = Memory::Dyadic::Instance();
        assert(blockSize>0);
        return mgr.acquireLegacy(blockSize);
    }

    void Quantized:: operator delete[](void *blockAddr, const size_t blockSize) noexcept
    {
        assert(blockSize>0);
        if(blockAddr)
        {
            static Memory::Dyadic &mgr = Memory::Dyadic::Location();
            mgr.releaseLegacy(blockAddr,blockSize);
        }
    }


    void * Quantized::operator new(size_t blockSize, void *addr) noexcept
    {
        assert(blockSize>0);
        assert(0!=addr);
        memset(addr,0,blockSize);
        return addr;
    }

    void Quantized:: operator delete(void *, void *) noexcept
    {

    }

}


