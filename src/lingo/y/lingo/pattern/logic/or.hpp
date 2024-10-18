//! \file

#ifndef Y_Lingo_Logic_Or_Included
#define Y_Lingo_Logic_Or_Included 1

#include "y/lingo/pattern/logic.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        class Or : public Logic
        {
        public:
            static const uint32_t UUID = Y_FOURCC('[','|', '|', ']');

            explicit Or() noexcept;
            virtual ~Or() noexcept;
            Or(const Or &);

            virtual Pattern *clone() const;

        private:
            Y_DISABLE_ASSIGN(Or);
        };
    }

}

#endif
