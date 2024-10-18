//! \file

#ifndef Y_Lingo_Logic_None_Included
#define Y_Lingo_Logic_None_Included 1

#include "y/lingo/pattern/logic.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        class None : public Logic
        {
        public:
            static const uint32_t UUID = Y_FOURCC('[','^', '^', ']');

            explicit None() noexcept;
            virtual ~None() noexcept;
            None(const None &);

            virtual Pattern *clone() const;

        private:
            Y_DISABLE_ASSIGN(None);
        };
    }

}

#endif
