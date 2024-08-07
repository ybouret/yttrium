//! \file

#ifndef Y_Ink_Types_Included
#define Y_Ink_Types_Included 1

#include "y/color/rgba.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{
    namespace Ink
    {

        typedef Color::RGBA<uint8_t> RGBA;        //!< alias
        typedef Memory::Dyadic       MemoryModel; //!< alias
    }

}

#endif
