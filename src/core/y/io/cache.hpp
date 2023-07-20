
//! \file

#ifndef Y_IO_Cache_Included
#define Y_IO_Cache_Included 1

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
        //! Versatile cache of Chars
        //
        //
        //______________________________________________________________________
        class Cache : public CxxListOf<Char>
        {
        public:
            explicit Cache() noexcept;
            virtual ~Cache() noexcept;
            Cache(const Cache &);
            
            Cache & operator<<(const uint8_t);
            Cache & operator>>(const uint8_t);
            Cache & operator<<(const char *);

            uint8_t pullTail() noexcept;
            uint8_t pullHead() noexcept;

        private:
            Y_DISABLE_ASSIGN(Cache);
        };
    }

}

#endif

