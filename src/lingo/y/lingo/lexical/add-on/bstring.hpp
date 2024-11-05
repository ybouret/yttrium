

//! \file

#ifndef Y_Lingo_Lexical_BString_Included
#define Y_Lingo_Lexical_BString_Included 1

#include "y/lingo/lexical/add-on/string.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        namespace Lexical
        {

            class BString : public String_
            {
            public:
                template <
                typename LABEL> inline
                explicit BString(Lexer       & rootLexer,
                                 const LABEL & addOnUUID) :
                String_(rootLexer,addOnUUID,'<','>')
                {
                }

                virtual ~BString() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(BString);
            };
        }

    }

}

#endif

