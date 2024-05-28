
#include "y/apk/natural/latch.hpp"

namespace Yttrium
{
    namespace APK
    {
        Latch::  Latch() noexcept : next(0) {}
        Latch:: ~Latch() noexcept {}

        Latch:: Handle:: Handle() : PtrType( Latch::Query() )
        {}
        
        Latch:: Handle:: ~Handle() noexcept
        {
            assert(0!=handle);
            Latch::Store(handle);
            handle = 0;
        }

    }

}
