
//! \file

#ifndef Y_IO_Chars_Included
#define Y_IO_Chars_Included 1

#include "y/io/char.hpp"
#include "y/data/list/cxx.hpp"

namespace Yttrium
{
    namespace IO
    {
        //______________________________________________________________________
        //
        //
        //
        //! Versatile list of Chars
        //
        //
        //______________________________________________________________________
        class Chars : public CxxListOf<Char>
        {
        public:
            explicit Chars() noexcept;
            virtual ~Chars() noexcept;
            Chars(const Chars &);
            
            Chars & operator<<(const uint8_t);
            Chars & operator>>(const uint8_t);
            Chars & operator<<(const char *);

            uint8_t pullTail() noexcept;
            uint8_t pullHead() noexcept;

        private:
            Y_DISABLE_ASSIGN(Chars);
        };
    }

}

#endif

