
//! \file

#ifndef Y_Lingo_Lexical_Drop_Included
#define Y_Lingo_Lexical_Drop_Included 1

#include "y/lingo/lexical/action.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {

            class Drop : public Action_
            {
            public:
                static const uint32_t UUID = Y_FOURCC('D','R','O','P');

                explicit Drop(const bool) noexcept;
                virtual ~Drop() noexcept;

            private:
                Y_DISABLE_ASSIGN(Drop);
            };

        }
    }

}

#endif

