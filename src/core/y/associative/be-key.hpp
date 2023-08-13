//! \file

#ifndef Y_Associative_BE_Key_Included
#define Y_Associative_BE_Key_Included 1

#include "y/memory/buffer/ro.hpp"

namespace Yttrium
{

    class BE_Key : public Memory::ReadOnlyBuffer
    {
    public:
        template <typename T>
        inline BE_Key(T &args) noexcept :
        code()
        {

        }

        BE_Key(const BE_Key &) noexcept;
        virtual ~BE_Key()      noexcept;
        
    private:
        Y_DISABLE_ASSIGN(BE_Key);
        uint8_t code[8];
        
    };

}

#endif

