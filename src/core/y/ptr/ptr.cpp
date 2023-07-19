
#include "y/ptr/ptr.hpp"
#include "y/system/error.hpp"
#include <iostream>
#include <cstdlib>

namespace Yttrium
{

    namespace Core
    {

        Ptr::  Ptr() noexcept {}
        Ptr:: ~Ptr() noexcept {}

        const char Ptr:: Nil[] = "(nil)";

        void Ptr:: Critical(const void *       addr,
                            const char * const name) noexcept
        {
            assert(0!=name);
            if(!addr) {
                std::cerr << "*** " << name  << Nil << std::endl;
                abort();
            }
        }

    }

}
