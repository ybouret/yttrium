#include "y/data/small/supply.hpp"
#include "y/object.hpp"

namespace Yttrium
{
    namespace Small
    {

        Supply:: ~Supply() noexcept {}

        Supply:: Supply() noexcept {}

        void *Supply:: zacquire(const size_t blockSize)
        {
            return Object:: operator new(blockSize);
        }

        void Supply:: zrelease(void *blockAddr, const size_t blockSize) noexcept
        {
            assert(0!=blockAddr);
            assert(blockSize>0);
            
            Object:: operator delete(blockAddr,blockSize);
        }
    }

}


