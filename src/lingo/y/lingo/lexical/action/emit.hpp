//! \file

#ifndef Y_Lingo_Lexical_Emit_Included
#define Y_Lingo_Lexical_Emit_Included 1

#include "y/lingo/lexical/action.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {

            class Emit : public Action_
            {
            public:
                static const uint32_t UUID = Y_FOURCC('E','M','I','T');

                explicit Emit(const bool) noexcept;
                virtual ~Emit() noexcept;

            private:
                Y_DISABLE_ASSIGN(Emit);
            };

        }
    }

}

#endif

