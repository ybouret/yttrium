//! \file

#ifndef Y_IOS_Char_Included
#define Y_IOS_Char_Included 1

#include "y/memory/exclusive/decl.hpp"

namespace Yttrium
{
    namespace IO
    {
        class Char
        {
        public:
            Char(const uint8_t) noexcept;
            Char(const Char &)  noexcept;
            ~Char()             noexcept;

            uint8_t &       operator*()       noexcept;
            const uint8_t & operator*() const noexcept;

            Char *next;
            Char *prev;

            Y_EXCLUSIVE_DECL();
        private:
            Y_DISABLE_ASSIGN(Char);
            uint8_t data;

        };
    }
}

#endif

