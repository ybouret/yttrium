//! \file

#ifndef Y_Jive_Context_Included
#define Y_Jive_Context_Included 1

#include "y/jive/tags.hpp"

namespace Yttrium
{
    namespace Jive
    {

        class Context
        {
        public:
            explicit Context(const Tag &) noexcept;
            Context(const Context &) noexcept;
            virtual ~Context() noexcept;

            const Tag      tag;
            const size_t   line;
            const size_t   column;

        private:
            Y_DISABLE_ASSIGN(Context);
        };
    }

}

#endif

