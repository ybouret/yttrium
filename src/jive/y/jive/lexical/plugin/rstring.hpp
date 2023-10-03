

//! \file

#ifndef Y_Jive_Lexical_RString_Included
#define Y_Jive_Lexical_RString_Included 1

#include "y/jive/lexical/plugin/string.hpp"

namespace Yttrium
{

    namespace Jive
    {

        namespace Lexical
        {

            class RString : public String_
            {
            public:

                template <typename ID> inline
                explicit RString(Lexer &    lx,
                                 const ID & id) :
                String_(lx,id,QUOTES)
                {

                }

                virtual ~RString() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(RString);
            };

        }

    }

}

#endif

