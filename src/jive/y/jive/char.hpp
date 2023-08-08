//! \file

#ifndef Y_Jive_Char_Included
#define Y_Jive_Char_Included 1

#include "y/jive/context.hpp"
#include "y/memory/exclusive/decl.hpp"
#include "y/data/list/cxx.hpp"

namespace Yttrium
{
    namespace Jive
    {

        class Char : public Context
        {
        public:
            typedef CxxListOf<Char> List;

            explicit Char(const Context &, const uint8_t) noexcept;
            virtual ~Char()    noexcept;
            Char(const Char &) noexcept;

            uint8_t &       operator*()       noexcept;
            const uint8_t & operator*() const noexcept;

            Char *  next;
            Char *  prev;

            //__________________________________________________________________
            //
            //! managed by a Memory::Studio
            //__________________________________________________________________
            Y_EXCLUSIVE_DECL();

        private:
            uint8_t data;
            Y_DISABLE_ASSIGN(Char);
        };
    }
}

#endif

