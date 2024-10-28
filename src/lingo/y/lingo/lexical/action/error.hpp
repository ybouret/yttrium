//! \file

#ifndef Y_Lingo_Lexical_Error_Included
#define Y_Lingo_Lexical_Error_Included 1

#include "y/lingo/lexical/action.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {

            class Error : public Action_
            {
            public:
                static const uint32_t UUID = Y_FOURCC('E','R','R','!');

                explicit Error() noexcept;
                virtual ~Error() noexcept;

            private:
                Y_DISABLE_ASSIGN(Error);
            };

        }
    }

}

#endif

