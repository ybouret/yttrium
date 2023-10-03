
//! \file

#ifndef Y_Jive_Lexical_JString_Included
#define Y_Jive_Lexical_JString_Included 1

#include "y/jive/lexical/plugin/string.hpp"

namespace Yttrium
{

    namespace Jive
    {

        namespace Lexical
        {
            
            class JString : public String_
            {
            public:

                template <typename ID> inline
                explicit JString(Lexer &    lx,
                                 const ID & id) :
                String_(lx,id,DQUOTES)
                {

                }

                virtual ~JString() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(JString);
            };

        }

    }

}

#endif

