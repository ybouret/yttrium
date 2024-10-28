//! \file

#ifndef Y_Lingo_Lexical_Back_Included
#define Y_Lingo_Lexical_Back_Included 1

#include "y/lingo/lexical/action.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {

            class Back : public Action_
            {
            public:
                static const uint32_t UUID = Y_FOURCC('B','A','C','K');

                explicit Back(const bool) noexcept;
                virtual ~Back() noexcept;

            private:
                Y_DISABLE_ASSIGN(Back);
            };

        }
    }

}

#endif

