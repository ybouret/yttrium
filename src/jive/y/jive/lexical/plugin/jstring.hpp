
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

            //__________________________________________________________________
            //
            //
            //
            //! Just a String (JSON/C style)
            //
            //
            //__________________________________________________________________
            class JString : public String_
            {
            public:

                //! setup
                template <typename ID> inline
                explicit JString(Lexer &    lx,
                                 const ID & id) :
                String_(lx,id,DQUOTES)
                {

                }

                //! cleanup
                virtual ~JString() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(JString);
            };

        }

    }

}

#endif

