
//! \file

#ifndef Y_Lingo_Lexical_Call_Included
#define Y_Lingo_Lexical_Call_Included 1

#include "y/lingo/lexical/action.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {

            class Call : public Action_
            {
            public:
                static const uint32_t UUID = Y_FOURCC('C','A','L','L');

                explicit Call(const bool, const Caption &)  noexcept;
                virtual ~Call() noexcept;

                const Caption name;
                
            private:
                Y_DISABLE_ASSIGN(Call);
            };

        }
    }

}

#endif

