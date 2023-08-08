//! \file

#ifndef Y_Jive_Context_Included
#define Y_Jive_Context_Included 1

#include "y/jive/mark.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Jive
    {
        
        class Context
        {
        public:
            Context(const Mark &)     noexcept;
            Context(String *)        noexcept;
            Context(const Context &) noexcept;
            virtual ~Context()       noexcept;

            Exception & stamp(Exception &) const noexcept;


            const Mark     mark;
            const unsigned line;
            const unsigned column;


        private:
            Y_DISABLE_ASSIGN(Context);
        };
    }

}

#endif

